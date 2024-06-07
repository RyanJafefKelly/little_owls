

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <math.h>
#include <boost/math/distributions/gamma.hpp>

#include "Model.h"
#include "Environment.h"
#include "RandomGenerator.h"
#include "Solar.h"
#include "ndarray.h"

using namespace std::chrono;

#include "RunSim.h"

RunSim::RunSim() {
    // TODO: boiler
}

RunSim::~RunSim() {
    // TODO: boiler
}

extern "C" {
void run_sim(double habitat_preference, double step_shape, double directional_bias, double roost_lambda,
         int environmentResolution, double startT, int startDoy, double solLat,
         double solLon, double observation_error, int upper_left_left, int upper_left_top, unsigned long int randomSeed,
         numpyArray<unsigned int> times_numpy, numpyArray<double> envMat,
         double* result_array, int result_size) {

    Ndarray<double,2> environment(envMat);

    Ndarray<unsigned int,1> times_nd(times_numpy);
    int len_times = times_nd.getShape(0);
    std::vector<unsigned int> times;
    for (int i = 0; i < len_times; i++) {
        // times.at(i) <- times_nd[i];
        times.push_back(times_nd[i]);
    }

	RandomGenerator ranGen;

	std::vector<double> param;

	unsigned int startX = 3133;  // TODO? change from manual
	unsigned int startY = 2589;  // TODO? ange from manual

	param.push_back(startX);
	param.push_back(startY);
	param.push_back(habitat_preference); // habitat preference INCLUDE
	param.push_back(step_shape); // stepShape  INCLUDE
	param.push_back(2); // stepScale
	param.push_back(directional_bias); // directional bias INCLUDE
	param.push_back(1.5); // dispersal resting time logMean
	param.push_back(0.7); // dispersal resting time logSd
	param.push_back(300); // TODO!  maximum effort 6000/20 --  copied Facade.R
	param.push_back(roost_lambda); // roost lambda INCLUDE
//	param.push_back(4); // recoveryRate
//	param.push_back(-5); // beta1
//	param.push_back(5); // beta2
    double new_obs_err = observation_error / environmentResolution;
	param.push_back(new_obs_err); // TODO: facade observationError
	param.push_back(10); // TODO: 200 (rsc range) / 20 (envres) rsc range

    int nr = 5829, nc = 6489;
    std::vector<std::vector<double> > landscape_data(nr, std::vector<double> (nc));
    std::vector<std::vector<double> >* landscape = &landscape_data;

    auto start = high_resolution_clock::now();
    int numpy_element_size = sizeof(environment[0][0]);

    // TODO: NEW APPROACH READ FROM MEMORY
    for(int r = 0; r < nr; r++){
        Ndarray<double,1> env_row = environment[r];

        std::vector<double> landscape_row(nc);
        memcpy(&landscape_row[0], &env_row[0], nc*numpy_element_size);

        landscape_data.at(r) = landscape_row;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

	ranGen.seedrand(randomSeed);

	Model mod(landscape);
	int maxDuration = -999;

	Individual* indOut = mod.runSimulation(param, 2000, maxDuration, ranGen, times,
			startT, startDoy, solLat, solLon, environmentResolution);

    int batch_size = indOut->xObs.size();

    for(int m = 0; m < result_size; m++) {
        result_array[m] = -1;
    }

    for(int i = 0; i < times.size(); i++) {  //todo? check last
        double tstamp = times.at(i);
        double x_obs = indOut->xObs.at(i)*environmentResolution + upper_left_left;
        double y_obs = -indOut->yObs.at(i)*environmentResolution + upper_left_top;
        double step_obs = indOut->stepDistanceObs.at(i)*environmentResolution;
        double turn_angle_obs = indOut->turningAngleObs.at(i);
        double env_val_obs = indOut->envValObs.at(i);
        double rsc = indOut->rsc.at(i);

        result_array[i*7] = tstamp;
        result_array[i*7+1] = x_obs;
        result_array[i*7+2] = y_obs;
        result_array[i*7+3] = step_obs;
        result_array[i*7+4] = turn_angle_obs;  // TODO! do <2 thing?
        result_array[i*7+5] = env_val_obs;
        result_array[i*7+6] = rsc;
    }
}
}