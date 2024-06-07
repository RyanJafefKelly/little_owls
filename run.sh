g++ -c -fpic -fdiagnostics-color=always -std=c++11 -g *.cpp
g++ -std=c++11 -shared -Wl,-soname=runsim.so -o runsim.so  *.o -v       
