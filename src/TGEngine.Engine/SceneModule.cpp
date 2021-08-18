#include "SceneModule.h"

TG_NAMESPACE_BEGIN

void Scene::Update()
{
    for (auto& gameObject : _gameObjects)
    {
        if (gameObject->IsActive())
        {
            gameObject->Update();
        }
    }
}

void SceneModule::Update()
{
    if (_activeScene != nullptr)
    {
        _activeScene->Update();
    }
}

void SceneModule::NewScene(NewSceneSetup newSceneSetup)
{
}

void SceneModule::LoadScene(const std::string& path, LoadSceneMode loadSceneMode)
{
}

//std::shared_ptr<GameObject> SceneManagement::Instantiate()
//{
//}
}
