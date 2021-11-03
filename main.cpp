#include <iostream>
#include <vector>
//#include "allocator.h"

#include "iterator.h"
#include<typeinfo>

using namespace std;

class A{
public:
    int a;
//    A():a(10){};
};

int main() {
    int *a=new int[1000];
    for(int i=0;i<1000;i++){
        a[i]=i+1;
    }
    delete[] a;
    int *b=new int[1000]();
    for(int i=0;i<100;i++){
        cout<<b[i]<<endl;
    }

    return 0;
}
