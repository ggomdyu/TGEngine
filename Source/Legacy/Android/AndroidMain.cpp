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
//	// 가속도계 모니터링을 준비합니다.
//	engine.sensorManager = ASensorManager_getInstance();
//	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
//		ASENSOR_TYPE_ACCELEROMETER);
//	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
//		state->looper, LOOPER_ID_USER, NULL, NULL);
//
//	if (state->savedState != NULL) {
//		// 이전에 저장된 상태로 시작되며, 이 지점에서 복원됩니다.
//		engine.state = *(struct saved_state*)state->savedState;
//	}
//
//	engine.animating = 1;
//
//	//수행할 작업을 대기하면서 루프를 실행합니다.
//
//	while (1) {
//		// 보류 중인 모든 이벤트를 읽습니다.
//		int ident;
//		int events;
//		struct android_poll_source* source;
//
//		// 애니메이션이 동작하지 않으면 이벤트 대기를 영구적으로 차단합니다.
//		// 애니메이션이 동작하면 모든 이벤트를 읽을 때까지 루프를 실행한 다음
//		// 계속해서 애니메이션의 다음 프레임을 그립니다.
//		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
//			(void**)&source)) >= 0) {
//
//			// 이 이벤트를 처리합니다.
//			if (source != NULL) {
//				source->process(state, source);
//			}
//
//			// 센서에 데이터가 있으면 바로 처리됩니다.
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
//			// 종료 중인지 확인합니다.
//			if (state->destroyRequested != 0) {
//				engine_term_display(&engine);
//				return;
//			}
//		}
//
//		if (engine.animating) {
//			// 이벤트를 종료한 후 다음 애니메이션 프레임을 그립니다.
//			engine.state.angle += .01f;
//			if (engine.state.angle > 1) {
//				engine.state.angle = 0;
//			}
//
//			// 그리기는 화면 업데이트 속도의 제한을 받으므로
//			// 여기에서는 타이밍을 계산할 필요가 없습니다.
//			engine_draw_frame(&engine);
//		}
//	}
//}

enum {
    /**
     * 앱의 주 스레드에서 오는 명령의 Looper 데이터 ID로
     * ALooper_pollOnce()의 식별자로 반환됩니다. 이 식별자에
     * 대한 데이터는 android_poll_source 구조에 대한 포인터입니다.
     * android_app_read_cmd()
     * 및 android_app_exec_cmd()로 검색하고 처리할 수 있습니다.
     */
    LOOPER_ID_MAIN = 1,

    /**
     * 응용 프로그램 창의 AInputQueue에서 오는 이벤트의 Looper 데이터 ID로
     * ALooper_pollOnce()에서 식별자로 반환됩니다.
     * 이 식별자에 대한 데이터는
     * android_poll_source 구조의 포인터입니다. android_app용 inputQueue
     * 개체를 통해 읽을 수 있습니다.
     */
    LOOPER_ID_INPUT = 2,

    /**
     * 사용자 정의된 ALooper 식별자의 시작입니다.
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