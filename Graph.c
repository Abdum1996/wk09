// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

#define TRUE 1
#define FALSE 0
#define NOWHERE -1

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex destination, int max, int *path)
{
	assert(g != NULL);
	
	
	// edge case early exit
	if (src == destination) { 
		path[0] = src;
		return 1;
	} 
	
	// ** Step 1. find shortest path ** // 
	
	// set up predecessors array 
	// and distances array
	Vertex pred[g->nV]; // predecessors array
	int dist[g->nV]; // total distance "hops" array
	int i;
	for (i = 0; i < g->nV; i++) {
		pred[i] = NOWHERE;
		dist[i] = 0;
	}
	
	// set up Queue
	Queue q = newQueue();
	QueueJoin(q, src);
	
	
	Vertex vCur; // current vertex
	while (!QueueIsEmpty(q) && ((vCur = QueueLeave(q)) != destination)) {
		
		for (i = 0; i < g->nV; i++) {
			
			if (g->edges[vCur][i] < max && pred[i] == NOWHERE) {
				// the predecessor of the vertex found is this vertex
				pred[i] = vCur;
				// the distance to this vertex is the
				// distance to vCur (the predecessor) + 1
				dist[i] = dist[vCur] + 1;
				QueueJoin(q, i);
				
			}
		}
	}
	
	
	// ** Step 2. build path ** //
	
	// populating path
	int pathLength = 0;
	if (dist[destination] > 0) { // there exists a non-empty path
		
		pathLength = dist[destination] + 1;
		path[pathLength - 1] = destination; // last index is destination
		
		for (i = pathLength - 2; i >= 0; i--) {
			// each step is the predecessor of the one after it
			path[i] = pred[path[i + 1]];
		}
	}
	
	return pathLength;
}