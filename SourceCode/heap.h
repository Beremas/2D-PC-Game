typedef struct heap_ Heap_t;
typedef struct pair_ Pair_t;
typedef struct tree_ Tree_t;


struct heap_ {
	void 	*content;
	int 	heapsize;
	int 	real_allocated;
	void 	(*output)(void*);
	void* 	(*input)(void);
	int 	(*compare)(void*,void*);
	Pair_t* (*getElem)(Heap_t*,int);
	int 	(*checkminormax)(int,int);
	void 	(*free)(Pair_t *);
	
	int tipo;
	int arrayortree;
	int minormax;
};
struct tree_ {
	Pair_t *val;
	Tree_t * sx;
	Tree_t * dx;
};
struct pair_ {
	void *content;
	double priority;
};


int minore(int a, int b);
int maggiore(int a, int b);
int cmpstr(void *a, void *b);
int cmpint(void *a, void *b);
int cmpdouble(void *a, void *b);
Pair_t *getArrayElem(Heap_t *heap, int indice);
Pair_t *getTreeElem(Heap_t *heap, int indice);
Tree_t *getTreeNode(Heap_t *heap, int indice);
Tree_t *mallocTreeNode(Tree_t *nodo);
Tree_t *instree(Tree_t *testa, int indice, Pair_t *el);
void _displayQueue(Pair_t *el, Heap_t *heap);
void _displayTree(Tree_t *testa, Heap_t *funz,int i);
void  displayHeap(Heap_t *heap);
void freePair(Pair_t *tmp);
void freeHeap(Heap_t *heap);
Heap_t* mallocHeap(Heap_t *heap);
Heap_t* reallocHeapArray(Heap_t* heap);
int 	infoHeap(Heap_t *heap);
Heap_t* initHeapArray(Heap_t* heap, int dimensione);
Heap_t* initHeapTree(Heap_t *heap, int dimensione);
Heap_t* initHeap(Heap_t* heap, int dimensione);
int heapgetnumfromel(Heap_t *ogg, void *el);
void swap(Pair_t *a, Pair_t *b);
void Heapify(Heap_t *heap, int i);
void buildHeap(Heap_t *heap);
Pair_t *getTopHeapItem(Heap_t *heap) ;
Pair_t *extractHeapItem(Heap_t *heap);
void deleteKey(Heap_t *heap, int indice) ;
void decreasePriorityItem(Heap_t *heap, int indice, double priorita) ;
void increasePriorityItem(Heap_t *heap, int indice, double priorita) ;
void insertKey(Heap_t *heap, void *el, double priorita);

void setHeap(Heap_t *rec, int type, int arrayortree, int minormax);
