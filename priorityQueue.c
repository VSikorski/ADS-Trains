#include "priorityQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//This function frees the memory allocated to a heap node
void freeHeapNode(heapNode node) {
  free(node);
}

//This function frees the memory allocated to a heap
void freeHeap(heap h, int source) {

  free(h->array);
  free(h->pos);
  free(h);
}

//This structure is used to remember the path discovered in the Dijkstra's algorithm
typedef struct pathHolder{
  int* IDs;
  int distance;
}pathHolder;


//This fucntion creates and allocates memory to a new node for the heap
heapNode newHeapNode(int distance, int v, adjListNode node) {
  heapNode new = malloc(sizeof(heapElem));
  new->dist = distance;
  new->v = v;
  new->node = node;
  return new;
}


//This function creates and allocates memory for a heap
heap makeHeap(int capacity) {
  heap new = malloc(sizeof(heapStructure));
  new->pos = malloc(capacity*sizeof(int));
  new->size = 0;
  new->capacity = capacity;
  new->front  = 0;
  new->array = malloc(capacity*sizeof(heapNode));
  for(int i = 0; i < capacity; i++) {
    new->array[i] = NULL;
  }
  return new;
}

//Thsi fucntion swaps two nodes of a heap
void swapHeapNodes(heapNode* a, heapNode* b) {
  heapNode temp = *a;
  *a = *b;
  *b = temp;
  return;
}

//This function fixes the heap from the given id in order for it to be a minimum Heap
//It also updates the position of nodes when they are swapped
void upHeap(heap h, int id) {
  int smallest, leftChild, rightChild;
  smallest = id;
  leftChild = id*2+1;
  rightChild = id*2+2;
  
  if(leftChild < h->size && h->array[leftChild]->dist < h->array[smallest]->dist) {
    smallest = leftChild;
  }

  if(rightChild < h->size && h->array[rightChild]->dist < h->array[smallest]->dist) {
    smallest = rightChild;
  }

  if(smallest != id) {
    heapNode smallestNode = h->array[smallest];
    heapNode idNode = h->array[id];
    h->pos[smallestNode->v] = id;
    h->pos[idNode->v] = smallest;


    swapHeapNodes(&h->array[smallest], &h->array[id]);

    upHeap(h, smallest);
  }
  return;
}


//This function returns the minimum of a heap and removes it from said structure
heapNode getMinimum(heap h) {
  if(isEpmtyHeap(h)) {
    return NULL;
  }

  heapNode root = h->array[0];
  heapNode lastNode = h->array[h->size - 1];
  h->array[0] = lastNode;

  h->pos[root->v] = h->size - 1;
  h->pos[lastNode->v] = 0;

  
  h->size = h->size-1;
  
  upHeap(h, 0);

  return root;
}




//This fucntion modifies the distance stored at vertex v/the priority stored at vertex v
void decreaseDistance(heap h, int vertex, int distance) {
  int i = h->pos[vertex];
  h->array[i]->dist = distance;

  //Travels down the heap until it is correct
  while(i && h->array[i]->dist < h->array[(i-1)/2]->dist) {
    h->pos[h->array[i]->v] = (i-1)/2;
    h->pos[h->array[(i-1)/2]->v] = i;
    swapHeapNodes(&h->array[i], &h->array[(i-1)/2]);
    i = (i-1)/2;
  }
}

//This checks whether the heap is empty
int isEpmtyHeap(heap h) {
  return h->size == 0;
}


//This checks whether an element is still in the heap
int isInHeap(heap h, int v) {
  if(h->pos[v] < h->size) {
    return 1;
  }

  return 0;
}


//This fucntion is used to print a string
void printString(char *str) {
  for(int i = 0; str[i] != '\0'; i++) {
    printf("%c", str[i]);
  }
  printf("\n");
}




//This is the implementation of Dijkstra's algorithm using and adjacency list and a min-priority queue represented by a heap
void dijkstraShortestPath(graph g, int source, int destination, int *reacheable, adjListNode* stations) {
  pathHolder* p = malloc(g->vertices*sizeof(pathHolder));
  int vertices = g->vertices;
  int* prev = malloc((vertices+1)*sizeof(int));
  //This si the empty priority queue
  heap S = makeHeap(vertices);
  
  //This loop declares the array which will be used to print the path later
  for(int i = 0; i < vertices; i++) {
    p[i].IDs = malloc((vertices+1)*sizeof(int));
  }

  //This is the initialization of the dist[source] = 0 and the other source-related values
  //newHeapNode is going to be used as the add_with_priority() function
  p[source].distance = 0;
  prev[source] = -1;
  S->array[source] = newHeapNode(p[source].distance, source, g->graphArray[source].head);
  S->pos[source] = source;
  
  //This initiates the other values different from the source
  //intead of INFINITY, the maximum value that can be held by an integer variable is used
  for(int i = 0; i < vertices; i++) {
    if(i != source) {
      p[i].distance = INT_MAX;
      prev[i] = -1;
      S->array[i] = newHeapNode(p[i].distance, i, g->graphArray[i].head);
      S->pos[i] = i;
    }
   
  }
  

  decreaseDistance(S, source, p[source].distance);
  S->size = vertices;
  //Here starts the loop to find the path
  //while S is not empty
  while(!isEpmtyHeap(S)) {
    //u <- S. extract_min()
    heapNode minimumHeapNode = getMinimum(S);
    int u = minimumHeapNode->v;
    //This is used to go through the incident nodes
    adjListNode crawler = g->graphArray[u].head;
    while(crawler != NULL) {
      //v is the node adjacent to the minimum node/u
      int v = crawler->stationIDNumber;
      //this holds the current distance from the source
      int alt = p[u].distance + crawler->distance;
      if(isInHeap(S, v) && p[u].distance!=INT_MAX && alt< p[v].distance) {
        *reacheable = 1;
        //This modifies the distance from the source to the node v
        p[v].distance = alt;
        prev[v] = u;
        //this reduces priority/ S.decrease_priority(v, alt), distance is the equivalent to priority
        decreaseDistance(S, v, alt);
      }
      crawler = crawler->next;
      
    }
    //After using the node, we free it to avoid memeory leaks
    free(minimumHeapNode);
  }
  
  //Here the printing is done, if the point is reacheable
  int nr =0;
  if(*reacheable!=0) {
    if(p[destination].distance == INT_MAX) {
      *reacheable = 0;
      return;
    }
    
    int j = destination;
    
    while(j!=-1) {
      p[destination].IDs[nr] = j;
      nr++;
      j = prev[j];
    }
    for(int z = nr-1; z>=0;z--) {
      printf("%s\n", stations[p[destination].IDs[z]]->stationName);
      
    }
    printf("%d\n", p[destination].distance);
  }

  //Now we free the used structures
  free(prev);
  for(int i = 0; i < vertices; i++) {
    free(p[i].IDs);
  }
  free(p);
  freeHeap(S, source);
  
  return;
}
