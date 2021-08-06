#ifndef GRAPH_SAT_H
#define GRAPH_SAT_H

#include "Vertex.h"
#include "DynArr.h"
#include "Stack.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>

class Graph_Sat {

private:

	DynArr<Vertex<Satellite>> vertices; // Adjacency calculate_distance

public:

	/*** Basic Graph Features ***/

	// Constructor
	Graph_Sat() : vertices() {}


	void add_vertex(Satellite vertexData) {
		Vertex<Satellite> theVertex(vertexData);
		vertices.push_back(theVertex);
	}

	Satellite get_vertex(unsigned int vertex) {
		return vertices[vertex].get_vertex_value();
	}

	// Get dynamic array of vertices
	DynArr< Vertex<Satellite> > get_vertices() {
		return vertices;
	}


	/*** Edge Manipulation ***/

	// Add Edge from Origin to Destination, with weight
	void add_edge(unsigned int origin, unsigned int destin, double weight) {
		if (origin < vertices.length() && destin < vertices.length()) {
			vertices[origin].add_edge(destin, weight);
		}
	}

	// Add edge without weight (calculate distance using the distance formula)
	void add_edge(unsigned int origin, unsigned int destin)
	{
		if (origin < vertices.length() && destin < vertices.length())
		{
			double x_diff = vertices[origin].get_vertex_value().get_x() - vertices[destin].get_vertex_value().get_x();
			double y_diff = vertices[origin].get_vertex_value().get_y() - vertices[destin].get_vertex_value().get_y();
			double z_diff = vertices[origin].get_vertex_value().get_z() - vertices[destin].get_vertex_value().get_z();
			double sum = pow(x_diff, 2) + pow(y_diff, 2) + pow(z_diff, 2);
			double weight = sqrt(sum);
			add_edge(origin, destin, weight);
		}
	}
	
	// Add edges in both directions between origin and destin
	void add_two_way_edge(unsigned int origin, unsigned int destin)
	{
		add_edge(origin, destin);
		add_edge(destin, origin);
	}

	// Set the value of an edge with a specific weight
	void set_edge_value(unsigned int origin, unsigned int destin, double weight) {
		if (origin < vertices.length() && destin < vertices.length()) {
			vertices[origin].set_edge_value(destin, weight);
		}
	}

	// Set edge value without weight (calculate distance between satellites
	void set_edge_value(unsigned int origin, unsigned int destin)
	{
		if(origin < vertices.length() && destin < vertices.length())
		{
			double x_diff = vertices[origin].get_vertex_value().get_x() - vertices[destin].get_vertex_value().get_x();
			double y_diff = vertices[origin].get_vertex_value().get_y() - vertices[destin].get_vertex_value().get_y();
			double z_diff = vertices[origin].get_vertex_value().get_z() - vertices[destin].get_vertex_value().get_z();
			double sum = pow(x_diff, 2) + pow(y_diff, 2) + pow(z_diff, 2);
			double weight = sqrt(sum);
			set_edge_value(origin, destin, weight);
		}
	}

	/*** Additional Satellite Getters ***/

	// Return x value of certain vertex
	double get_satellite_x(unsigned int vertex) {
		double x;
		if (vertex < vertices.length()) {
			x = vertices[vertex].get_vertex_value().get_x();
			return x;
		}
		// We will prevent this return from ever happening
		return -1;
	}

	// Return y value of certain vertex
	double get_satellite_y(unsigned int vertex) {
		double y;
		if (vertex < vertices.length()) {
			y = vertices[vertex].get_vertex_value().get_y();
			return y;
		}
		// We will prevent this return from ever happening
		return -1;
	}

	// Return x value of certain vertex
	double get_satellite_z(unsigned int vertex) {
		double z;
		if (vertex < vertices.length()) {
			z = vertices[vertex].get_vertex_value().get_z();
			return z;
		}
		// We will prevent this return from ever happening
		return -1;
	}

	/*** Additional Satellite-Specific Setters ***/

	// Set x value of certain vertex
	void set_satellite_x(unsigned int vertex, double x_in) {
		if (vertex < vertices.length()) {
			vertices[vertex].get_vertex_value().set_x(x_in);
		}
	}

	// Set y value of certain vertex
	void set_satellite_y(unsigned int vertex, double y_in) {
		if (vertex < vertices.length()) {
			vertices[vertex].get_vertex_value().set_y(y_in);
		}
	}

	// Set y value of certain vertex
	void set_satellite_z(unsigned int vertex, double z_in) {
		if (vertex < vertices.length()) {
			vertices[vertex].get_vertex_value().set_z(z_in);
		}
	}

	// Set xyz values of certain vertex
	void set_satellite_xyz(unsigned int vertex, double x_in, double y_in, double z_in) {
		if (vertex < vertices.length()) {
			vertices[vertex].get_vertex_value().set_coords(x_in, y_in, z_in);
		}
	}

	/*** Graph Utility Functions ***/

	// Update each edge in the graph based on their current distances
	void update_edges()
	{
		for(unsigned int i = 0; i < vertices.length(); i ++)
		{
			for(unsigned int j = 0; j < vertices[i].num_edges(); j ++)
			{
				set_edge_value(i, vertices[i].get_edge_destin(j));
			}
		}
	}

	// Dijkstra's Algorithm for calulating the shortest path between a specified origin and destination node
	// If it finds a valid path, returns the distance (through a return statement) and the path to get there (through a Stack passed by reference)
	double Dijkstra( unsigned int origin, unsigned int destin, Stack<unsigned int> &finalPath ){
		if( origin >= vertices.length() || destin >= vertices.length() || vertices.length() == 0 ){

			std::cout << "Invalid Inputs" << std::endl;
			return -1;

		}

		/* Initialize the Elements */
		Stack< unsigned int > theStack;
		DynArr< int > parents( vertices.length() );
		DynArr< double > distance( vertices.length() );

		bool found = false;

		/* Initialize the origin */
		theStack.push( origin );
		distance[origin] = 0;
		parents[origin] = -1;

		if( destin == origin ){
			found = true;
		}

		if( !found ){
			/* Initialize all the distances after the origin */
			for( unsigned int iter = 0; iter < vertices.length(); iter++ ){
				if(iter != origin)
				{
					// Make it the largest possible int
					distance[ iter ] = 2147483647;
					// Set the parent to -1
					parents[ iter ] = -1;
				}
			}

			/* Run the shortest path algorithm */
			while( !theStack.empty() ){
				// Get the top element of the stack and pop
				unsigned int index = theStack.top();
				theStack.pop();

				// Evaluate the edges from the vertex
				for(unsigned int iter = 0; iter < vertices[ index ].num_edges(); iter++ ){
					// Obtain the edge
					Edge tempEdge = vertices[ index ].get_edge( iter );

					// If the weight of the edge plus distance of the  distance is less than the destin weight
					if( distance[ index ] + tempEdge.weight < distance[ tempEdge.destin ] ) {
						// Update the distance
						distance[ tempEdge.destin ] = distance[ index ] + tempEdge.weight;

						// Update the parent of the destin
						parents[ tempEdge.destin ] = (int)index;

						// Check if destin is the result;
						if( tempEdge.destin == destin && !found ){

							found = true;
						}
						theStack.push( tempEdge.destin );
					}
				}
			}
		}

		// Otherwise, go through the parents until we find the origin
		if( found ){
			unsigned int sentinel = destin;
			finalPath.push( sentinel );		// Push the desination onto the stack

			while( parents[sentinel] != -1 ){
				finalPath.push( (unsigned int)parents[sentinel] );	// Push the parent onto the stack
				sentinel = (unsigned int)parents[sentinel];			// Update the sentinel
			}

			// Stack contains the correct order
			/*
				std::cout << "The valid Dijkstra path from " << origin << " to " << destin << " is: ";
						while( !finalPath.empty() ){

							std::cout << finalPath.top() << " ";
							finalPath.pop();
						}
						std::cout << ", and the distance is " << distance[destin] << std::endl;
						std::cout << std::endl;
				*/

			// The path stack is returned through the pass by ref
			return distance[destin];
		}
		return -1;
	}

	// Overloaded operator
	friend std::ostream& operator<<(std::ostream& output, const Graph_Sat& theGraph) {
		for (unsigned int i = 0; i < theGraph.vertices.length(); ++i) {
			if (i < 10) {
				output << "Sat (" << i << ")  : " << theGraph.vertices[i] << std::endl;
			}
			else {
				output << "Sat (" << i << ") : " << theGraph.vertices[i] << std::endl;
			}
		}
		return output;
	}
};

#endif
