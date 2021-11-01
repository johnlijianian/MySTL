#include <iostream>
#include <vector>
//#include "allocator.h"

#include "iterator.h"
#include<typeinfo>

//using namespace std;

int main() {
    typename iterator_traits<const char*>::difference_type abc;
    int c;
    std::cout<<typeid(static_cast<typename iterator_traits<int *>::difference_type *>(0));
    return 0;

}
