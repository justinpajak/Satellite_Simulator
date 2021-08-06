#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <iostream>
#include <iomanip>

class Structure {

private:

	// Coordinates of the structure
	// Relative to the center of the Earth
	double x;
	double y;
	double z;

public:

	// Constructor
	Structure(double x_in, double y_in, double z_in) : x(x_in), y(y_in), z(z_in) {}

	// Virtual Destructor
	virtual ~Structure() {}

	/*** Getters ***/
	
	// Get x value
	double get_x() const {
		return x;
	}

	// Get y value
	double get_y() const {
		return y;
	}
	
	// Get z value
	double get_z() const {
		return z;
	}

	/*** Setters ***/
	
	// Set x value
	void set_x(double x_in) {
		this -> x = x_in;
	}

	// Set y value
	void set_y(double y_in) {
		this -> y = y_in;
	}

	// Set z value
	void set_z(double z_in) {
		this -> z = z_in;
	}

	// Set xyz values
	void set_coords(double x_in, double y_in, double z_in) {
		this -> x = x_in;
		this -> y = y_in;
		this -> z = z_in;
	}

	// Overloaded << operator
	friend std::ostream& operator<<(std::ostream& output, const Structure& theStructure){
		output << std::fixed;
		output << std::setprecision(1);
		output << "[x: " << theStructure.get_x() << ",\t" << " y: " << theStructure.get_y() << ",\t" << "z: " << theStructure.get_z() << "]";
		return output;
	}
};

#endif
