#ifndef PROJECT_H
#define PROJECT_H

// Project dependencies
#include "../include/Structure.h"
#include "../include/Satellite.h"
#include "../include/GroundStation.h"
#include "../include/Graph_Sat.h"
#include "../include/Stack.h"
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <float.h>
#include <thread>
#include <atomic>

// Helper functions for Project
void add_edges(Graph_Sat& constellation);
void add_noise(unsigned int num, Graph_Sat& constellation);
void orbit(double t, Graph_Sat& constellation);

#endif
