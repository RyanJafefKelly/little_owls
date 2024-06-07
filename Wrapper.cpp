// Include necessary headers
#include <stdexcept>
#include <vector>
#include "ndarray.h"
#include "RunSim.h"

extern "C" { // Ensure name mangling doesn't affect function name

int run_sim_wrapper(double habitat_preference, double step_shape, double directional_bias, double roost_lambda,
         int environmentResolution, double startT, int startDoy, double solLat,
         double solLon, double observation_error, int upper_left_left, int upper_left_top, unsigned long int randomSeed,
         numpyArray<unsigned int> times_numpy, numpyArray<double> envMat,
         double* result_array, int result_size) {
    try {
  	    run_sim(habitat_preference, step_shape, directional_bias, roost_lambda,
         environmentResolution, startT, startDoy, solLat,
         solLon, observation_error, upper_left_left, upper_left_top, randomSeed,
         times_numpy, envMat, result_array, result_size);
        return 0;
    } catch (const std::out_of_range& e) {
        // Handle the specific exception
        // return nullptr; // Indicate failure due to std::out_of_range
        return 1;
    } catch (...) {
        // Catch any other exceptions
        // return nullptr; // Indicate failure due to an unspecified exception
        return 2;
    }
}

}

