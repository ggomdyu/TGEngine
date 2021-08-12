#include "PrecompiledHeader.h"

#include "TextureAtlasTree.h"

namespace tgon
{

TextureAltasNode::TextureAltasNode(const I32Rect& rect) :
    rect(rect),
    id(0)
{
}

TextureAltasNode* TextureAltasNode::Insert(KeyType key, const I32Extent2D& size, int32_t paddingOffset)
{
    if (left == nullptr && right == nullptr)
    {
        bool isImageAlreadyAssigned = (this->id != 0);
        if (isImageAlreadyAssigned)
        {
            return nullptr;
        }

        int32_t paddedImageWidth = size.width + paddingOffset;
        int32_t paddedImageHeight = size.height + paddingOffset;
        if (rect.width < paddedImageWidth || rect.height < paddedImageHeight)
        {
            return nullptr;
        }
        
        if (rect.width == paddedImageWidth && rect.height == paddedImageHeight)
        {
            this->id = key;
            return this;
        }

        if (rect.width - paddedImageWidth > rect.height - paddedImageHeight)
        {
            left = std::make_unique<TextureAltasNode>(I32Rect(rect.x, rect.y, paddedImageWidth, rect.height));
            right = std::make_unique<TextureAltasNode>(I32Rect(rect.x + paddedImageWidth, rect.y, rect.width - paddedImageWidth, rect.height));
        }
        else
        {
            left = std::make_unique<TextureAltasNode>(I32Rect(rect.x, rect.y, rect.width, paddedImageHeight));
            right = std::make_unique<TextureAltasNode>(I32Rect(rect.x, rect.y + paddedImageHeight, rect.width, rect.height - paddedImageHeight));
        }
        
        return left->Insert(key, size, paddingOffset);
    }
    else
    {
        auto insertedNode = left->Insert(key, size, paddingOffset);
        if (insertedNode == nullptr)
        {
            insertedNode = right->Insert(key, size, paddingOffset);
        }

        return insertedNode;
    }
    
    return nullptr;
}

bool TextureAltasNode::IsLeafNode() const noexcept
{
    return left == nullptr && right == nullptr;
}

TextureAtlasTree::TextureAtlasTree(const I32Extent2D& textureAtlasSize, int32_t paddingOffset) :
    m_rootNode(I32Rect(0, 0, textureAtlasSize.width, textureAtlasSize.height)),
    m_paddingOffset(paddingOffset)
{
}

TextureAtlasTree::IteratorType TextureAtlasTree::Insert(KeyType id, const I32Extent2D& size)
{
    return m_rootNode.Insert(id, size, m_paddingOffset);
}

int32_t TextureAtlasTree::GetPaddingOffset() const noexcept
{
    return m_paddingOffset;
}

const TextureAltasNode* TextureAtlasTree::GetRootNode() const noexcept
{
    return &m_rootNode;
}

TextureAltasNode* TextureAtlasTree::GetRootNode() noexcept
{
    return &m_rootNode;
}

//static _Rb_tree_node_base*
//local_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
//{
//    if (__x->_M_right != 0)
//    {
//        __x = __x->_M_right;
//        while (__x->_M_left != 0)
//            __x = __x->_M_left;
//    }
//    else
//    {
//        _Rb_tree_node_base* __y = __x->_M_parent;
//        while (__x == __y->_M_right)
//        {
//            __x = __y;
//            __y = __y->_M_parent;
//        }
//        if (__x->_M_right != __y)
//            __x = __y;
//    }
//    return __x;
//}

} /* namespace tgon */
