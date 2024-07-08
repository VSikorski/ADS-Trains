#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphStuff.h"


//This function is similar to the one provided in the code for the 4th assignment and its purpose is to accept
//a name as input
char *inputName() {
  int strLen = 100;
  int length = 0;

  char character = getchar();
  char *name  = malloc(sizeof(char)*(strLen+1));

  while(character!='\n') {
    name[length] = character;
    length++;
    if(length >= strLen) {
      strLen = 2*strLen;
      name = realloc(name, (strLen+1)*sizeof(char));
    }
    character = getchar();
  }
  name[length] = '\0';
  return name;
}


//This function makes and returns a new graph node using
adjListNode newGraphNode (int id, int distance) {
  adjListNode new = malloc(sizeof(adjListElement));
  new->stationIDNumber = id;
  new->stationDistance = distance;
  new->next = NULL;
  new->distance = distance;
  return new;
}

//This function creates and empty graph with the number of nodes equal to the one inputed
graph makeGraph (int nr) {
  graph newGraph = malloc(sizeof(graph)*(nr+1));
  newGraph->vertices = nr;
  newGraph->graphArray = malloc(sizeof(adjListElement)*(nr+1));
  for(int i = 0; i < nr; i++) {
    newGraph->graphArray[i].head = NULL;
  }
  return newGraph;
}

//This function adds an edge to the graph's adjacency list using the data provided
void addEdge(graph elGrapho, adjListNode source, adjListNode destination, int distance) {
  adjListNode new1 = newGraphNode(destination->stationIDNumber, distance);
  new1->next = elGrapho->graphArray[source->stationIDNumber].head;
  elGrapho->graphArray[source->stationIDNumber].head = new1;

  //The graph is undirected so it must go from vertex A to B and from vertex B to A
  adjListNode new2 = newGraphNode(source->stationIDNumber, distance);
  new2->next = elGrapho->graphArray[destination->stationIDNumber].head;
  elGrapho->graphArray[destination->stationIDNumber].head = new2;
}



//This function removes and edge from the graph's adjacency list, ignoring the invalid deletions
void removeEdge (graph elGrapho, adjListNode toRemove1, adjListNode toRemove2, int *totalStations) {
  //Both the edge from A to B and from B to A must be removed, since the graph is undirected
  adjListNode currentNode = elGrapho->graphArray[toRemove1->stationIDNumber].head;
  adjListNode previousNode = NULL;
  //This loop looks for the other vertex incident with the edge to be removed
  while(currentNode !=NULL && currentNode->stationIDNumber!=toRemove2->stationIDNumber) {
    previousNode = currentNode;
    currentNode = currentNode->next;
  }
  

  //Upon finding it,if it's found, it's removed by freeing the memory
  if(currentNode!=NULL) {
    if(previousNode == NULL) {
      elGrapho->graphArray[toRemove1->stationIDNumber].head = currentNode->next;
    } else {
      previousNode->next = currentNode->next;
    }
    free(currentNode);
  }

  //The same procedure is applied for the second vertex of the node
  currentNode = elGrapho->graphArray[toRemove2->stationIDNumber].head;
  previousNode = NULL;
  while(currentNode !=NULL && currentNode->stationIDNumber!=toRemove1->stationIDNumber) {
    previousNode = currentNode;
    currentNode = currentNode->next;
  }

  if(currentNode != NULL) {
    if(previousNode == NULL) {
      elGrapho->graphArray[toRemove2->stationIDNumber].head = currentNode->next;
    } else {
      previousNode->next = currentNode->next;
    }
    free(currentNode);
  }

  return;
}

//This funtion looks into the list of nodes and returns the position of the station in the list
int findInList(adjListNode* stations, char* name, int stationsNr) {
  for(int i = 0; i<stationsNr; i++) {
    if(strcmp((*stations[i]).stationName, name)==0) {
      return i;
    }
  }
  return -1;
}

//This function frees the graph
void freeGraph(graph g) {
  for(int i = 0; i<g->vertices; i++) {
    adjListNode newNode = g->graphArray[i].head;
    adjListNode next;
    while(newNode!=NULL) {
      next = newNode->next;
      freeNode(newNode);
      newNode = next;
    }
  }
  free(g->graphArray);
  free(g);
}

//This funtcion frees a particular node
void freeNode(adjListNode node) {
  free(node);
}

//This function frees the adjacency list
void freeNodeList(adjListNode* list, int number) {
  for(int i=0; i< number; i++) {
    free(list[i]->stationName);
    freeNode(list[i]);
  }
  free(list);
}
