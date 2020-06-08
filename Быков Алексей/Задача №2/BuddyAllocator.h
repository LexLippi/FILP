#pragma once
//#include <intrin.h>

/* Работоспособность программы можно проверить на первом задании 
 * подключив вместо allocator.h данную библиотеку
 */

struct segment {
	char* start;
	segment* buddy, * parent;
	bool used;
	segment() {
		start = nullptr;
		buddy = parent = nullptr;
		used = false;
	}

	segment(segment* _parent, size_t shift) {
		start = _parent->start + shift;
		parent = _parent;
		used = false;
		buddy = nullptr;
	}

	~segment() {
		if (parent == nullptr)
			delete[] start;
		parent = buddy = nullptr;
		start = nullptr;
	}

	segment(const segment& x) = delete;
	segment& operator=(const segment& x) = delete;
};

/* Extra structure for keeping segments */
struct LinkedList {
private:
	struct element {
		segment* val;
		element* prev, * next;
		element() {
			val = nullptr;
			prev = next = nullptr;
		}

		element(segment* value) {
			val = value;
			prev = next = nullptr;
		}

		~element() {
			next = nullptr;
			delete val;
			if (prev != nullptr)
				delete prev;
		}

		element(const element& x) = delete;
		element& operator=(const element& x) = delete;
	};
public:
	element* cur;
	LinkedList() {
		cur = nullptr;
	}

	~LinkedList() {
		if (cur != nullptr)
			delete cur;
	}

	LinkedList(const LinkedList& x) = delete;
	LinkedList& operator=(const LinkedList& x) {
		this->cur = x.cur;
		return *this;
	}

	LinkedList(segment* value) {
		cur = new element(value);
	}

	void push_back(segment* value) {
		if (cur != nullptr) {
			element* nv = new element(value);
			nv->prev = cur;
			cur->next = nv;
			cur = nv;
		}
		else
			cur = new element(value);
	}

	bool empty() {
		return cur == nullptr;
	}

	element* find(segment* value) {
		for (element* i = cur; i != nullptr; i = i->prev)
			if (i->val == value)
				return i;
		return nullptr;
	}

	element* findbymem(char* value) {
		for (element* i = cur; i != nullptr; i = i->prev)
			if (i->val->start == value)
				return i;
		return nullptr;
	}

	void erase(segment* value) {
		element* i = find(value);
		if (i == nullptr)
			return;
		element* nx = i->next;
		element* pr = i->prev;
		i->val = nullptr;
		if (pr != nullptr)
			pr->next = nx;
		if (nx != nullptr)
			nx->prev = pr;
		if (cur == i)
			cur = i->prev;
		i->prev = i->next = nullptr;
		delete i;

	}
};
/* End of structure */

/* Allocator starts */
const size_t mbyte = 1024 * 1024; // One Megabyte as constant for alloc
const size_t mxdegree = 21;

template<class T>
class MyAlloc {
private:
	/* "Added" from intrin.h */
	unsigned char bsr(unsigned long &index, size_t mask) {
		unsigned char res = 0;
		for (index = mxdegree; index >= 0; --index) {
			if (mask & (1 << index)) {
				res = 1;
				break;
			}
		}
		return res;
	}

	size_t getsize2(size_t msize) {
		unsigned long index;
		unsigned char non_zero = bsr(index, msize);
		if (non_zero && (1 << index) != msize)
			index++;
		return (non_zero ? index : -1);
	}
	
public:
	std::shared_ptr<LinkedList> mem_levels[mxdegree] = { nullptr };
	std::shared_ptr<LinkedList> used_mem[mxdegree] = { nullptr };
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using value_type = T;


	MyAlloc() {
		for (size_t i = 0; i < mxdegree; ++i) {
			if (mem_levels[i] == nullptr)
				mem_levels[i] = std::shared_ptr<LinkedList>(new LinkedList());
			if (used_mem[i] == nullptr)
				used_mem[i] = std::shared_ptr<LinkedList>(new LinkedList());
		}
	}

	template<class U>
	MyAlloc(MyAlloc<U>& other) {
		for (size_t i = 0; i < mxdegree; ++i) {
			mem_levels[i] = other.mem_levels[i];
			used_mem[i] = other.used_mem[i];
		}
	}

	MyAlloc& operator=(const MyAlloc& x) = default;

	~MyAlloc() = default;

	T* allocate(size_type n) {
		size_t index = getsize2(n * sizeof(T));
		for (size_t i = index; i < mxdegree; ++i) {
			if (!mem_levels[i]->empty()) {
				segment* current = mem_levels[i]->cur->val;
				mem_levels[i]->erase(current);
				for (size_t j = i; j != index; --j) {
					current->used = true;
					used_mem[j]->push_back(current);
					segment* left = new segment(current, 0);
					segment* right = new segment(current, 1 << (j - 1));
					left->buddy = right, right->buddy = left;
					mem_levels[j - 1]->push_back(right);
					current = left;
				}
				current->used = true;
				used_mem[index]->push_back(current);
				return reinterpret_cast<T*>(current->start);
			}
		}
		char* block = new char[1 << index];
		segment* root = new segment();
		root->start = block;
		root->used = true;
		used_mem[index]->push_back(root);
		return reinterpret_cast<T*>(block);
	}

	void deallocate(T* p, size_type n) {
		size_t index = getsize2(n * sizeof(T));
		segment* dealloc = used_mem[index]->findbymem(reinterpret_cast<char*>(p))->val;
		used_mem[index]->erase(dealloc);
		dealloc->used = false;
		if (dealloc->buddy != nullptr && !dealloc->buddy->used) {
			segment* par = dealloc->parent;
			mem_levels[index]->erase(dealloc->buddy);
			delete dealloc->buddy;
			delete dealloc;
			used_mem[index + 1]->erase(par);
			par->used = false;
			if (par->parent != nullptr)
				mem_levels[index + 1]->push_back(par);
			else
				delete par;
		}
		else if (dealloc->buddy != nullptr)
			mem_levels[index]->push_back(dealloc);
		else
			delete dealloc;
	}

	void destroy(T* _Ptr) {
		((T*)_Ptr)->~T();
	}

	size_type max_size() const {
		return mbyte / sizeof(value_type);
	}
};

/* Allocator ends */
