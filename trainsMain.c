#include <stdio.h>
#include <stdlib.h>
#include "priorityQueue.h"
#include <string.h>

int main(int argc, char** argv) {
  //First we read the number of networks to expect from the input
  int networks;
  scanf("%d", &networks);
  while(networks) {
    networks--;
    //Now we start reading the input for each network
    int stationsNr;
    char bufferCleaner;
    scanf("%d", &stationsNr);
    //This list will hold all the stations and the data relevant to them
    adjListNode* stations = malloc(sizeof(adjListElement)*(stationsNr));
    //Here we make the graph used to apply Dijkstra's algorithm
    graph stationsMap = makeGraph(stationsNr);
    int cStationsNr = stationsNr;
    while(cStationsNr){
      cStationsNr--;
      //This accepts the input for each station
      int idNum;
      scanf("%d ", &idNum);
      char* staionNameInput = inputName();
      stations[idNum] = newGraphNode(idNum, 0);
      stations[idNum]->stationName = malloc((strlen(staionNameInput)+1)*sizeof(char));
      strcpy(stations[idNum]->stationName, staionNameInput);
      free(staionNameInput);
    }
    //This section handles the connections
    int connections;
    scanf("%d\n", &connections);
    int i = 0;
    while(i<connections) {
      i++;
      int place1, place2, distance;
      //each connection is added as an edge
      scanf("%d %d %d", &place1, &place2, &distance);
      addEdge(stationsMap, stations[place1], stations[place2], distance);
    }
    //This section handles the disruptions
    int disruptions;
    scanf("%d", &disruptions);
    //The buffer cleaner is here to handle the appearence of a NULL character
      bufferCleaner = getchar();
      if(bufferCleaner != '\n') {
        printf("detected issue");
      }

    
    char* toremove;
    while(disruptions>0) {
      //This accepts a name as an input and removes the edge from the graph, it will ignore incorrect disruptions
      toremove = inputName();
      int stationToRemove1 = findInList(stations, toremove, stationsNr);
      free(toremove);
      toremove = inputName();
      int stationToRemove2 = findInList(stations, toremove, stationsNr);
      removeEdge(stationsMap, stations[stationToRemove1], stations[stationToRemove2], &stationsNr);
      disruptions--;
      free(toremove);
    }
    //This section handles the queries
    int queryID1, queryID2;
    char* queryInput = inputName();
    while(queryInput[0]!= '!') {
      //The queries consist of two names and the path and distance between them or UNREACHABLE if there is no path between them
      int reached = 0;
      queryID1 = findInList(stations, queryInput, stationsNr);
      free(queryInput);
      queryInput = inputName();
      queryID2 = findInList(stations, queryInput, stationsNr);
      dijkstraShortestPath(stationsMap, queryID1, queryID2, &reached, stations);
      if(reached==0) {
        printf("UNREACHABLE\n");
      }
      free(queryInput);
      queryInput = inputName();
    }
    //Now we free all the data structures we used
    free(queryInput);
    freeGraph(stationsMap);
    freeNodeList(stations, stationsNr);
  }
}
