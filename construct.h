//
// Created by LJN on 2021/10/29.
//

#ifndef MYSTL_CONSTRUCT_H
#define MYSTL_CONSTRUCT_H
# include <new.h>
template <class T1, class T2>
inline void construct(T1 *p, const T2& value){
    new(p) T1(value);
}

template <class T>
inline void destroy(T* pointer){
    pointer->~T();
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last){
    __destroy(first, last, value_type(first));
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*){
    // typedef typename语法
    // https://blog.csdn.net/zhangxiao93/article/details/50569924
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type){
    for(; first < last; ++ first)
        destroy(&*first);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type){}

inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *){}
#endif //MYSTL_CONSTRUCT_H
