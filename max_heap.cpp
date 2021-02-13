#include "max_heap.h"
#include <iostream>
#include <new>


max_heap::max_heap() 
{
}
max_heap::~max_heap()
{
}

/*
 *max Heapify from bottom to the root
 */
void max_heap::maxHeapify_BottToRoot(int i)
{
	int parentp = parent(i);
	int child = i;
	std::string key;
    unsigned int freq;

	//keep compare with it's parent and change them until the parent is larger
	while(parentp >= 0)
	{
		if(heap_v[parentp].freq < heap_v[child].freq)  
		{
			key = heap_v[parentp].key;
            freq = heap_v[parentp].freq;
			heap_v[parentp].key = heap_v[child].key;
            heap_v[parentp].freq = heap_v[child].freq;
			heap_v[child].key = key;
            heap_v[child].freq = freq;

			child = parentp;
			parentp = parent(child);
		}
		else  //the parent is larger than the child, so stop here
		{
			return;
		}
	}
}

/*
 *print out the heap
 */
void max_heap::heapArrayDisply()
{
	for(unsigned int i=0; i<heap_v.size(); i++)
	{
		std::cout << "heap_Arr[" << i << "]: " << heap_v[i].key << " freq: " << heap_v[i].freq << std::endl;
	}
}

/*
 *insert key into heap
 */
void max_heap::insertKey(heap_node k) 
{
	heap_v.push_back(k);   //Insert the new element at the end of the heap
	maxHeapify_BottToRoot(heap_v.size()-1);  //heapify start from the bottom
}

/*
 *extract the word information which has max frequence
 */
heap_node max_heap::extractMax() 
{ 
    heap_node res("", 0);
	if(heap_v.size() == 0)  //Heap is empty
	{
		std::cout << "Heap is empty!" << std::endl;
		return res;  //Suppose all the elements are positive number
	}
	res.key = heap_v[0].key; //The root
    res.freq = heap_v[0].freq;
	heap_v[0].key = heap_v[heap_v.size()-1].key;  //Put the last element to the root
    heap_v[0].freq = heap_v[heap_v.size()-1].freq;
    heap_v.pop_back();
	maxHeapify(0);  //Heapify from the root
	return res; 
} 

/*
 *max Heapify from root to the bottom
 */
void max_heap::maxHeapify(int i) 
{ 
	unsigned int leftChild = left(i);
	unsigned int rightChild = right(i);
	int largest = -1;
	if(leftChild < heap_v.size() && heap_v[leftChild].freq > heap_v[i].freq)
	{
		largest = leftChild;
	}
	else
	{
		largest = i;
	}
	if(rightChild < heap_v.size() && heap_v[rightChild].freq > heap_v[largest].freq)
	{
		largest = rightChild;
	}
	if(largest != i)
	{
		heap_node temp(heap_v[i].key, heap_v[i].freq);
		heap_v[i].key = heap_v[largest].key;
        heap_v[i].freq = heap_v[largest].freq;
		heap_v[largest].key = temp.key;
        heap_v[largest].freq = temp.freq;
	}
	else
	{
		return;
	}
	maxHeapify(largest);
} 
