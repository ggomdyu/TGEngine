#include "PrecompiledHeader.h"

#include "Batch.h"

namespace tgon
{
    
//Batch::Batch(const std::shared_ptr<Material>& material) :
//    m_material(material)
//{
//}
//
//Batch::Batch(const std::shared_ptr<Material>& material, const std::initializer_list<DrawPrimitive>& drawPrimitives) :
//    m_material(material),
//    m_drawPrimitives(drawPrimitives)
//{
//}
//
//void Batch::AddDrawPrimitive(const std::shared_ptr<Mesh>& mesh, const Matrix4x4* matWorld)
//{
//    m_drawPrimitives.push_back(DrawPrimitive{mesh, matWorld});
//}
//
//void Batch::AddDrawPrimitive(const DrawPrimitive& drawPrimitive)
//{
//    m_drawPrimitives.push_back(drawPrimitive);
//}
//    
//bool Batch::CanBatch(const std::shared_ptr<Material>& material) const
//{
//    return m_material->CanBatch(*material);
//}
//    
//void Batch::Draw(Graphics& graphics, const Camera& camera)
//{
//    m_material->Use();
//
//    for (auto& drawPrimitives : m_drawPrimitives)
//    {
//        // Set the world-view-projection matrix.
//        m_material->SetWVP(*drawPrimitives.matWorld * camera.GetViewProjectionMatrix());
//
//        auto& mesh = drawPrimitives.mesh;
//        auto& vertexBuffer = mesh->GetVertexBuffer();
//        auto& indexBuffer = mesh->GetIndexBuffer();
//
//        vertexBuffer->Use();
//        indexBuffer->Use();
//
//        int32_t primitiveCount = static_cast<int32_t>(indexBuffer->GetDataBytes() / sizeof(unsigned int));
//        graphics.DrawPrimitives(PrimitiveType::Triangles, primitiveCount);
//    }
//}
//    
//void BatchGroup::AddBatch(const std::shared_ptr<Material>& material, const Batch::DrawPrimitive& drawPrimitive)
//{
//    for (auto& batch : m_batches)
//    {
//        if (batch.CanBatch(material))
//        {
//            batch.AddDrawPrimitive(drawPrimitive);
//            return;
//        }
//    }
//    
//    m_batches.push_back(Batch(material, {drawPrimitive}));
//}
//
//void BatchGroup::AddBatch(const Batch& batch)
//{
//    m_batches.push_back(batch);
//}
//
//void BatchGroup::FlushBatch(Graphics& graphics, const Camera& camera)
//{
//    for (auto& batch : m_batches)
//    {
//        batch.Draw(graphics, camera);
//    }
//}
    
} /* namespace tgon */
