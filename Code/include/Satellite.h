#ifndef SATELLITE_H
#define SATELLITE_H

class Satellite : public Structure {

public:
	
	// Default Constructor
	Satellite() : Structure(0, 0, 0) {};

	// Parameterized Constructor
	Satellite(double x_in, double y_in, double z_in) : Structure(x_in, y_in, z_in) {}
	
	// Destructor
	~Satellite() {}

};

#endif
