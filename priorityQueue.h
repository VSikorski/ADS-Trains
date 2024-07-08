#include "graphStuff.h"


typedef struct heapElem *heapNode;
typedef struct heapElem {
  int dist;
  int v; 
  adjListElement* node; 
}heapElem;

typedef struct heapStructure* heap;
typedef struct heapStructure{
  int size;
  int capacity;
  int* pos;
  int front;
  heapNode* array;
}heapStructure;

heapNode newHeapNode(int distance, int v, adjListNode node);
heap makeHeap(int capacity);
void swapHeapNodes(heapNode *a, heapNode *b);
void ipHeap(heap h, int id);
int isEpmtyHeap(heap h);
heapNode getMinimum(heap h);
void decreaseDistance(heap h, int vertex, int distance);
int isInHeap(heap h, int v);
void dijkstraShortestPath(graph g, int source, int destination, int*reachable, adjListNode* stations);
void printString(char *str);
