//
// Created by LJN on 2021/11/17.
//

#ifndef MYSTL_STL_HEAP_H
#define MYSTL_STL_HEAP_H

template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last){
    if(first == last) return last;
    ForwardIterator next = first;
    while (++next != last){
        if (*first == *next) return first;
        first = next;
    }
    return last;
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
    if(first == last) return last;
    ForwardIterator next = first;
    while (++next != last){
        if (binary_pred(*first, *next)) return first;
        first = next;
    }
    return last;
}

template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value){
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(; first != last; ++first)
        if (*first == value)
            ++n;
        return n;
}

template <class InputIterator, class T, class Size>
void count(InputIterator first, InputIterator last, const T& value, Size& n){
    for(; first != last; ++first)
        if (*first != value)
            ++n;
}

template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if (InputIterator first, InputIterator last, Predicate pred){
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(; first != last; ++first)
        if (pred(*first))
            ++n;
    return n;
}

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value){
    while(first != last && *first != value) ++first;
    return first;
}

template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred){
    while (first != last && !pred(*first)) ++first;
    return first;
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1,
         ForwardIterator2 first2, ForwardIterator2 last2){
    typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
    typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

    return __find_end(first1, last1, first2, last2, category1(), category2());
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag){
    if (first2 == last2)
        return last1;
    else{
        ForwardIterator1 result = last1;
        while (1){
            ForwardIterator1 new_result = search(first1, last1, first2, last2);
            if (new_result == last1)
                return result;
            else{
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                                  BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                                  bidirectional_iterator_tag, bidirectional_iterator_tag){
    typedef reverse_iterator<BidirectionalIterator1> reviter1;
    typedef reverse_iterator<BidirectionalIterator2> reviter2;

    reviter1 rlast1(first1);
    reviter2 rlast2(first2);

    reviter1 rresult = search(reviter1(last1), rlast1,
                              reviter2(last2), rlast2);

    if (rresult == rlast1)
        return last1;
    else{
        BidirectionalIterator1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
    }
}

template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2)
{
    for (; first1 != last1 ; ++first1)
        for (ForwardIterator iter = first2; iter != last2; ++iter) {
            if (*first1 == *iter)
                return first1;
            return last1;
        }
}

template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp){

}




#endif //MYSTL_STL_HEAP_H
