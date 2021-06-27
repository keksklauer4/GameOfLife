mkdir build
cd build
cmake ..
cd src
make
cp lib8051EmulatorStaticd.a ../../gui/libemulator.a
cd ../../

cd gui
mkdir build
cd build
qmake ..
make

cp LEDMatrix ../../LEDMatrix