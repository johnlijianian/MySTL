//
// Created by LJN on 2021/11/2.
//

#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

template <class T> struct list_node_base;
template <class T> struct list_node;

// list 的节点结构

template <class T>
struct list_node_base
{
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    base_ptr prev;  // 前一节点
    base_ptr next;  // 下一节点

    list_node_base() = default;

    node_ptr as_node()
    {
        return static_cast<node_ptr>(self());
    }

    void unlink()
    {
        prev = next = self();
    }

    base_ptr self()
    {
        return static_cast<base_ptr>(&*this);
    }
};

template <class T>
struct list_node : public list_node_base<T>
{
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    T value;  // 数据域

    list_node() = default;
    list_node(const T& v)
            :value(v)
    {
    }
    list_node(T&& v)
            :value(mystl::move(v))
    {
    }

    base_ptr as_base()
    {
        return static_cast<base_ptr>(&*this);
    }
    node_ptr self()
    {
        return static_cast<node_ptr>(&*this);
    }
};

template <class T>
struct node_traits
{
    typedef list_node_base<T>* base_ptr;
    typedef list_node<T>*      node_ptr;
};

template<class T>
struct list_iterator{
    typedef T                                 value_type;
    typedef T*                                pointer;
    typedef T&                                reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_iterator<T>                  self;

    base_ptr node_;

    // 构造函数
    list_iterator() = default;
    list_iterator(base_ptr x)
            :node_(x) {}
    list_iterator(node_ptr x)
            :node_(x->as_base()) {}
    list_iterator(const list_iterator& rhs)
            :node_(rhs.node_) {}

    bool operator==(const self& x) const {return node == x.node;}
    bool operator!=(const self& x) const {return node != x.node;}

    reference operator*() const {return (*node).data;}

    pointer operator->() const {return &(operator*());}



    self& operator++{
        node = (link_type)((*node).next);
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        ++*this;
        reutrn tmp;
    }

    self& operator--(){
        node = (line_type)((*node).prev);
        return *this;
    }

    self& operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template <class T, class Alloc = alloc>
class list{
protected:
    typedef __list_node<T> list_node;
public:
    typedef list_node* link_type;

protected:
    link_type node;
};

#endif //MYSTL_LIST_H
