all: matrixmult.cpp simd_big_matrixmult.cpp dac.cpp
	g++ -o mat matrixmult.cpp
	g++ -o simd simd_big_matrixmult.cpp
	g++ -o dac dac.cpp

