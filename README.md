# Token Bucket Algorithm Implementation and Simulation

An implementation and simulation of the Token Bucket Algorithm in C++.

## Compilation
Before any thing, at the beggining 'TokenBucketAlgortihm.h' header file, there a preprocessed constant variable named TICKS which determine how many ticks will be done, change it for you needs.
### Linux
You could easily compile and link the project using one **g++** line, with full check for any errors or warnings [-Wall -Wextra] and the output [-o] executable will be optimised [-O]:
> g++ -Wall -Wextra -O main.cpp TokenBucketAlgorithm.cpp -o TokenBucketAlgorithm

or you could use **CMake**:
To configure and make the build files, use command:
> cmake -S . -B build

The generated files will be in a new subdirectory named 'build'

To compile the code, cd to the build subdirectory and run:
> make

### Windows
You could easily open the project in Visual studio and the rest is on it.

#### OR from the Powershell, yoy have two options:
##### 1 - Visual Studio
If you already have Visual studio 2019 you could use this **one line** command (For community edition)
[Note: check the version of the MSVC you have, in my case it was 14.28.29910]:
> cmd /c 'call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" && "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\cl.exe" /EHsc /c main.cpp && "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\cl.exe" /EHsc /c TokenBucketAlgorithm.cpp && "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\link.exe" main.obj TokenBucketAlgorithm.obj'

##### 2 - Build Tools for Visual Studio 2019
Also, you could use the Microsoft C/C++ Build tools
[Down from here](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019),
and use this **one line** command (2019 Vsersion)
[Note: check the version of the MSVC you have, in my case it was 14.28.29910]:
> cmd /c 'call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64" && "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\cl.exe" /EHsc /c main.cpp && "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\cl.exe" /EHsc /c TokenBucketAlgorithm.cpp && "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.28.29910\bin\Hostx64\x64\ling.exe" main.obj TokenBucketAlgorithm.obj'

## Running & Usage
Then you could use the executable 'TokenBucketAlgorithm':

> ./TokenBucketAlgorithm [bucket maximum size] [bucket tocken rate] [queue size] [queue maximum packet arrival eate] [packets maximum size]

or just run it withoout arguments, and it will ask you for:
- Maximum packet arrival rate :
- Maximum packet size :
- Size of the queue :
- Token rate :
- Bucket capacity :


## Debugging & Logging
Also, you could compile it with the LOGGING flag, to get a detailed output for debugging, you add:

- At the begining of 'TokenBucketAlgorithm.h' header file:
> #define DEBUG


Then you could use the executable 'TokenBucketAlgorithm', and get a detailed info and the current status of everything in each step (function):
