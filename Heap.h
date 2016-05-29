#ifndef __HEAP_H__
#define __HEAP_H__


#include <vector>
#include <cstdio>
using std::vector;


const int INC = 0; // INC for min-heap
const int DEC = 1; // DEC for max-Heap.


template <class V>
class Heap
{
public:
	typedef struct _Elmt {
		V value;
		int idx;
	} Elmt;

	struct ElmtComp {
		bool operator () (const Elmt& left, const Elmt& right) const { return left.value < right.value; }
	};


	/*returns the parent of a heap position*/
	//inline int parent(int posel) {
	//	if (posel%2) return posel/2;
	//	else return (posel-1)/2;
	//};

	inline int parent(int posel) {
		return (posel - 1) >> 1;
	}

	Heap(int sign = INC) {
		num_elems = 0;
		this->sign = sign;
	};

	~Heap() {
		vector<Elmt>().swap(heap);
		vector<int>().swap(order);
	};

	void clear() {
		heap.clear();
		order.clear();
		num_elems = 0;
	}

	bool is_exist(const int idx, V &v) {
		if (num_elems > 0 && idx < order.size() && order[idx] != -1) {
			v = heap[order[idx]].value;
			return true;
		} else
			return false;
	};

	bool is_exist(const int idx) {
		if (num_elems > 0 && idx < order.size() && order[idx] != -1) {
			return true;
		} else
			return false;
	};

	// old remove function with bug!!!

	//bool remove(int idx) {
	//	if (num_elems>0 && idx<order.size() && order[idx]!=-1) {
	//		int posel = order[idx];
	//   heap[posel].value = -1;
	//   moveup(posel);
	//
	//   V value;
	//   dequeue(idx, value);
	//
	//		return true;
	//	}
	//	return false;
	//};


	bool remove(int idx) {
		if (num_elems > 0 && idx < order.size() && order[idx] != -1) {
			int pos = order[idx];
			order[idx] = -1;
			if (num_elems - 1 > 0) {
				order[heap[num_elems - 1].idx] = pos;
				heap[pos].idx = heap[num_elems - 1].idx;
				//heap[pos].value = heap[endpos].value;
				num_elems--;
				update_queue(heap[pos].idx, heap[num_elems].value);
			}
			else num_elems--;
			return true;
		}
		return false;
	}

	void enqueue(int idx, V value) {
		//if (num_elems>0 && idx<order.size() && order[idx]!=-1) {
		//	updatequeue(idx, value);
		//	return;
		//}

		V tmp;
		int tmpidx,p,posel;

		posel = num_elems; //last position
		if (heap.size() < num_elems + 1)
			heap.push_back(Elmt());
		heap[num_elems].value = value;
		heap[num_elems++].idx = idx;
		if (idx >= order.size())
			order.insert(order.end(), idx-order.size() + 1, -1);
		order[idx] = posel;

		while (posel >0) {
			p = parent(posel);
			//if (value<heap[p].value) {
			if (compare(value, heap[p].value)) {
				/* swap element with its parent */
				tmp = heap[p].value;
				heap[p].value = value;
				heap[posel].value = tmp;

				tmpidx = heap[p].idx;
				heap[p].idx = idx;
				heap[posel].idx = tmpidx;

				/* adjust order */
				order[idx] = p;
				order[tmpidx] = posel;

				posel = parent(posel);
			}
			else break;
		}
	};

	int dequeue(int &idx) {
		if (num_elems == 0) /* empty queue */
			return 0;

		//value = heap[0].value;
		idx = heap[0].idx;
		//printf("dequeue %d", idx);
		order[idx] = -1; /* not in queue */
		if (num_elems - 1 > 0) {
			heap[0].value = heap[num_elems - 1].value;
			heap[0].idx = heap[num_elems - 1].idx;
			order[heap[0].idx] = 0;
			num_elems--;
			move_down(0);
		}
		else num_elems--;

		return 1;
	};

	int dequeue() {
		if (num_elems == 0) /* empty queue */
			return 0;

		//value = heap[0].value;
		int idx = heap[0].idx;
		//printf("dequeue %d ", idx);
		order[idx] = -1; /* not in queue */
		if (num_elems - 1 > 0) {
			heap[0].value = heap[num_elems - 1].value;
			heap[0].idx = heap[num_elems - 1].idx;
			order[heap[0].idx] = 0;
			num_elems--;
			move_down(0);
		}
		else num_elems--;

		return 1;
	};

	int dequeue(int &idx, V &value) {
		if (num_elems == 0) /* empty queue */
			return 0;

		value = heap[0].value;
		idx = heap[0].idx;
		order[idx] = -1; /* not in queue */
		if (num_elems - 1 > 0) {
			heap[0].value = heap[num_elems - 1].value;
			heap[0].idx = heap[num_elems - 1].idx;
			order[heap[0].idx] = 0;
			num_elems--;
			move_down(0);
		}
		else num_elems--;

		return 1;
	};

	void update_queue(int idx, V new_val) {
		int posel;

		posel = order[idx];

		if (new_val == heap[posel].value)
			return;

		//if (new_val<=heap[posel].value)
		if (!compare(heap[posel].value, new_val)) {
			heap[posel].value = new_val;
			move_up(posel);
		}
		else {
			heap[posel].value = new_val;
			move_down(posel);
		}
	};

	void move_down(int md_start) {
		V tmp;
		int tmpidx;
		int posel = md_start; //root
		int swap;
		/*while posel is not a leaf and heap[posel].value > any of childen*/
		while ((posel << 1) + 1 < num_elems) { // exists a left son
			if ((posel << 1) + 2 < num_elems) { // exists a right son
				//if (heap[posel*2+1].value<heap[posel*2+2].value) // find minimum son
				if (compare(heap[(posel << 1) + 1].value, heap[(posel << 1) + 2].value))
					swap = (posel << 1) + 1;
				else
					swap = (posel << 1) + 2;
			}
			else swap = (posel << 1) + 1;

			//if (heap[posel].value > heap[swap].value) { /*larger than smallest son*/
			if (compare(heap[swap].value, heap[posel].value)) {
				/*swap elements*/
				tmp = heap[swap].value;
				heap[swap].value = heap[posel].value;
				heap[posel].value = tmp;

				tmpidx = heap[swap].idx;
				heap[swap].idx = heap[posel].idx;
				heap[posel].idx = tmpidx;

				/*swap orders*/
				order[heap[swap].idx] = swap;
				order[heap[posel].idx] = posel;

				posel = swap;
			}
			else break;
		}
	};

	void move_up(int md_start=0) {
		V tmp;
		int tmpidx;
		int posel = md_start; //root
		int p;
		int idx = heap[posel].idx;

		/*while posel is not a root and heap[posel].value < parent */
		while (posel >0) {
			p = parent(posel);
			//if (heap[posel].value<heap[p].value) {
			if (compare(heap[posel].value,heap[p].value)) {
				/* swap element with its parent */
				tmp = heap[p].value;
				heap[p].value = heap[posel].value;
				heap[posel].value = tmp;

				tmpidx = heap[p].idx;
				heap[p].idx = heap[posel].idx;
				heap[posel].idx = tmpidx;

				/* adjust order */
				order[heap[p].idx] = p;
				order[heap[posel].idx] = posel;

				posel = p;
			}
			else break;
		}
	};

	void get_top(int &idx, V &value) {
		idx = heap[0].idx;
		value = heap[0].value;
	};

	void reset() { 
		heap.clear();
		order.clear();
		num_elems = 0;
	};

	void reset2() { 
		heap.clear();
		num_elems = 0;
	};

	V get_top_val() { return heap[0].value; };
	int get_top_idx() { return heap[0].idx; };
	V get_second_top_val() { 
		return heap[1].value < heap[2].value ? heap[1].value : heap[2].value;
	}

	int size() { return num_elems; }

	void print_heap() {
		for (int i = 0; i < num_elems; i++)
			printf("%f(%d, %d) ", heap[i].value, heap[i].idx, order[heap[i].idx]);
		printf("\n");
	};

//private:
	bool compare(V a, V b) { return (sign == INC)? (a < b) : (a > b); };

	vector<Elmt> heap;
	vector<int> order;
	int num_elems;
	int sign;
};

///
/// Heap - end

#endif
