
mkdir ./build
cd ./build

cmake ../../../src/
cmake --build ./ --target TestMd5File

cp ./lTest/TestMd5/TestMd5File ../Test

cd ../
rm -rf ./build
