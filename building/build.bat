rm build
a
cd ..
mkdir build
cd build
cmake .. -G "Ninja" -D EXECUTABLE_OUTPUT_PATH="../" -DCMAKE_C_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\g++.exe --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release
cmake --build . --config Release
cd ..
rm build
a