#include "heap.h"
#include "support.h"

int minore(int a, int b) {
	return (a < b ? 1 : 0);
}
int maggiore(int a, int b) {
	return (a > b ? 1 : 0);
}

int cmpstr(void *a, void *b) {
	if(a == NULL || b == NULL) {
		return -1;
	}
	return strcmp((char *)a, (char *)b);
}
int cmpint(void *a, void *b) {
	int na, nb;
	na = 0;
	nb = 0;
	if(a == NULL || b == NULL) {
		return -1;
	}
	na = *(int *)a;
	nb = *(int *)b;
	if(na == nb) {
		return 0;
	} else if(na > nb) {
		return 1;
	} else {
		return -1;
	}
}
int cmpdouble(void *a, void *b) {
	double na, nb;
	na = 0.0;
	nb = 0.0;
	if(a == NULL || b == NULL) {
		return -1;
	}
	na = *(double *)a;
	nb = *(double *)b;
	if(na == nb) {
		return 0;
	} else if(na > nb) {
		return 1;
	} else {
		return -1;
	}
}

Pair_t *getArrayElem(Heap_t *heap, int indice) {
	if(indice >= 0 && indice < heap->heapsize) {
		return ((Pair_t **)(heap->content))[indice];
	} else {
		return (Pair_t *) NULL;
	}
}
Pair_t *getTreeElem(Heap_t *heap, int indice) {
	int i, j, heapsize, mask, primo1;
	Tree_t *cur;
	i=0;
	j=0;
	primo1 = 0;
	heapsize = indice+1;
	if(heapsize == 1)
		return (((Tree_t *)heap->content) != NULL) ? ((Tree_t *)heap->content)->val : (Pair_t*) NULL;
	else if(heapsize < 1)
		return (Pair_t *) NULL;
	mask = pow(2, (int)log2(heapsize));
	cur = ((Tree_t *)heap->content);
	while(j<log2((double)(mask))+1 && cur != NULL){
		i=(mask>>j)&(heapsize);
		if(i>0){
			if(primo1 == 0) primo1 = 1;
			else /*1*/		cur = cur->dx;
		}else	 /*0*/		cur = cur->sx;
		
		j++;
	}
	if(cur != NULL)
		return (Pair_t *) cur->val;
	else
		return (Pair_t *) NULL;
}
Tree_t *getTreeNode(Heap_t *heap, int indice) {
	int i, j, heapsize, mask, primo1;
	Tree_t *cur;
	i=0;
	j=0;
	primo1 = 0;
	heapsize = indice+1;
	if(heapsize == 1)
		return (((Tree_t*)heap->content) != NULL) ? ((Tree_t *)heap->content) : (Tree_t *) NULL;
	else if(heapsize < 1)
		return (Tree_t *) NULL;
	mask = pow(2, (int)log2(heapsize));
	cur = ((Tree_t*)heap->content);
	while(j<log2((double)(mask))+1 && cur != NULL){
		i=(mask>>j)&(heapsize);
		if(i>0){
			if(primo1 == 0) {
				primo1 = 1;
			} else {
				//1
				cur = cur->dx;
			}
		}else {
			//0
			cur = cur->sx;
		}
		j++;
	}
	if(cur != NULL)
		return (Tree_t *) cur;
	else
		return (Tree_t*) NULL;
}


Tree_t *mallocTreeNode(Tree_t *nodo) {
	nodo = (Tree_t *) malloc(sizeof(Tree_t));
	return nodo;
}
Tree_t *instree(Tree_t *testa, int indice, Pair_t *el) {
	if(indice == 0) {
		testa = mallocTreeNode(testa);
		testa->val = el;
		testa->sx = NULL;
		testa->dx = NULL;
		return testa;
	} else {
		Tree_t *cur;
		int i, j, heapsize, mask, primo1;
		cur = testa;
		i=0;
		j=0;
		primo1 = 0;
		heapsize = indice+1;
		mask = pow(2, (int)log2(heapsize));
		while(j<log2((double)(mask))+1){
		i=(mask>>j)&(heapsize);
		if(i>0){
			if(primo1 == 0) {
				primo1 = 1;
			} else {
				//1
				if(cur->dx == NULL) {
					cur->dx = instree(cur, 0, el);
					break;
				} else {
					cur = cur->dx;
				}
			}
		}else {
				//0
				if(cur->sx == NULL) {
					cur->sx = instree(cur, 0, el);
					break;
				} else {
					cur = cur->sx;
				}
			}
		j++;
		}
	}
	printf("\n");
	return testa;
}

//Display heap
void _displayQueue(Pair_t *el, Heap_t *heap) {
	if(el) {
		//printf("\t");
		heap->output(el->content);
		//printf("\t%lf\n",el->priority);
	} else {
		//printf(" NULL \n");
	}
}
void _displayTree(Tree_t *testa, Heap_t *funz,int i) {
	int	index=i;
	if(testa != NULL) {
		//printf("%d",index);
		_displayQueue(testa->val, funz);
		index++;
		_displayTree(testa->sx, funz,index);
		_displayTree(testa->dx, funz,index);
	}
}
void  displayHeap(Heap_t *heap) {
	int i;
	if(heap->heapsize < 1) {
		//printf("\tQueue does not contains any item. Please insert some item before use any options. Thanks\n");
	} else {
		//printf("Index\tItem\tPriority\n");
		if(heap->arrayortree == 1) {
			i = 0;		
			while(i<heap->heapsize) {
				printf("%d ", i);
				_displayQueue(((Pair_t **)(heap->content))[i], heap);
				i++;
			}
		} else {
			_displayTree((Tree_t *)heap->content, heap,0);
		
		
		}
	}
}

//Free memory
void freePair(Pair_t *tmp) {
	if(!tmp)
		return;
	if(tmp->content) {
		free(tmp->content);
	}
	if(tmp)
		free(tmp);
}
void freeHeap(Heap_t *heap) {
	Pair_t *tmp;
	if(!heap)
		return;
	while(heap->heapsize > 0) {
		tmp = extractHeapItem(heap);
		heap->free(tmp);
	}
	if(heap->content)
		free(heap->content);
	heap->content = NULL;
	if(heap)
		free(heap);
}

//Init heap
Heap_t* mallocHeap(Heap_t *heap) {
	heap = malloc(sizeof(Heap_t));
	return heap;
}
Heap_t* reallocHeapArray(Heap_t* heap) {
	Pair_t **arr;
	if((heap->heapsize+1) >= heap->real_allocated) {
		heap->real_allocated = (heap->heapsize+1)*2;
		arr = ((Pair_t **)(heap->content));
		arr = realloc(arr, sizeof(Pair_t*) * (heap->real_allocated));
		heap->content = arr;
	}
	return heap;
}
int 	infoHeap(Heap_t *heap) {
	int minormax, arrayortree, tipo, dimensione;
	system("clear");
	printf(RED_COLOR_TEXT"LABORATORIO DI ALGORITMI E STRUTTURE DATI. Code a priorità.\n"RESET_COLOR_TEXT);	
	printf(RED_COLOR_TEXT"\t\tMENU'\n"RESET_COLOR_TEXT);
	
	printf("Generate:\n1. MIN HEAP\n2. MAX HEAP\n");		
	printf("Choice: ");
	readInt_utility(&minormax);

	printf("\nChoose HEAP implementation:\n1. Array\n2. Tree\n");
	printf("Choice: ");
	readInt_utility(&arrayortree);
	
	printf("\nChoose the type of HEAP's element:\n1. Alphanumeric\n2. Integer\n3. Decimal\n");
	printf("Choice: ");
	readInt_utility(&tipo);
	
	printf("\nInsert the number of HEAP's element: ");
	readInt_utility(&dimensione);
	
	setHeap(heap, tipo, arrayortree, minormax);
	
	return dimensione;
}
Heap_t* initHeapArray(Heap_t* heap, int dimensione) {
	int i;
	Pair_t** arr;
	arr = malloc(sizeof(Pair_t**) * (dimensione*2));
	heap->heapsize = dimensione;
	heap->real_allocated = dimensione*2;
	i = 0;
	//printf("\nInserting items (in Array)..\n");
	while(i<dimensione) {
		arr[i] = (Pair_t *) malloc(sizeof(Pair_t));
			//printf("\nItem %d -> ",i+1);
		arr[i]->content = heap->input();
			//printf("Priority item %d -> ",i+1);
			double prior;
			readDouble_utility(&prior);			
		arr[i]->priority = prior;
		i++;
	}
	heap->content = arr;
	
	//printf("Item correctly inserted!\n");
	return heap;
}
Heap_t* initHeapTree(Heap_t *heap, int dimensione) {
	int i;
	Pair_t *el;
	Tree_t *al;
	al = NULL;
	heap->content = al;
	heap->heapsize = dimensione;
	i = 0;
	//printf("\nInserting items (in Tree)..\n");
	while(i<dimensione) {
		el = malloc(sizeof(Pair_t));
			//printf("\nItem %d -> ",i+1);
		el->content = heap->input();
			//printf("Priority item %d -> ",i+1);
			double prior;
			readDouble_utility(&prior);		
		el->priority = prior;
		heap->content = instree(((Tree_t *)heap->content), i, el);
		i++;
	}
	_displayTree(((Tree_t *)heap->content), heap,0);
	return heap;
}
Heap_t* initHeap(Heap_t* heap, int dimensione) {
	if(heap->arrayortree == 1) {
		return initHeapArray(heap, dimensione);
	} else {
		return initHeapTree(heap, dimensione);
	}
	return heap;
}


int heapgetnumfromel(Heap_t *ogg, void *el) {
	int i;
	Pair_t *tmp;
	i = 0;
	while(i<ogg->heapsize) {
		tmp = ogg->getElem(ogg, i);
		if(tmp) {
			if(ogg->compare(tmp->content,el) == 0)
			return i;
		}
		i++;
	}
	return -1;
}


//Heap functions
void swap(Pair_t *a, Pair_t *b) {
	int tmp1;
	void *tmp2;
	tmp1 = a->priority;
	a->priority = b->priority;
	b->priority = tmp1;
	tmp2 = a->content;
	a->content = b->content;
	b->content = tmp2;
}
void Heapify(Heap_t *heap, int i) {
	int l, r, m;
	Pair_t *left, *right, *current, *max;
	l = (2*i)+1;
	r = l+1;
	m = i;
	left = heap->getElem(heap, l);
	right = heap->getElem(heap, r);
	current = heap->getElem(heap, i);
	if(left != NULL && l < heap->heapsize && heap->checkminormax(left->priority,current->priority)==1) {
		m = l;
	} else {
		m = i;
	}
	max = heap->getElem(heap, m);
	if(right != NULL && r < heap->heapsize && heap->checkminormax(right->priority,max->priority)==1) {
		m = r;
	}
	if(m != i) {
		swap(current, heap->getElem(heap, m));
		/* 1 2 3 4 5 -> 5 4 3 1 2 */
		Heapify(heap, m);
	}
}
void buildHeap(Heap_t *heap) {
	int i;
	i = heap->heapsize / 2;
	while(i>=0) {
		Heapify(heap, i);
		i--;
	}
}

Pair_t *getTopHeapItem(Heap_t *heap) {
	if(heap->heapsize < 1) {
		LOG_ERR("HeapSize < 1");
		return (Pair_t*) NULL;
	} else {
		return heap->getElem(heap, 0);
	}
}
Pair_t *extractHeapItem(Heap_t *heap) {
	if(heap->heapsize < 1) {
		LOG_ERR("HeapSize < 1");
		return (Pair_t*) NULL;
	} else {
		Pair_t *top, *last, *primo;
		primo = heap->getElem(heap, 0);
		last = heap->getElem(heap, heap->heapsize-1);
		top = malloc(sizeof(Pair_t));
		top->content = primo->content;
		top->priority = (int) (primo->priority);
		heap->getElem(heap, 0)->content = last->content;
		heap->getElem(heap, 0)->priority = last->priority;
		//delete last 
		if(heap->arrayortree == 2) {
			Tree_t *padre;
			int zind;
			int pad;
			zind = (heap->heapsize);
			pad = zind/2;
			pad--;
			padre = getTreeNode(heap, pad);
			if(padre != NULL) {
				if(padre->dx != NULL) {
					if(padre->dx->val == last) {
						free(padre->dx);
						padre->dx = NULL;
					}
				}
				if(padre->sx != NULL) {
					if(padre->sx->val == last) {
						free(padre->sx);
						padre->sx = NULL;
					}
				}
			}
		}
		//end delete last
		free(last);
		heap->heapsize = heap->heapsize - 1;
		Heapify(heap, 0);
		return top;
	}
}
void deleteKey(Heap_t *heap, int indice) {
	if(heap->heapsize < 1) {
		LOG_ERR("HeapSize < 1");
	} else if(indice >= heap->heapsize || indice < 0) {
		//printf("Index incorrect or invalid. Try again.\n");
	} else {
		int padre;
		Pair_t *el, *padre_el, *last;
		el = heap->getElem(heap, indice);

		last = heap->getElem(heap, heap->heapsize-1);
		swap(heap->getElem(heap, indice), heap->getElem(heap, heap->heapsize-1));
		heap->heapsize = heap->heapsize-1;
		padre = (int)((indice+1)/2)-1;
		padre_el = heap->getElem(heap, padre);

		if(heap->arrayortree == 2) {
			Tree_t *padre_nodo;
			padre_nodo = getTreeNode(heap, (int)((heap->heapsize+1)/2)-1);
			if(padre_nodo != NULL) {
				if(padre_nodo->dx != NULL) {
					if(padre_nodo->dx->val == last) {
						padre_nodo->dx = NULL;
					}
				}
				if(padre_nodo->sx != NULL) {
					if(padre_nodo->sx->val == last) {
						padre_nodo->sx = NULL;
					}
				}
			}
		}
		el = NULL;
		if(indice < ( heap->heapsize / 2)) { 
			if(indice == 0 || (padre_el != NULL && padre_el->priority < el->priority)) {
				Heapify(heap, indice);
			} else {
				while(indice > 0 && heap->getElem(heap, (int)((indice+1)/2)-1)->priority < heap->getElem(heap, indice)->priority) {
					swap(heap->getElem(heap, indice), heap->getElem(heap, (int)((indice+1)/2)-1));
					indice = (int)((indice+1)/2)-1;
				}
			}
		}
	el = last;
	heap->free(el);
	//printf("Item correctly deleted.\n");
	}

}
void decreasePriorityItem(Heap_t *heap, int indice, double priorita) {

	Pair_t *p;
	p = heap->getElem(heap, indice);
	if(p == NULL) {
		//printf("Index incorrect or invalid. Try again.\n");
	} else if(priorita > p->priority) {
		//printf("The new priority is higher than the old one. (Less priority) \n");
	} else {
		p->priority = priorita;
		if(heap->minormax == 2) { // down
			Heapify(heap, indice);	
		} else { // up
			while(indice > 0 && heap->getElem(heap, (int)((indice+1)/2)-1)->priority > heap->getElem(heap, indice)->priority) {
				swap(heap->getElem(heap, indice), heap->getElem(heap, (int)((indice+1)/2)-1));
				indice = (int)((indice+1)/2)-1;
			}
		}
		//printf("Priority correctly decreased.\n");
	}
}
void increasePriorityItem(Heap_t *heap, int indice, double priorita) {
	Pair_t *p;
	p = heap->getElem(heap, indice);
	if(p == NULL) {
		//printf("Index incorrect or invalid. Try again.\n");
	} else if(priorita < p->priority) {
		//printf("The new priority is less than the old one. (Higher priority) \n");
	} else {
		p->priority = priorita;
		if(heap->minormax == 2) { //MAXHEAP
			while(indice > 0 && heap->getElem(heap, (int)((indice+1)/2)-1)->priority < heap->getElem(heap, indice)->priority) {
				swap(heap->getElem(heap, indice), heap->getElem(heap, (int)((indice+1)/2)-1));
				indice = (int)((indice+1)/2)-1;
			}
		} else { //MINHEAP
			Heapify(heap, indice);
		}
		//printf("Priority correctly increased.\n");
	}
	
}
void insertKey(Heap_t *heap, void *el, double priorita) {
	Pair_t *elem;
	heap->heapsize = heap->heapsize+1;
	elem = malloc(sizeof(Pair_t));
	elem->content = el;
	elem->priority = priorita;
	if(heap->arrayortree == 1) {
		reallocHeapArray(heap); 
		((Pair_t **)(heap->content))[heap->heapsize-1] = elem;
	} else {
		heap->content = instree(((Tree_t *)heap->content), heap->heapsize-1, elem);
	}
	int indice = heap->heapsize-1;
	while(indice > 0 && heap->checkminormax(priorita,heap->getElem(heap, (int)((indice+1)/2)-1)->priority) == 1) {
		swap(heap->getElem(heap, indice), heap->getElem(heap, (int)((indice+1)/2)-1));
		indice = (int)((indice+1)/2)-1;
	}
	//printf("Item correctly inserted.\n");
}



void setHeap(Heap_t *rec, int type, int arrayortree, int minormax) {
	rec->tipo = type;
	rec->arrayortree = arrayortree;
	rec->minormax = minormax;
	
	switch(minormax) {
			default:
			case 1:
				rec->checkminormax = &minore;
				break;
			case 2:
				rec->checkminormax = &maggiore;
				break;
	}
	switch(arrayortree) {
			default:
			case 1: //ARRAY
				rec->getElem = &getArrayElem;
				break;
			case 2: //ALBERO
				rec->getElem = &getTreeElem;
				break;
	}
	rec->free = &freePair;
	switch(type) {
			default:
			case 1: //STRING
				rec->output = &printStr;
				rec->input = &readStr;
				rec->compare = &cmpstr;
				break;
			case 2: //INT
				rec->output = &printInt;
				rec->input = &readInt;
				rec->compare = &cmpint;
				break;
			case 3: //DOUBLE
				rec->output = &printDouble;
				rec->input = &readDouble;
				rec->compare = &cmpdouble;
				break;
			
	}
}
