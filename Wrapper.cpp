#include <stdexcept>
#include <vector>
#include <fstream>
#include "ndarray.h"
#include "RunSim.h"

extern "C" { // Ensure name mangling doesn't affect function name

int run_sim_wrapper(double habitat_preference, double step_shape, double directional_bias, double roost_lambda,
         int environmentResolution, double startT, int startDoy, double solLat,
         double solLon, double observation_error, int upper_left_left, int upper_left_top, unsigned long int randomSeed,
         numpyArray<unsigned int> times_numpy, numpyArray<double> envMat,
         double* result_array, int result_size) {
    // std::ofstream //debug_file("//debug_output_wrapper.txt", std::ios::app); // Opens the file for appending
    try {
        run_sim(habitat_preference, step_shape, directional_bias, roost_lambda,
            environmentResolution, startT, startDoy, solLat,
            solLon, observation_error, upper_left_left, upper_left_top, randomSeed,
            times_numpy, envMat, result_array, result_size);
        //debug_file << "Simulation ran successfully." << std::endl;
        return 0;
    } catch (const std::out_of_range& e) {
        //debug_file << "Out of range error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        //debug_file << "Standard exception: " << e.what() << std::endl;
        return 2;
    } catch (...) {
        //debug_file << "Unknown exception occurred." << std::endl;
        return 3;
    }
}

}
