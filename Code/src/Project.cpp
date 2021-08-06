#include "../include/Project.h"

#define EARTH_RADIUS 6378137 // meters, using WGS 83 ellipsoid
#define EARTH_ECCENTRICITY 8.1819190842622e-2 // using WGS 83 ellipsoid
#define SPEED_OF_LIGHT 299792458 // meters per second

enum sat_id {sat1, sat2, sat3, sat4, sat5, sat6, sat7, sat8,
	sat9, sat10, sat11, sat12, sat13, sat14, sat15, sat16,
	sat17, sat18, sat19, sat20, sat21, sat22, sat23, sat24};

// Thread control booleans
std::atomic<bool> stopOrbits;
std::atomic<bool> stopThread;

// Calculate straight-line distance between two Structure objects
double calculate_distance(Structure struct1, Structure struct2) {
	double x_diff = struct1.get_x() - struct2.get_x();
	double y_diff = struct1.get_y() - struct2.get_y();
	double z_diff = struct1.get_z() - struct2.get_z();
	double sum = pow(x_diff, 2) + pow(y_diff, 2) + pow(z_diff, 2);
	double dist = sqrt(sum);
	return dist;
}

// Function for converting from LLA (Latitude, Longitude, Altitude)
// to ECEF (Earth Centered, Earth Fixed) coordinate systems
// Based on Matlab code at gist.github.com/klucar/1536056
void lla2ecef(double* coords)
{
	double esq = pow(EARTH_ECCENTRICITY, 2);
	double lat = coords[0]/180*M_PI; // Latitude, in radians
	double lng = coords[1]/180*M_PI; // Longitude, in radians
	double alt = coords[2]; // Altitude, in meters
	double N = EARTH_RADIUS/(sqrt(1 - esq*pow(sin(lat), 2)));
	coords[0] = ((N+alt)*cos(lat)*cos(lng))/1000; // x, in km
	coords[1] = ((N+alt)*cos(lat)*sin(lng))/1000; // y, in km
	coords[2] = ((N*(1-esq)+alt)*sin(lat))/1000; // z, in km
}

// Add edges between the 24 hypothetical satellites
void add_edges(Graph_Sat& constellation)
{
	// Add viable edges between satellites
	// Satellites can communicate with the one in front of it, behind it, above it, and below it
	for(unsigned int i = 0; i < 7; i ++)
	{
		// Horizontal links
		constellation.add_edge(i, i+1);
		constellation.add_edge(i+1, i);
		constellation.add_edge(i+8, i+9);
		constellation.add_edge(i+9, i+8);
		constellation.add_edge(i+16, i+17);
		constellation.add_edge(i+17, i+16);

		// Vertical links
		constellation.add_edge(i+8, i);
		constellation.add_edge(i, i+8);
		constellation.add_edge(i+8, i+16);
		constellation.add_edge(i+16, i+8);
	}

	// Final horizonal and vertical links (back to front, front to back)
	constellation.add_two_way_edge(sat1, sat8);
	constellation.add_two_way_edge(sat9, sat16);
	constellation.add_two_way_edge(sat17, sat24);
	constellation.add_two_way_edge(sat16, sat8);
	constellation.add_two_way_edge(sat16, sat24);
}


// Simulate small differences in the orbital pattern of the first num structures
// Adds a random value between +/-25 to add entropy to the system
// and show the distance recalculations
void add_noise(unsigned int num, Graph_Sat& constellation)
{
	for(unsigned int i = 0; i < num; ++i)
	{
		double new_x = constellation.get_satellite_x(i) + (rand()%50-25);
		double new_y = constellation.get_satellite_y(i) + (rand()%50-25);
		double new_z = constellation.get_satellite_z(i) + (rand()%50-25);
		constellation.set_satellite_xyz(i, new_x, new_y, new_z);
	}
}

void orbit(double t, Graph_Sat& constellation) {

	// Satellites at latitude 30 degrees, altitude of 500 km from sea level, spaced 45 degrees from each other
	for(unsigned int i = sat1; i <= sat8; ++i)
	{
		constellation.set_satellite_xyz(i, cos(t+M_PI*i/4) * 5961, sin(t+M_PI*i/4) * 5961, 3240);
	}

	// Satellites above the equator, altitude of 500 km from sea level, spaced 45 degrees from each other
	for(unsigned int i = sat9; i <= sat16; ++i)
	{
		constellation.set_satellite_xyz(i, cos(t+M_PI*i/4) * 6878, sin(t+M_PI*i/4) * 6878, 0);
	}

	// Satellites at latitude -30 degrees, altitude of 500 km from sea level, spaced 45 degrees from each other
	for(unsigned int i = sat17; i <= sat24; ++i)
	{
		constellation.set_satellite_xyz(i, cos(t+M_PI*i/4) * 5961, sin(t+M_PI*i/4) * 5961, -3240);
	}

	// Simulate small differences in orbital pattern by randomly moving the satellites
	add_noise(24, constellation);
}

void runOrbits(Graph_Sat& constellation) {
	double t = 0;
	while (!stopThread) {
		if(!(stopOrbits)) {
			usleep(100000);
			std::cout << "\033[2J\033[1;1H";
			std::cout << "Press Enter to access menu" << std::endl << std::endl;
			std::cout << constellation << std::endl;
			orbit(t, constellation);
			constellation.update_edges();
			t += 0.01;
		}
	}
}

int main() {
	Graph_Sat constellation;
	char c;

	// Add 24 Satellites to the constellation
	for(int i = 0; i < 24; ++i)
	{
		Satellite sat(0,0,0);
		constellation.add_vertex(sat);
	}

	// Initialize the constellation orbits at t=0
	orbit(0, constellation);

	// Add all intersatellite links to the constellation
	add_edges(constellation);

	// Create GroundStations
	GroundStation gs_1(313, -4759, 4221); // Notre Dame
	GroundStation gs_2(3978, -9, 4969); // London

	DynArr<GroundStation> stations(0);
	stations.push_back(gs_1);
	stations.push_back(gs_2);


	// Sets satellites in motion for one orbit and displays location data

	// Once paused
	bool exit = false;

	// Thread control booleans (declared globally)
	stopOrbits = false;
	stopThread = false;
	// Start the orbit thread
	std::thread oThread(runOrbits, std::ref(constellation));

	while (!exit) {
		int choice;

		// Wait until user presses enter
		fflush(stdin);
		std::cin.get(c);

		// Pause the orbit calculations while user is making selections
		stopOrbits = true;

		// Ensure that the orbits are done printing before showing the menu
		usleep(200000);
		std::cout << "Menu: " << std::endl;
		std::cout << "1: Continue Simulation" << std::endl;
		std::cout << "2: Add Ground Station" << std::endl;
		std::cout << "3: Determine smallest latency path between ground stations" << std::endl;
		std::cout << "4: Exit program" << std::endl;
		std::cout << "Your choice: ";
		std::cin >> choice;

		if (choice == 1) {
			// continue simulation
			stopOrbits = false;
			std::cin.ignore();
		}

		else if (choice == 2) {
			// Add Ground Station
			// Get LLA Coordinates
			double coords[3];
			std::cout << std::endl << "Enter the longitude, latitude, and elevation of the new ground station: " << std::endl << std::endl;
			std::cout << "Latitude: ";
			std::cin >> coords[0];
			std::cout << "Latitude: ";
			std::cin >> coords[1];
			std::cout << "Elevation (in meters): ";
			std::cin >> coords[2];

			// LLA coordinates to ECEF coordinates
			lla2ecef(coords);

			GroundStation new_gs(coords[0], coords[1], coords[2]);
			stations.push_back(new_gs);
			std::cout << "New groundstation added at ECEF coordinates (" << coords[0] << " km, " << coords[1] << " km, " << coords[2] << " km)"  << std::endl << std::endl;
		}

		else if (choice == 3) {
			// Determine smallest latency path between ground stations
			std::cout << std::endl;
			for (unsigned int i = 0; i < stations.length(); ++i) {
				std::cout << "(GS" << i + 1 << ")  ";
				std::cout << "x: " << stations[i].get_x() << " km  ";
				std::cout << "y: " << stations[i].get_y() << " km  ";
				std::cout << "z: " << stations[i].get_z() << " km  ";
				std::cout << std::endl;
			}

			unsigned int select_1, select_2;
			std::cout << std::endl << "What ground stations would you like to connect? ";
			std::cout << "Ground Station (origin): ";
			std::cin >> select_1;
			std::cout << "Ground Station (destination): ";
			std::cin >> select_2;
			std::cout << std::endl;

			// Find satellite that is the closest to each ground station
			double smallest_dist_1 = DBL_MAX;
			double smallest_dist_2 = DBL_MAX;
			unsigned int closest_sat_1;
			unsigned int closest_sat_2;
			for (unsigned int i = 0; i < constellation.get_vertices().length(); ++i) {
				double current_dist_1 = calculate_distance(constellation.get_vertex(i), stations[select_1 - 1]);
				double current_dist_2 = calculate_distance(constellation.get_vertex(i), stations[select_2 - 1]);
				if (current_dist_1 < smallest_dist_1) {
					smallest_dist_1 = current_dist_1;
					closest_sat_1 = i;
				}
				if (current_dist_2 < smallest_dist_2) {
					smallest_dist_2 = current_dist_2;
					closest_sat_2 = i;
				}
			}
			// Total distance is the distance from each ground station to its nearest
			// satellite, plus the distance from one satellite to the other.
			double total_dist = smallest_dist_1 + smallest_dist_2;

			// Run Dijkstra's Algorithm
			Stack<unsigned int> finalPath;
			total_dist += constellation.Dijkstra(closest_sat_1, closest_sat_2, finalPath);
			std::cout << "Shortest Path: ";
			std::cout << "(GS" << select_1 << ") --> ";

			unsigned int connections = finalPath.size();
			// Traverse the finalPath stack to output the shortest path
			for (unsigned int i = 0; i < connections; ++i) {
				std::cout << "Sat(" << finalPath.top() << ") --> ";
				finalPath.pop();
			}
			//std::cout << "Sat(" << finalPath.top() << ") --> ";
			std::cout << "(GS" << select_2 << ")";

			// Calculate Latency
			//double speed_of_light = 299792458; // meters per second
			total_dist *= 1000;  // convert from km to meters
			double latency = total_dist / SPEED_OF_LIGHT;

			// Print latency
			std::cout << std::fixed;
			std::cout << std::setprecision(5);
			std::cout << std::endl << "Latency time: " << latency << " seconds" << std::endl << std::endl;

		}

		else {
			exit = true;
		}
	}

	// Thread cleanup
	stopThread = true;
	oThread.join();
	std::cout << "\033[2J\033[1;1H";
	std::cout << "Successfully exited program" << std::endl;
	return 0;
}
