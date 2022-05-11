//============================================================================
// Name        : ST2.cpp
// Author      : SH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <math.h>
#include <boost/math/distributions/gamma.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/vector.hpp>

// #include "tinytiffreader.h"
// extern "C" {
//     #include <tiffio.h>
// }


// #include <tiffio.h>
#include "Model.h"
#include "Environment.h"
#include "RandomGenerator.h"
#include "Solar.h"
#include "ndarray.h"

using namespace std::chrono;

// define pi which was dropped in C99
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif


// function to print matrix as is
void displayMatrix(std::vector<std::vector<unsigned int> > dArray, unsigned int xSize,
		unsigned int ySize){

	std::cout << "The Matrix looks like this:\n" << std::endl;

//	std::vector<double>* pArray = dArray;
	unsigned int subscript = 1; // running subscript
	for(unsigned int y = 0; y < ySize; y++){
		for(unsigned int x = 0; x < xSize; x++, subscript++){
			std::cout << dArray[x][y] << " ";
			if(subscript % xSize == 0){
				std::cout << std::endl;
			}
		}
	}
	std::cout << std::endl;
}

// equivalent to dnorm() in R
double gaussianPdf(double x, double mean = 0.0, double sd = 1.0, bool Log = true){
	double pOut;
	pOut = 1 / (sd * sqrt(2 * M_PI)) * exp(-pow(x - mean, 2) / (2 * pow(sd, 2)));
	if(Log){
		pOut = log(pOut);
	}
	return pOut;
}


// main // to be replaced with Rcpp chunk
// extern "C" {
// int run_sim(int habitat_preference, int step_shape, int directional_bias, int roost_lambda,
//          double environmentResolution, double startT, int startDoy, double solLat,
//          double solLon, int upper_left_left, int upper_left_top, unsigned long int randomSeed,
//          std::vector<unsigned int> times) {
// //	Solar sol;
// //	sol.solarcalc(31, 47.999, 7.8421);
// //	std::cout <<  "sunrise = " << sol.sunrise <<
// //			" sunset = " << sol.sunset << std::endl;

// 	// read txt matrix
// //    std::ifstream in("D:/InteamDocs/Uni/littleowl/test.txt", std::ios_base::in | std::ios_base::binary);
// //    std::vector<std::vector<double> >*v = new std::vector<std::vector<double> >
// //    	 (0, std::vector<double> (0));
// //    if (in) {
// //        std::string line;
// //
// //        while (std::getline(in, line)) {
// //          std::istringstream buffer(line);
// //          std::vector<double> line((std::istream_iterator<double>(buffer)),
// //                                   std::istream_iterator<double>());
// //
// //          v->push_back(line);
// //        }
// ////        while (std::getline(in, line)) {
// ////            v->push_back(std::vector<double>());
// ////
// ////            // Break down the row into column values
// ////            std::stringstream split(line);
// ////            double value;
// ////
// ////            while (split >> value)
// ////                v->back().push_back(value);
// ////        }
// //    }
// //
// //    for (int i = 0; i < v->size(); i++) {
// //        for (int j = 0; j < v->at(i).size(); j++)
// //            std::cout << v->at(i).at(j) << ' ';
// //
// //        std::cout << '\n';
// //    }

//     std::cout << "start " << std::endl;
//     // std::cout << argc << std::endl;


//     // int habitat_preference = 0;
//     // int step_shape = 0;
//     // int directional_bias = 0;
//     // int roost_lambda = 0;
//     // double environmentResolution = 20;  // TODO: CONFIRM
//     // // if (argc == 5) { // TODO: check 5???
//     // habitat_preference = std::stod(argv[1]);
//     // step_shape = std::stod(argv[2]);
//     // directional_bias = std::stod(argv[3]);
//     // roost_lambda = std::stod(argv[4]);

//     // double startT = std::stod(argv[5]);
//     // int startDoy = std::stoi(argv[6]);
//     // double solLat = std::stod(argv[7]);
//     // double solLon = std::stod(argv[8]);
//     // int upper_left_left = std::stoi(argv[9]);
//     // int upper_left_top = std::stoi(argv[10]);
//     // unsigned long int randomSeed = std::stoi(argv[11]);
//     // std::vector<unsigned int> times;
//     // for(int i = 12; i < argc; i++) {  // TODO!
//     //     times.push_back(std::stoi(argv[i]));
//     //     // std::cout << std::stoi(argv[i]) << std::endl;
//     // }

//     // for our purposes... want
//     //
//     // m['rho'], m['k'], m['tau'], m['lmda_r'],
// 	//

//     // TODO: ENVIRONMENT
//     // TODO: upperleft ...

// 	RandomGenerator ranGen;

// 	// use std::vector
//     // TODO: Probably set up correctly...
// 	// unsigned int lat = 10000; // latitude, height
// 	// unsigned int lon = 10000; // longitude, width
// 	// unsigned int randomSeed = 15;

// 	std::vector<double> param;
//     // TODO: Probably set up correctly...
//     // TODO! HANDCODED FOR ONE
//     // TODO: LOOK INTO UPPER LEFT AND DIVIDING BY RESOLUTION
// 	unsigned int startX = 3133;
// 	unsigned int startY = 2589;
// 	param.push_back(startX);
// 	param.push_back(startY);
// 	param.push_back(habitat_preference); // habitat preference INCLUDE
// 	param.push_back(step_shape); // stepShape  INCLUDE
// 	param.push_back(2); // stepScale
// 	param.push_back(directional_bias); // directional bias INCLUDE
// 	param.push_back(1.5); // dispersal resting time logMean
// 	param.push_back(0.7); // dispersal resting time logSd
// 	param.push_back(10); // maximum effort
// 	param.push_back(roost_lambda); // roost lambda INCLUDE
// //	param.push_back(4); // recoveryRate
// //	param.push_back(-5); // beta1
// //	param.push_back(5); // beta2
// 	param.push_back(1); // observationError
// 	param.push_back(5); // rsc range


// 	// double startT = 14.32;
// 	// int startDoy = 150;
// 	// double solLat = 47.999;
// 	// double solLon = 7.8421;
// 	// double resolution = 1;

//     // // TODO: INPUT TIMES...
// 	// std::vector<unsigned int> times;
// 	// times.push_back(0);
// 	// times.push_back(2373);
// 	// times.push_back(6863);
// 	// times.push_back(6868);
// 	// times.push_back(13909);
// 	// times.push_back(13914);
// 	// times.push_back(21381);
// 	// times.push_back(21386);
// 	// times.push_back(22030);
// 	// times.push_back(27039);
// 	// times.push_back(27044);
// 	// times.push_back(34218);
// 	// = {0, 2373, 2378, 6863,
// 	// 		6868, 13909, 13914, 21381, 21386, 22030, 27039,
// 	// 		27044, 34218};
//     // std::cout << "here" << std::endl;
//     // std::vector<double> environment = {};
//     // TIFF* tif = TIFFOpen("habitatSuitability_scaled_20.tif", "r");

//     std::string filename = "habitatSuitability_scaled_20_new.txt";

//     int nr = 15092, nc = 16730;
//     std::vector<std::vector<double> >* landscape = new std::vector<std::vector<double> >
//         (nr, std::vector<double> (nc));

//     // if(filename[0] == "empty"){
//     //     for(int c = 0; c < nc; c++){
//     //     for(int r = 0; r < nr; r++){
//     //         landscape->at(r).at(c) = environment(r, c);
//     //     }
//     //     }
//     // }
//     // else{
//         	// read txt matrix
//     // std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
//     // std::vector<std::vector<double> >*landscape = new std::vector<std::vector<double> >
//     //     (0, std::vector<double> (0));
//     // // if (in) {
//     // std::string line;

//     // while (std::getline(in, line)) {
//     //     std::istringstream buffer(line);
//     //     std::vector<double> line((std::istream_iterator<double>(buffer)),
//     //                             std::istream_iterator<double>());

//     //     landscape->push_back(line);
//     // }
// //        while (std::getline(in, line)) {
// //            v->push_back(std::vector<double>());
// //
// //            // Break down the row into column values
// //            std::stringstream split(line);
// //            double value;
// //
// //            while (split >> value)
// //                v->back().push_back(value);
// //        }

//     // for (int i = 0; i < landscape->size(); i++) {
//     //     for (int j = 0; j < landscape->at(i).size(); j++)
//     //         std::cout << landscape->at(i).at(j) << ' ';

//     //     std::cout << '\n';
//     // }
//     // std::cout << "file_test_neg1" << std::endl;
//     // long int data_size = nr * nc;
//     // std::cout << "file_test_neg15" << std::endl;
//     // // double habitat_array[1000];
//     // std::cout << "file_test_neg2" << std::endl;
//     std::string fname = filename;
//     // // read txt matrix
//     // TODO: SWITCHED infile
//     std::ifstream inFile(fname);
//     // std::ifstream inFile ("habitatSuitability_scaled_20_new_bin.bin", std::ios::in | std::ios::binary);
//     // if (!inFile) {
//     //     std::cout << "cannot open" << std::endl;
//     // }


//     // char buffer[100];  // TODO: MADE UP NUMBER
//     // m
//     // std::cout << "file_test_1" << std::endl;
//     // for(int i = 0; i < nr; i++) {
//     //     // for(int j = 0; j < nc; j++) {
            
//     //     // }
//     //     std::vector<char> line;
//     //     inFile.read(line, sizeof(double) * nc);
//     //     landscape[i] = line;
//     //     // landscape[i] = // TODO:
//     // }
//     // //     // myFile >> habitat_array[i];
//     // // }
//     // std::cout << "file_test_2" << std::endl;

//     // myFile.close();
//     // std::cout << "here2" << std::endl;

//     // inFile.seekg(0, std::ios::end);
//     // size_t filesize=inFile.tellg();
//     // inFile.seekg(0, std::ios::beg);
//     // char buffer[filesize];  // TODO: abandoned this too big...
//     // inFile.read(buffer, filesize);
//     // inFile.close();

//     // double* double_data = (double*)buffer;
//     // std::vector<double> buffer2;
//     // std::vector<double> buffer2(double_data, double_data + (filesize/sizeof(double)));
//     // std::cout << "file_test_2" << std::endl;
//     // .resize(filesize/sizeof(myType));\

//     auto start = high_resolution_clock::now();


//     // todo! uncomment !
//     if (inFile.good()) {
//         std::string temp;
//         // std::cout << "here3" << std::endl;

//         int i = 0;
//         double tempdouble;
//         char delimiter;
//         while (std::getline(inFile, temp)) {
//             std::istringstream buffer(temp);
//             // TODO! faster approach?
//             auto start1 = high_resolution_clock::now();
//             // TODO: changed istreambuf_iterator from istream_iterator
//             // std::vector<double> line;
//             // line.resize()
//             // std
//             std::vector<double> line((std::istream_iterator<double>(buffer)),
//                                      std::istream_iterator<double>());

//             // std::vector<double> line; //(nc);  //todo? test fixed size vector
//             // while (buffer >> tempdouble) {
//             //     line.push_back(tempdouble);
//             //     buffer >> delimiter;
//             // }
//             // landscape.e
//             auto stop1 = high_resolution_clock::now();

//             auto duration1 = duration_cast<microseconds>(stop1 - start1);
//             std::cout << "duration1: " << duration1.count() << std::endl;

//             auto start2 = high_resolution_clock::now();
//             landscape -> emplace_back(line);
//             auto stop2 = high_resolution_clock::now();

//             auto duration2 = duration_cast<microseconds>(stop2 - start2);
//             std::cout << "duration2: " << duration2.count() << std::endl;

//             i += 1;
//         }
//     }

//     inFile.close();

//     // {
//     //     std::ofstream ofs("/tmp/copy.ser");
//     //     boost::archive::text_oarchive oa(ofs);
//     //     oa & landscape;
//     // }

//     // std::vector<std::vector<double> >* landscape2 = new std::vector<std::vector<double> >
//     //     (nr, std::vector<double> (nc));

//     // {
//     //     std::ifstream ifs("/tmp/copy.ser");
//     //     boost::archive::text_iarchive ia(ifs);
//     //     ia & landscape2;
//     // }

//     auto stop = high_resolution_clock::now();

//     auto duration = duration_cast<microseconds>(stop - start);
 
//     // To get the value of duration use the count()
//     // member function on the duration object
//     std::cout << duration.count() << std::endl;


//     // std::cout << "here4" << std::endl;

// // }

// 	// std::vector<std::vector<double> >*landscape = new std::vector<std::vector<double> >
// 	//  (lat, std::vector<double> (lon));

// 	// fill array, row major
// //	for(unsigned int y = 0; y < lon; y++){
// //		for(unsigned int x = 0; x < lat; x++){
// //			landscape->at(x).at(y) = (gaussianPdf(y, lon/2, 2, true) +
// //										 gaussianPdf(x, lat/2, 2, true)) * -1;
// //		}
// //	}

// 	// for(unsigned int y = 0; y < lon; y++){
// 	// 	for(unsigned int x = 0; x < lat; x++){
//     //         std::cout << y << std::endl;
// 	// 		landscape->at(x).at(y) = 1;
// 	// 	}
// 	// }
//     // std::cout << "here5" << std::endl;
// 	// // set random seed
// 	ranGen.seedrand(randomSeed);

// //	bool checkBinom = ranGen.rBinomial(0.9);
// //	std::cout << checkBinom << std::endl;

// 	Model mod(landscape);
// 	// displayMatrix(*mod.env->values, lat,lon);
//     // std::cout << "here6" << std::endl;
// 	// std::vector<unsigned int> tt;
// 	int maxDuration = 40000;
// 	Individual* indOut = mod.runSimulation(param, 2000, maxDuration, ranGen, times,
// 			startT, startDoy, solLat, solLon, environmentResolution);
//     // std::cout << "here7" << std::endl;

//     int batch_size = indOut->xObs.size();
//     // int batch_size = 1;
//     // auto out_matrix = new int [1][3];
//     for(int i = 0; i < times.size(); i++) {  //todo? check last
//         int tstamp = times.at(i);
//         auto tstamp_str = std::to_string(tstamp);
//         std::cout << tstamp_str << ',';
//         std::cout << indOut->xObs.at(i)*environmentResolution + upper_left_left << ','; // TODO: PRINT MORE DETAIL?
//         std::cout << indOut->yObs.at(i)*environmentResolution + upper_left_top  << ','; // todo? need negative
//         std::cout << indOut->stepDistanceObs.at(i)*environmentResolution << ','; // todo? * environmentResolution;
//         std::cout << indOut->turningAngleObs.at(i) << ',';
//         std::cout << indOut->envValObs.at(i) << ',';
//         std::cout << indOut->rsc.at(i) << std::endl;
//             // out_matrix[i][0] = indOut->timestamp.at(i);
//         // out_matrix[i][1] = indOut->x.at(i); // * environmentResolution + upperleft(0);
//         // out_matrix[i][2] = -indOut->y.at(i); // * environmentResolution + upperleft(1);

//     }
// //	std::cout << indOut->observationIndex.at(0)
// //			<< "\n" << indOut->observationIndex.at(1)
// //			<< "\n" << indOut->observationIndex.at(2)
// //			<< "\n" << indOut->observationIndex.at(3)
// //			<< " and nSim: " << indOut->x.size()
// //			<< std::endl;
// 	// std::cout << indOut->x.at(1) << " and rsc2: " << indOut->rsc.at(2) << std::endl;
// //	std::cout << indOut->validityCode << " " << indOut->numberOfSteps << std::endl;
// 	//displayMatrix(out, 2,6);
// 	// std::cout << "helloworld";
//     // std::cout << observationTimeVec.at(0);
//     // delete landscape;

// 	// compute 95% quantile for gamma distribution
// //	double shape = 2;
// //	double scale = 1;
// //	boost::math::gamma_distribution<double> d(shape, scale);
// //	std::cout << quantile(d, 0.95) << std::endl;


// 	return 0;
//     // delete out_matrix;
// }
// }

int main() {
    return 0;
}