# cpp-beleg
CMake configurations: <br/>
cmake -B ./build -DCMAKE_BUILD_TYPE=Release <br/> <br/>

Build command: <br/>
cmake --build ./build --config Release<br/> <br/>

Copy example data into build directory: <br/>
cp ./exampleData/Media.txt ./build/Media.txt <br/>
cp ./exampleData/People.txt ./build/People.txt<br/> <br/>

Run program: <br/>
./build/beleg
