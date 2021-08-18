#pragma once

#include "TGEngine.Core/RuntimeObject.h"

TG_NAMESPACE_BEGIN

class Scene :
    public RuntimeObject
{
public:
    TG_OBJECT(Scene)

public:
    void Initialize();
    void Update();

private:
    std::vector<std::shared_ptr<GameObject>> _gameObjects;
};

TG_NAMESPACE_END