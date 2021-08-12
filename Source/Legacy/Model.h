/**
 * @file    Mesh.h
 * @author  ggomdyu
 * @since   01/06/2018
 */

#pragma once
#include <memory>

#include "Math/Matrix4x4.h"
#include "Platform/Config.h"

#include "Material.h"
#include "Mesh.h"

namespace tgon
{

class TGON_API Model final
{
/* @section Constructor */
public:
    /* @brief   Initializes the mesh renderer with mesh, material, and world-view-projection matrix. */
    Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    
/* @section Method */
public:
    /* @brief   Sets the mesh. */
    void SetMesh(const std::shared_ptr<Mesh>& mesh);
    
    /* @brief   Sets the material. */
    void SetMaterial(const std::shared_ptr<Material>& mesh);
    
    /* @brief   Gets the mesh. */
    const std::shared_ptr<Mesh>& GetMesh() const noexcept;
    
    /* @brief   Gets the material. */
    const std::shared_ptr<Material>& GetMaterial() const noexcept;
    
/* @section Variable */
protected:
    std::shared_ptr<Mesh> m_mesh;
    
    std::shared_ptr<Material> m_material;
};

} /* namespace tgon */
