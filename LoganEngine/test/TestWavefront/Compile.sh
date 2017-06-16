
mkdir ./build
cd ./build

cmake ../../../src/
cmake --build ./ --target TestWavefront

cp ./lTest/TestWavefront/TestWavefront ../Test

cd ../
rm -rf ./build
