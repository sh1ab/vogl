rm build
a
mkdir build
cd build
cmake .. -G"Visual Studio 16 2019"
cmake --build . --config Release
cd example
cd Release
start example.exe
exit