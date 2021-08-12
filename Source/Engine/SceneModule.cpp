#include "PrecompiledHeader.h"

#include "SceneModule.h"

namespace tg
{

void Scene::Update()
{
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
        {
            gameObject->Update();
        }
    }
}

void SceneModule::Update()
{
    if (m_activeScene != nullptr)
    {
        m_activeScene->Update();
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
