#pragma once

#include <string>

#include "Core/Delegate.h"
#include "Game/GameObject.h"

#include "Module.h"

namespace tg
{

enum class NewSceneSetup
{
    EmptyScene,
    DefaultGameObjects,
};

enum class LoadSceneMode
{
    Single,
    Additive,
};

class Scene :
    public RuntimeObject
{
public:
    TGON_RTTI(Scene)

/**@section Method */
public:
    void Initialize();
    void Update();

private:
    std::vector<std::shared_ptr<GameObject>> m_gameObjects;
};

class SceneModule :
    public Module
{
public:
    TGON_RTTI(SceneModule)

/**@section Type */
public:
    using NewSceneCreatedCallback = Delegate<void(const std::shared_ptr<Scene>&, NewSceneSetup)>;
    using SceneOpeningCallback = Delegate<void(const std::shared_ptr<Scene>&, LoadSceneMode)>;
    using SceneOpenCallback = Delegate<void(const std::shared_ptr<Scene>&, LoadSceneMode)>;

/**@section Method */
public:
    /**
     * @brief   Updates the frame of the object.
     */
    void Update() override;

    /**
     * @brief   Creates a new scene.
     * @param newSceneSetup     The setup parameter allows you to select whether or not the default set of object should be added to the new scene.
     */
    void NewScene(NewSceneSetup newSceneSetup);

    /**
     * @brief   Loads the specified name of scene.
     * @param name              The name of scene.
     * @param loadSceneMode     It allows you to select whether or not merge objects to active scene.
     */
    void LoadScene(const std::string& name, LoadSceneMode loadSceneMode);

    /**
     * @brief   Instantiate a new GameObject to the active scene.
     * @return  A new instantiated object.
     */
    std::shared_ptr<GameObject> Instantiate();

    /**
     * @brief   Makes object don't destroy on load new scene.
     * @param object    The target object.
     */
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object);

/**@section Variable */
public:
    static constexpr auto ModuleStage = ModuleStage::Update;
    SceneOpeningCallback OnOpeningScene;
    SceneOpenCallback OnOpenScene;

protected:
    std::shared_ptr<Scene> m_activeScene;
    std::vector<std::shared_ptr<Scene>> m_sceneList;
};

}
