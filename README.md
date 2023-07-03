To configure you have to change to the "s85515" folder<br/><br/>

<h2>Create build folder:</h2>
mkdir build<br/>
cd build<br/>

<h2>Configure CMake:</h2>
cmake ..<br/><br/>

<h2>Copy example data into build directory:</h2>
cp ../exampleData/Media.txt .<br/>
cp ../exampleData/Person.txt .<br/> <br/>

<h2>Build runnable file</h2>
make beleg

<h2>Run program:</h2>
./beleg
