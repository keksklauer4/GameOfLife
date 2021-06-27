mkdir build
cd build
cmake ..
cd test

# make test folder
make

# generate test data
./gen_test_data

# assemble all generated test files into .bin files
cd data
bash assemble_tests.sh

# run actual tests
cd ..
./8051EmulatorTest