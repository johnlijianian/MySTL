//
// Created by LJN on 2021/11/15.
//

#ifndef MYSTL_STL_ALGOBASE_H
#define MYSTL_STL_ALGOBASE_H

template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
    for (; first1 != last1; ++first1, ++first2)
        if(*first1 != *first2)
            return false;
        return true;
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred){
    for(; first1 != last1; ++first1, ++first2)
        if(!binary_pred(*first1, *first2))
            return false;
        return true;
}

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value){
    for(; first != last; ++first)
        *first = value;
}

template <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b){
    __iter_swap(a,b,value_type(a));
}

template <class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*){
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2){
        if(*first1 < *first2)
            return true;
        if(*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2, Compare comp){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2){
        if(comp(*first1, *first2))
            return true;
        if(comp(*first1, *first2))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

inline bool lexicographical_compare(const unsigned char* first1, const unsigned  char* last1, const unsigned char* first2, const unsigned char* last){
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result != 0 ? result < 0 : len1 < len2;
}

template <class T>
inline const T& max(const T&a, const T&b){
    return a < b ? b : a;
}

template <class T, class Compare>
inline const T& max(const T&a, const T&b, Compare comp){
    return comp(a,b) ? b : a;
}

template <class T>
inline const T& min(const T& a, const T& b){
    return b < a ? b : a;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp){
    return comp(b,a) ? b : a;
}

template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
    while(first1 != last1 && *first1 == *first2){
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> minmatch(
        InputIterator1 first1,
        InputIterator1 last1,
        InputIterator2 first2,
        BinaryPredicate binary_pred){
    while(first1 != last1 && binary_pred(*first1, *first2){
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

inline char* copy(const char* first, const char* last, char* result){
    memove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* copy(const wchar_t * first, const wchar_t * last, wchar_t * result){
    memove(result, first, sizeof(wchar_t ) * (last - first));
    return result + (last - first);
}

template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
        return __copy_dispatch<InputIterator, OutputIterator>()
        (first, last, result);
}

template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
            OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result){
                return __copy(first, last, result, iterator_category(first));
            }
};

template <class T>
struct __copy_dispatch<T*, T*>{
    T* operator() (T* first, T* last, T* result){
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return copy_t(first, last, result, t());
    }
};

template <class T>
struct __copy_dispatch<const T*, T*>
{
    T* operator() (const T* first, const T* last, T* result){
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return copy_t(first, last, result, t());
    }
};

template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag){
    for (; first != last ; ++result, ++first)
        *result = *first;
    return result;
}

template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag){
    return __copy_d(first, last, result, distance_type(first));
}

template <class RandomAccessIterator, class OutputIterator, class Disstance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Disstance*){
    for (Disstance n = last - first; n > 0; --n, ++result, ++first)
        *result = *first;
    return result;
}

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type){
    memove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
    return __copy_d(first, last, result, (ptrdiff_t *) 0);
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result){
    while(first1 != last1 && first2 != last2){
        if(*first1 < *first2){
            *result = *first1;
            ++first1;
        }else if(*first2 < *first1){
            *result = *first2;
            ++first2;
        }else{
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result){
    while(first1 != last1 && first2 != last2){
        if(*first1 < *first2)
            ++first1;
        else if(*first2 < *first1)
            ++first2;
        else{
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
        return result;
    }
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result){
    while(first1 != last1 && first2 != last2){
        if(*first1 < *first2){
            *result = *first1;
            ++first1;
            ++result;
        }else if(*first2 < *first1)
            ++first2;
        else{
            ++first1;
            ++first2;
        }
        return copy(first1, last1, result);
    }
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2, OutputIterator result){
    while(first1 != last1 && first2 != last2){
        if (*first1 < *first2){
            *result = *first1;
            ++first1;
            ++result;
        }else if(*first2 < *first1) {
            *result = *first2;
            ++first2;
            ++result;
        }else{
            ++first1;
            ++first2;
        }
        return copy(first2, last2, copy(first1, last1, result));
    }
}



#endif //MYSTL_STL_ALGOBASE_H
