
mkdir ./build
cd ./build

cmake ../../../src/
cmake --build ./ --target TestJSON 

cp ./lTest/TestJSON/TestJSON ../Test

cd ../
rm -rf ./build
