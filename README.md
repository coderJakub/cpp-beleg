# cpp-beleg
<hr>CMake configurations:<\hr> <br/>
cmake -B ./build -DCMAKE_BUILD_TYPE=Release <br/> <br/>

<hr>Build command:<\hr> <br/>
cmake --build ./build --config Release<br/> <br/>

<hr>Copy example data into build directory:<\hr> <br/>
cp ./exampleData/Media.txt ./build/Media.txt <br/>
cp ./exampleData/People.txt ./build/People.txt<br/> <br/>

<hr>Run program:<\hr> <br/>
./build/beleg
