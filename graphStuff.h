//This is the structure used to define a graph node
typedef struct adjListElement* adjListNode;
typedef struct adjListElement {
  int stationIDNumber;
  int stationDistance;
  char* stationName;
  int distance;
  adjListNode next;
}adjListElement;


//This is the structure used to define an elemnt of the adjacency list
typedef struct adjList{
  adjListNode head;
}adjList;

//This is a structure used to define a graph
typedef struct graphStructure *graph;
typedef struct graphStructure{
  int vertices; //number of vertices in the graph
  adjList* graphArray;
}graphStructure;

char *inputName();
adjListNode newGraphNode (int id, int distance);
graph makeGraph (int nr);
void addEdge (graph elGrapho, adjListNode source, adjListNode destination, int distance);
void removeEdge (graph elGrapho, adjListNode toRemove1, adjListNode toRemove2, int* totalStations);
int findInList(adjListNode* stations, char* name, int stationsNr);
void printGraph(graph g, int vertices);
void freeGraph(graph g);
void freeNode(adjListNode node);
void freeNodeList(adjListNode* list, int number);
