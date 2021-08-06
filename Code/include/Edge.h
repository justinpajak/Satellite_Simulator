#ifndef EDGE_H
#define EDGE_H

struct Edge{

	unsigned int destin;	// Location of Edge Destination
	double weight;				// Edge Weight. Can be negative.

	// Default Constructor
	Edge() : destin(), weight( ) {}

	// Overloaded Constructor
	Edge( unsigned int dest, double weightIn ) : destin(dest), weight(weightIn) {}

};

#endif
