#include "PrecompiledHeader.h"

#include <android/sensor.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

struct android_poll_source
{
    int32_t id;
    struct android_app* app;
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

struct android_app
{
    void* userData;
    void (*onAppCmd)(struct android_app* app, int32_t cmd);
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);
    ANativeActivity* activity;
    AConfiguration* config;
    void* savedState;
    size_t savedStateSize;
    ALooper* looper;
    AInputQueue* inputQueue;
    ANativeWindow* window;
    ARect contentRect;
    int activityState;
    int destroyRequested;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int msgread;
    int msgwrite;
    pthread_t thread;
    android_poll_source cmdPollSource;
    android_poll_source inputPollSource;
    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
    ARect pendingContentRect;
};

struct saved_state
{
    float angle;
    int32_t x;
    int32_t y;
};

struct engine
{
    android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    int32_t width;
    int32_t height;
    saved_state state;
};

namespace tgon
{

extern int main(int argc, char** argv);

}

//void android_main(struct android_app* state)
//{
//    tgon::engine engine {};
//	state->userData = &engine;
//	state->onAppCmd = engine_handle_cmd;
//	state->onInputEvent = engine_handle_input;
//	engine.app = state;
//
//	// ���ӵ��� ����͸��� �غ��մϴ�.
//	engine.sensorManager = ASensorManager_getInstance();
//	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
//		ASENSOR_TYPE_ACCELEROMETER);
//	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
//		state->looper, LOOPER_ID_USER, NULL, NULL);
//
//	if (state->savedState != NULL) {
//		// ������ ����� ���·� ���۵Ǹ�, �� �������� �����˴ϴ�.
//		engine.state = *(struct saved_state*)state->savedState;
//	}
//
//	engine.animating = 1;
//
//	//������ �۾��� ����ϸ鼭 ������ �����մϴ�.
//
//	while (1) {
//		// ���� ���� ��� �̺�Ʈ�� �н��ϴ�.
//		int ident;
//		int events;
//		struct android_poll_source* source;
//
//		// �ִϸ��̼��� �������� ������ �̺�Ʈ ��⸦ ���������� �����մϴ�.
//		// �ִϸ��̼��� �����ϸ� ��� �̺�Ʈ�� ���� ������ ������ ������ ����
//		// ����ؼ� �ִϸ��̼��� ���� �������� �׸��ϴ�.
//		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
//			(void**)&source)) >= 0) {
//
//			// �� �̺�Ʈ�� ó���մϴ�.
//			if (source != NULL) {
//				source->process(state, source);
//			}
//
//			// ������ �����Ͱ� ������ �ٷ� ó���˴ϴ�.
//			if (ident == LOOPER_ID_USER) {
//				if (engine.accelerometerSensor != NULL) {
//					ASensorEvent event;
//					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
//						&event, 1) > 0) {
//						LOGI("accelerometer: x=%f y=%f z=%f",
//							event.acceleration.x, event.acceleration.y,
//							event.acceleration.z);
//					}
//				}
//			}
//
//			// ���� ������ Ȯ���մϴ�.
//			if (state->destroyRequested != 0) {
//				engine_term_display(&engine);
//				return;
//			}
//		}
//
//		if (engine.animating) {
//			// �̺�Ʈ�� ������ �� ���� �ִϸ��̼� �������� �׸��ϴ�.
//			engine.state.angle += .01f;
//			if (engine.state.angle > 1) {
//				engine.state.angle = 0;
//			}
//
//			// �׸���� ȭ�� ������Ʈ �ӵ��� ������ �����Ƿ�
//			// ���⿡���� Ÿ�̹��� ����� �ʿ䰡 �����ϴ�.
//			engine_draw_frame(&engine);
//		}
//	}
//}

enum {
    /**
     * ���� �� �����忡�� ���� ����� Looper ������ ID��
     * ALooper_pollOnce()�� �ĺ��ڷ� ��ȯ�˴ϴ�. �� �ĺ��ڿ�
     * ���� �����ʹ� android_poll_source ������ ���� �������Դϴ�.
     * android_app_read_cmd()
     * �� android_app_exec_cmd()�� �˻��ϰ� ó���� �� �ֽ��ϴ�.
     */
    LOOPER_ID_MAIN = 1,

    /**
     * ���� ���α׷� â�� AInputQueue���� ���� �̺�Ʈ�� Looper ������ ID��
     * ALooper_pollOnce()���� �ĺ��ڷ� ��ȯ�˴ϴ�.
     * �� �ĺ��ڿ� ���� �����ʹ�
     * android_poll_source ������ �������Դϴ�. android_app�� inputQueue
     * ��ü�� ���� ���� �� �ֽ��ϴ�.
     */
    LOOPER_ID_INPUT = 2,

    /**
     * ����� ���ǵ� ALooper �ĺ����� �����Դϴ�.
     */
    LOOPER_ID_USER = 3,
};

enum
{
    APP_CMD_INPUT_CHANGED,
    APP_CMD_INIT_WINDOW,
    APP_CMD_TERM_WINDOW,
    APP_CMD_WINDOW_RESIZED,
    APP_CMD_WINDOW_REDRAW_NEEDED,
    APP_CMD_CONTENT_RECT_CHANGED,
    APP_CMD_GAINED_FOCUS,
    APP_CMD_LOST_FOCUS,
    APP_CMD_CONFIG_CHANGED,
    APP_CMD_LOW_MEMORY,
    APP_CMD_START,
    APP_CMD_RESUME,
    APP_CMD_SAVE_STATE,
    APP_CMD_PAUSE,
    APP_CMD_STOP,
    APP_CMD_DESTROY,
};

void FreeSavedState(android_app* app)
{
    if (app->savedState == NULL)
    {
        return;
    }

    pthread_mutex_lock(&app->mutex);
    {
        free(app->savedState);
        app->savedState = NULL;
        app->savedStateSize = 0;
    }
    pthread_mutex_unlock(&app->mutex);
}

bool PullMessage(android_app* android_app, int8_t* cmd)
{
    return read(android_app->msgread, cmd, sizeof(*cmd)) == sizeof(*cmd) ? true : false;
}

void PreCommandExecProc(android_app* app, int8_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_INPUT_CHANGED:
        pthread_mutex_lock(&app->mutex);
        {
            if (app->inputQueue != NULL)
            {
                AInputQueue_detachLooper(app->inputQueue);
            }
            if (app->pendingInputQueue != NULL)
            {
                app->inputQueue = app->pendingInputQueue;
                AInputQueue_attachLooper(app->inputQueue, app->looper, LOOPER_ID_INPUT, NULL, &app->inputPollSource);
            }
        }
        pthread_cond_broadcast(&app->cond);
        pthread_mutex_unlock(&app->mutex);
        break;

    case APP_CMD_INIT_WINDOW:
        pthread_mutex_lock(&app->mutex);
        {
            app->window = app->pendingWindow;
        }
        pthread_cond_broadcast(&app->cond);
        pthread_mutex_unlock(&app->mutex);
        break;

    case APP_CMD_TERM_WINDOW:
        pthread_cond_broadcast(&app->cond);
        break;

    case APP_CMD_RESUME:
    case APP_CMD_START:
    case APP_CMD_PAUSE:
    case APP_CMD_STOP:
        pthread_mutex_lock(&app->mutex);
        {
            app->activityState = cmd;
        }
        pthread_cond_broadcast(&app->cond);
        pthread_mutex_unlock(&app->mutex);
        break;

    case APP_CMD_CONFIG_CHANGED:
        AConfiguration_fromAssetManager(app->config, app->activity->assetManager);
        break;

    case APP_CMD_SAVE_STATE:
        FreeSavedState(app);
        break;

    case APP_CMD_DESTROY:
        app->destroyRequested = 1;
        break;
    }
}

void PostCommandExecProc(android_app* android_app, int8_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_TERM_WINDOW:
        pthread_mutex_lock(&android_app->mutex);
        {
            android_app->window = NULL;
        }
        pthread_cond_broadcast(&android_app->cond);
        pthread_mutex_unlock(&android_app->mutex);
        break;

    case APP_CMD_SAVE_STATE:
        pthread_mutex_lock(&android_app->mutex);
        {
            android_app->stateSaved = 1;
        }
        pthread_cond_broadcast(&android_app->cond);
        pthread_mutex_unlock(&android_app->mutex);
        break;

    case APP_CMD_RESUME:
        FreeSavedState(android_app);
        break;
    }
}

static void ProcessCommand(android_app* app, android_poll_source* pollSource)
{
    int8_t cmd;
    if (PullMessage(app, &cmd))
    {
        PreCommandExecProc(app, cmd);
        {
            if (app->onAppCmd != NULL)
            {
                app->onAppCmd(app, cmd);
            }
        }
        PostCommandExecProc(app, cmd);
    }
}

static void ProcessInput(android_app* app, android_poll_source* source)
{
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(app->inputQueue, event))
        {
            continue;
        }

        int32_t handled = 0;
        if (app->onInputEvent != NULL)
        {
            handled = app->onInputEvent(app, event);
        }

        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

static void* android_app_entry(void* param)
{
    android_app* app = reinterpret_cast<android_app*>(param);

    app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(app->config, app->activity->assetManager);

    app->cmdPollSource.id = LOOPER_ID_MAIN;
    app->cmdPollSource.app = app;
    app->cmdPollSource.process = ProcessCommand;

    app->inputPollSource.id = LOOPER_ID_INPUT;
    app->inputPollSource.app = app;
    app->inputPollSource.process = ProcessInput;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL, &app->looper);

    pthread_mutex_lock(&app->mutex);
    {
        app->running = 1;
    }
    pthread_cond_broadcast(&app->cond);
    pthread_mutex_unlock(&app->mutex);

}