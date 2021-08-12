#include "PrecompiledHeader.h"

#include "Model.h"

namespace tgon
{

Model::Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) :
    m_mesh(mesh),
    m_material(material)
{
}

void Model::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_mesh = mesh;
}
    
void Model::SetMaterial(const std::shared_ptr<Material>& material)
{
    m_material = material;
}
    
const std::shared_ptr<Mesh>& Model::GetMesh() const noexcept
{
    return m_mesh;
}
    
const std::shared_ptr<Material>& Model::GetMaterial() const noexcept
{
    return m_material;
}

} /* namespace tgon */
