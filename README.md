# cpp-beleg
<h4>CMake configurations:</h4> <br/>
cmake -B ./build -DCMAKE_BUILD_TYPE=Release <br/> <br/>

<h4>Build command:</h4><br/>
cmake --build ./build --config Release<br/> <br/>

<h4>Copy example data into build directory:</h4><br/>
cp ./exampleData/Media.txt ./build/Media.txt <br/>
cp ./exampleData/People.txt ./build/People.txt<br/> <br/>

<h4>Run program:</h4> <br/>
./build/beleg
