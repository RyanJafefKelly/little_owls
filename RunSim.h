/*
 * RunSim.h
 *
 *  Created on: 10 May 2022
 *      Author: RK
 */

#ifndef RUNSIM_H_INCLUDED__
#define RUNSIM_H_INCLUDED__
#include "ndarray.h"

class RunSim {
public:
    RunSim();
    virtual ~RunSim();

};

extern "C" void run_sim(double habitat_preference, double step_shape, double directional_bias, double roost_lambda,
                           int environmentResolution, double startT, int startDoy, double solLat,
                           double solLon, double observation_error, int upper_left_left, int upper_left_top, unsigned long int randomSeed,
                           numpyArray<unsigned int> times_numpy, numpyArray<double> envMat,
                           double* result_array, int result_size);

#endif
