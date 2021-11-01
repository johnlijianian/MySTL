//
// Created by LJN on 2021/10/29.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream.h>
#   define __THROW_BAD_ALLOC cerr << "out of memory" <<endl; exit(1)
#endif

enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};

# 第一级配置器
template <int inst>
class __malloc_alloc_template {
private:
    // 内存不足的情况
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)();

public:
    static void * allocate(size_t n){
        void *result = malloc(n);
        if (0 == result) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t /* n */){
        free(p);
    }

    static void * reallocate(void *p, size_t /* old_sz */, size_t new_sz){
        void * result = reallocate(p, new_sz);
        if (0 == result) result = oom_realloc(p, new_sz);
        return result;
    }

    static void (* set_malloc_handler(void (*f)())) (){
        void(*old) () = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return(old);
    }
};

template <int inst>
void (* __malloc_alloc_template)

template <int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void(* my_malloc_handler) ();
    void *result;

    for(;;){
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(0 == my_malloc_handler) {__THROW_BAD_ALLOC;}
        (*my_malloc_handler)();
        result = malloc(n);
        if(result) return (result);
    }
}

template<int inst>
void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
    void (*my_malloc_handler)();
    void *result;

    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(0 == my_malloc_handler) {__THROW_BAD_ALLOC;}
        (*my_malloc_handler)();
        result = reallocate(p, n);
        if (result) return (result);
    }
}

// 第二级配置器
template<bool threads, int inst>
class __default_alloc_template{
private:
    static size_t  ROUND_UP(size_t bytes){
        return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
    }

private:
    union obj {
        union obj * free_list_link; // 指向下一个区块
        char client_data[1]; // 储存本块内存的首地址
    };

private:
    static obj * volatile free_list[__NFREELISTS];

    // 根据区块大小，决定使用第n号free-list。
    static size_t  FREELIST_INDEX(size_t bytes){
        return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
    }

    static void *refill(size_t n){
        int nobjs = 20;
        char * chunk = chunk_alloc(n, nobjs);
        obj * volatile * my_free_list;
        obj * result;
        obj * current_obj, * next_obj;
        int i;

        // 一个区块
        if(1 == nobjs) return chunk;

        my_free_list = free_list + FREELIST_INDEX(n);

        // chunk空间内简历free list
        result = (obj *)chunk;

        *my_free_list = next_obj = (obj *)(chunk + n);

        // free list各个节点串起来
        for(int i = 1; ; i++){
            current_obj = (obj *)((char *)next_obj + n);
            if (nobjs - 1 == i){
                current_obj -> free_list_link = 0;
                break;
            }else{
                current_obj -> free_list_link = next_obj;
            }
        }
    }

    static char *chunk_alloc(size_t size, int &nobjs) {
        char * result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free; // 内存池剩余空间

        if(bytes_left >= total_bytes) { // 充足的剩余空间
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if(bytes_left >= size) {// 不能完全满足，但是够一个区块
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return (result);
        } else {// 一个都无法提供
            size_t byte_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            // 从残余零头中获取价值
            if (bytes_left > 0) {
                // 内存池中还有一些零头
                obj *volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
                // 将内存池中的剩余内存编入free list
                ((obj *) start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj *) start_free;
            }
            // heap中获取
            start_free = (char *) malloc(byte_to_get);
            if(0 == start_free) {
                int i;
                obj * volatile * my_free_list, *p;
                for(i = size; i<= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(0 != p) {
                        *my_free_list = p->free_list_link;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return (chunk_alloc(size, nobjs));
                    }
                }
                end_free = 0;
                start_free = (char *)malloc_alloc::allocate(bytes_to_get);
            }
            heap_size += byte_to_get;
            end_free = start_free + byte_to_get;
            return (chunk_alloc(size, nobjs));
        }

    }




    static char *start_free;
    static char *end_free;
    static size_t heap_size;

public:
    static void * allocate(size_t n){
        obj * volatile * my_free_list;
        obj * result;

        // 大于128调用第一级配置器
        if (n > (size_t) __MAX_BYTES) {
            return (malloc_alloc::allocate(n));
        }

        // 寻找16个free lists中最合适的一个
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if(result == 0) {
            void *r = refill(ROUND_UP(n));
            return r;
        }

        *my_free_list = result -> free_list_link;
        return (result);
    }
    static void deallocate(void *p, size_t n){
        obj *q = (obj *)p;
        obj * volatile * my_free_list;

        if (n > (size_t) __MAX_BYTES) {
            malloc_alloc::deallocate(p, n);
            return;
        }
        my_free_list = free_list + FREELIST_INDEX(n);

        q -> free_list_link = *my_free_list;
        *my_free_list = q;
    }
    static void * reallocate(void *p, size_t old_sz, size_t new_sz);
};

template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;

template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst>
__default_alloc_template<threads, inst>::obj * volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] =
        {0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};

};



#endif //MYSTL_ALLOC_H
