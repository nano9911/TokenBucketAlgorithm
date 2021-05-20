# CMakeList.txt : CMake project for TokenBucketAlgorithm, include source and define
cmake_minimum_required (VERSION 3.8)

project ("TokenBucketAlgorithm")

# Add source to this project's executable.
add_executable (TokenBucketAlgorithm "TokenBucketAlgorithm.cpp" "TokenBucketAlgorithm.h" "main.cpp")
