#include <iostream>
#include "Heap.h"

using namespace std;

int main() {
	Heap<double> heap;
	heap.enqueue(1, 2.1);
	heap.enqueue(2, -1.4);
	heap.enqueue(3, 2.54);
	heap.enqueue(4, 1.1);

	heap.print_heap();
	return 0;
}