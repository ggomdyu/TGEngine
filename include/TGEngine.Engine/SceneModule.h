#pragma once

#include <string>

#include "Core/Delegate.h"
#include "Game/GameObject.h"

#include "Module.h"

TG_NAMESPACE_BEGIN

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

class SceneModule :
    public Module
{
public:
    TG_OBJECT(SceneModule)

public:
    using NewSceneCreatedCallback = Delegate<void(const std::shared_ptr<Scene>&, NewSceneSetup)>;
    using SceneOpeningCallback = Delegate<void(const std::shared_ptr<Scene>&, LoadSceneMode)>;
    using SceneOpenCallback = Delegate<void(const std::shared_ptr<Scene>&, LoadSceneMode)>;

public:
    std::shared_ptr<GameObject> Instantiate();
    void NewScene(NewSceneSetup newSceneSetup);
    void LoadScene(std::string_view name, LoadSceneMode loadSceneMode);
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object);
    void Update() override;

public:
    static constexpr auto ModuleStage = ModuleStage::Update;
    SceneOpeningCallback OnOpeningScene;
    SceneOpenCallback OnOpenScene;

protected:
    std::shared_ptr<Scene> _activeScene;
    std::vector<std::shared_ptr<Scene>> _sceneList;
};

TG_NAMESPACE_END