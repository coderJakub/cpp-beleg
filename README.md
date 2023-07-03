<h2>CMake configurations:</h2>
cmake -B ./build -DCMAKE_BUILD_TYPE=Release <br/> <br/>

<h2>Build command:</h2>
cmake --build ./build --config Release<br/> <br/>

<h2>Copy example data into build directory:</h2>
cp ./exampleData/Media.txt ./build/Media.txt <br/>
cp ./exampleData/People.txt ./build/People.txt<br/> <br/>

<h2>Run program:</h2>
./build/beleg
