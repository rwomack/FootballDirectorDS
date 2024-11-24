//
// Sparse Array Template Class
//
template <class DataT> class ArrayOb {
public :
	long index;		// index of array element
	DataT info;		// information
};

// Linked list array object for sparse arrays.
template <class DataT>
class SparseOb: public ArrayOb<DataT> {
public:
	SparseOb<DataT> *next;		// Pointer to next object
	SparseOb<DataT> *prior;		// Pointer to previous object

	SparseOb() {
		info = 0;
		index = -1;
		next = NULL;
		prior = NULL;
	}
};

// A generic doubly linked list class for sparse array
template <class DataT>
class SparseList : public SparseOb<DataT> {
	SparseOb<DataT> *start, *end;
public:
	SparseList() { start = end = NULL; }
	~SparseList();

	void store(long ix, DataT c); // store element
	void remove(long ix); // delete element

	// return pointer to element given index
	SparseOb<DataT> *find(long ix);
};

// SparseList destructor.
template <class DataT> SparseList<DataT>::~SparseList()
{
	SparseOb<DataT> *p, *p1;

	// free all elements in the list
	p = start;
	while(p) {
		p1 = p->next;
		delete p;
		p = p1;
	}
}

// Add the next element.
template <class DataT>
void SparseList<DataT>::store(long ix, DataT c)
{
	SparseOb<DataT> *p;
	p = FD_ENGINE_NEW SparseOb<DataT>;
	if(!p) {
		//cout << "Allocation Error.\n";
		exit(1);
	}

	p->info = c;
	p->index = ix;

	if (start==NULL) { // First element in list
		end = start = p;
	}
	else { // put on end
		p->prior = end;
		end->next = p;
		end = p;
	}
}


template <class DataT>
void SparseList<DataT>::remove(long ix)
{
	SparseOb<DataT> *ob;

	ob = find(ix);		// get pointer to element
	if(!ob) return;		// array element does not exist

	if (ob->prior) {		// not deleting first element
		ob->prior->next = ob->next;
		if (ob->next)		// not deleting last element
			ob->next->prior = ob->prior;
		else				// otherwise, deleting last element
			end = ob->prior;	// update end pointer
	}
	else {		// deleting first element
		if (ob->next) {
			ob->next->prior = NULL;
			start = ob->next;
		}
		else // list now empty
			start = end = NULL;
	}

	delete ob;		// Added by Nick
}


// Find an array element given its index.
template <class DataT>
SparseOb<DataT> * SparseList<DataT>::find(long ix)
{
	SparseOb<DataT> *temp;

	temp = start;

	while(temp) {
		if (ix==temp->index) return temp;	// found
		temp = temp->next;
	}
	return NULL; // not in list
}


// A generic sparse array class.
template <class DataT>
class SparseArray : public ArrayOb<DataT> {
	long length;	// dimension of array
	ArrayOb<DataT> *primary;
	SparseList<DataT> *chains;
	int hash(long ix);	// hashing function
public:
	SparseArray(void) {}
	SparseArray(long size);
	void Create(long size);		// Added by Nick - does the job of the original constructor
	~SparseArray();				// Added by Nick
	DataT &operator[](long i);
};

// Constructor for SparseArray using hashing.
template <class DataT>
SparseArray<DataT>::SparseArray(long size)
{
	// Revised the original constructor to call what now does its job
	Create(size);
}

template <class DataT>
void SparseArray<DataT>::Create(long size)
{
	long i;

	length = size;

	// dynamically allocate storage for primary array
	primary = FD_ENGINE_NEW ArrayOb<DataT>[hash(size) + 1];
	if (!primary) {
		//cout << "Cannot allocate primary array.\n";
		exit(1);
	}

	// initialize primary array to zero
	for (i=0; i < (hash(size) + 1); i++) {
		primary[i].index = -1;
		primary[i].info = 0;
	}

	// allocate hash chain object
	chains = FD_ENGINE_NEW SparseList<DataT>[hash(size) + 1];
}

// Destructor added by Nick
template <class DataT>
SparseArray<DataT>::~SparseArray()
{
	delete[] chains;	// Destroy CSparseList objects
	delete[] primary;	// Destroy CArrayOb objects
}

// Index into the sparse array
template <class DataT>
DataT &SparseArray<DataT>::operator[](long ix)
{
	if (ix<0 || ix>length - 1) {
		//cout << "\nIndex value of ";
		//cout << ix << " is out of bounds.\n";
		exit(1);
	}

	// first, see if index in primary array
	if (ix == primary[hash(ix)].index)
		return primary[hash(ix)].info;

	// if not, add element to primary array
	if (primary[hash(ix)].index == -1) {
		primary[hash(ix)].index = ix;		// put in primary array
		return primary[hash(ix)].info;		// return pointer to it
	}

	// otherwise, see if in hash chain
	SparseOb<DataT> *o;

	o = chains[hash(ix)].find(ix);		//	get pointer to element
	if (!o) {
		chains[hash(ix)].store(ix, 0);
		o = chains[hash(ix)].find(ix);	// get pointer to new element
	}

	return o->info;
}

// define hash function here.
template <class DataT>
int SparseArray<DataT>::hash(long ix)
{
	return ix / 10;
}
