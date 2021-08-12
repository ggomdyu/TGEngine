/**
 * @file    Batch.h
 * @author  ggomdyu
 * @since   06/03/2018
 */

#pragma once
#include <vector>
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Graphics.h"

namespace tgon
{

//class TGON_API Batch final
//{
///**@section Public struct */
//public:
//    struct DrawPrimitive
//    {
//        std::shared_ptr<Mesh> mesh;
//        const Matrix4x4* matWorld;
//    };
//
///**@section Constructor */
//public:
//    /**@brief   Initializes the batch with a material. */
//    explicit Batch(const std::shared_ptr<Material>& material);
//    
//    /**@brief   Initializes the batch with a material and primitives. */
//    explicit Batch(const std::shared_ptr<Material>& material, const std::initializer_list<DrawPrimitive>& drawPrimitives);
//
///**@section Method */
//public:
//    /**@brief   Adds a draw primitive into the batch. */
//    void AddDrawPrimitive(const std::shared_ptr<Mesh>& mesh, const Matrix4x4* matWorld);
//    
//    /**@brief   Adds a draw primitive into the batch. */
//    void AddDrawPrimitive(const DrawPrimitive& drawPrimitive);
//
//    /**@brief   Checks whether the specified material can batched. */
//    bool CanBatch(const std::shared_ptr<Material>& material) const;
//    
//    /**@brief   Draws all batched primitives. */
//    void Draw(Graphics& graphics, const Camera& camera);
//
///**@section Variable */
//private:
//    std::shared_ptr<Material> m_material;
//    
//    std::vector<Sprite> m_sprites;
//};
//    
//class TGON_API BatchGroup final
//{
///**@section Method */
//public:
//    /**@brief   Adds a mesh into the batch list. */
//    void AddBatch(const std::shared_ptr<Material>& material, const Batch::DrawPrimitive& drawPrimitive);
//    
//    /**@brief   Adds a batch into the batch list. */
//    void AddBatch(const Batch& batch);
//    
//    /**@brief   Flushes all the batch in list. */
//    void FlushBatch(Graphics& graphics, const Camera& camera);
//
///**@section Variable */
//private:
//    std::vector<Batch> m_batches;
//};
    
} /* namespace tgon */
