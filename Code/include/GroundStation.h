#ifndef GROUNDSTATION_H
#define GROUNDSTATION_H

class GroundStation : public Structure {

public:

	// Default Constructor
	GroundStation() : Structure(0, 0, 0) {}

	// Constructor
	GroundStation(double x_in, double y_in, double z_in) : Structure(x_in, y_in, z_in) {}

	// Destructor
	~GroundStation() {}

};

#endif
