/*
 * Max Heap 
 * This heap can deal with the key with the same frequency
 */
#pragma once

#include <vector>
#include "config.h"

class max_heap
{
    private:
        std::vector<heap_node> heap_v;
    public:
        max_heap();
        ~max_heap();

        void insertKey(heap_node k);
        heap_node extractMax();
        void heapArrayDisply();                //print out the heap

        void maxHeapify_BottToRoot(int i);     //max Heapify from bottom to the root
        void maxHeapify(int i);                //heapify a subtree with the root at given index 

        int parent(int i) { return (i-1)/2; }  //get index of parent
        int left(int i) { return (2*i + 1); }  //get index of left child
        int right(int i) { return (2*i + 2); } //get index of right child
        int size() { return heap_v.size(); }
        heap_node getMax() { return heap_v[0]; }
};

