# cpp-beleg
CMake configurations://
cmake -B ./build -DCMAKE_BUILD_TYPE=Release

Build command:
cmake --build ./build --config Release

Copy example data into build directory:
cp ./exampleData/Media.txt ./build/Media.txt
cp ./exampleData/People.txt ./build/People.txt

Run program:
./build/beleg
