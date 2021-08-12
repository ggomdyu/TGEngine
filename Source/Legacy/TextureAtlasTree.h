/**
 * @file    TextureAtlasTree.h
 * @author  ggomdyu
 * @since   06/18/2019
 * @see     http://blackpawn.com/texts/lightmaps/
 */

#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Platform/Config.h"
#include "Math/Rect.h"
#include "Math/Extent.h"

namespace tgon
{

using KeyType = int32_t;

struct TextureAltasNode :
    private boost::noncopyable
{
    /*template<typename _Tp>
    struct Iterator
    {
        typedef _Tp  value_type;
        typedef _Tp& reference;
        typedef _Tp* pointer;

        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t			 difference_type;

        typedef _Rb_tree_iterator<_Tp>		_Self;
        typedef _Rb_tree_node_base::_Base_ptr	_Base_ptr;
        typedef _Rb_tree_node<_Tp>* _Link_type;

        _Rb_tree_iterator() _GLIBCXX_NOEXCEPT
            : _M_node() { }

        explicit
            _Rb_tree_iterator(_Base_ptr __x) _GLIBCXX_NOEXCEPT
            : _M_node(__x) { }

        reference
            operator*() const _GLIBCXX_NOEXCEPT
        {
            return *static_cast<_Link_type>(_M_node)->_M_valptr();
        }

        pointer
            operator->() const _GLIBCXX_NOEXCEPT
        {
            return static_cast<_Link_type> (_M_node)->_M_valptr();
        }

        _Self&
            operator++() _GLIBCXX_NOEXCEPT
        {
            _M_node = _Rb_tree_increment(_M_node);
            return *this;
        }

        _Self
            operator++(int) _GLIBCXX_NOEXCEPT
        {
            _Self __tmp = *this;
            _M_node = _Rb_tree_increment(_M_node);
            return __tmp;
        }

        _Self&
            operator--() _GLIBCXX_NOEXCEPT
        {
            _M_node = _Rb_tree_decrement(_M_node);
            return *this;
        }

        _Self
            operator--(int) _GLIBCXX_NOEXCEPT
        {
            _Self __tmp = *this;
            _M_node = _Rb_tree_decrement(_M_node);
            return __tmp;
        }

        friend bool
            operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
        {
            return __x._M_node == __y._M_node;
        }

        friend bool
            operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
        {
            return __x._M_node != __y._M_node;
        }

        _Base_ptr _M_node;
    };

    template<typename _Tp>
    struct _Rb_tree_const_iterator
    {
        typedef _Tp	 value_type;
        typedef const _Tp& reference;
        typedef const _Tp* pointer;

        typedef _Rb_tree_iterator<_Tp> iterator;

        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t			 difference_type;

        typedef _Rb_tree_const_iterator<_Tp>		_Self;
        typedef _Rb_tree_node_base::_Const_Base_ptr	_Base_ptr;
        typedef const _Rb_tree_node<_Tp>* _Link_type;

        _Rb_tree_const_iterator() _GLIBCXX_NOEXCEPT
            : _M_node() { }

        explicit
            _Rb_tree_const_iterator(_Base_ptr __x) _GLIBCXX_NOEXCEPT
            : _M_node(__x) { }

        _Rb_tree_const_iterator(const iterator& __it) _GLIBCXX_NOEXCEPT
            : _M_node(__it._M_node) { }

        iterator
            _M_const_cast() const _GLIBCXX_NOEXCEPT
        {
            return iterator(const_cast<typename iterator::_Base_ptr>(_M_node));
        }

        reference
            operator*() const _GLIBCXX_NOEXCEPT
        {
            return *static_cast<_Link_type>(_M_node)->_M_valptr();
        }

        pointer
            operator->() const _GLIBCXX_NOEXCEPT
        {
            return static_cast<_Link_type>(_M_node)->_M_valptr();
        }

        _Self&
            operator++() _GLIBCXX_NOEXCEPT
        {
            _M_node = _Rb_tree_increment(_M_node);
            return *this;
        }

        _Self
            operator++(int) _GLIBCXX_NOEXCEPT
        {
            _Self __tmp = *this;
            _M_node = _Rb_tree_increment(_M_node);
            return __tmp;
        }

        _Self&
            operator--() _GLIBCXX_NOEXCEPT
        {
            _M_node = _Rb_tree_decrement(_M_node);
            return *this;
        }

        _Self
            operator--(int) _GLIBCXX_NOEXCEPT
        {
            _Self __tmp = *this;
            _M_node = _Rb_tree_decrement(_M_node);
            return __tmp;
        }

        friend bool
            operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
        {
            return __x._M_node == __y._M_node;
        }

        friend bool
            operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
        {
            return __x._M_node != __y._M_node;
        }

        _Base_ptr _M_node;
    };*/

/**@section Constructor */
public:
    explicit TextureAltasNode(const I32Rect& rect);

/**@section Method */
public:
    TextureAltasNode* Insert(KeyType id, const I32Extent2D& size, int32_t paddingOffset);
    bool IsLeafNode() const noexcept;

/**@section Variable */
public:
    I32Rect rect;
    KeyType id;
    TextureAltasNode* parent;
    std::unique_ptr<TextureAltasNode> left;
    std::unique_ptr<TextureAltasNode> right;
};

class TGON_API TextureAtlasTree :
    private boost::noncopyable
{
/**@section Type */
public:
    using ReferenceType = TextureAltasNode&;
    using ConstReferenceType = const TextureAltasNode&;
    using IteratorType = TextureAltasNode*;
    using ConstIteratorType = const TextureAltasNode*;
    using ReverseIteratorType = std::reverse_iterator<IteratorType>;
    using ConstReverseIteratorType = std::reverse_iterator<ConstIteratorType>;

/**@section Constructor */
public:
    TextureAtlasTree(const I32Extent2D& textureSize, int32_t paddingOffset);

/**@section Method */
public:
    IteratorType Insert(KeyType key, const I32Extent2D& size);
    IteratorType Find(KeyType key);
    int32_t GetPaddingOffset() const noexcept;
    const TextureAltasNode* GetRootNode() const noexcept;
    TextureAltasNode* GetRootNode() noexcept;
    IteratorType begin() noexcept;
    IteratorType end() noexcept;
    ConstIteratorType cbegin() const noexcept;
    ConstIteratorType cend() const noexcept;
    ReverseIteratorType rbegin() noexcept;
    ReverseIteratorType rend() noexcept;
    ConstReverseIteratorType crbegin() const noexcept;
    ConstReverseIteratorType crend() const noexcept;

private:
    TextureAltasNode m_rootNode;
    std::unordered_map<KeyType, TextureAltasNode> m_nodeHashTable;
    KeyType m_paddingOffset;
};

} /* namespace tgon */
