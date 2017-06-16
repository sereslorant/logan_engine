
mkdir ./build
cd ./build

cmake ../../../src/
cmake --build ./ --target TestMd5Anim

cp ./lTest/TestMd5/TestMd5Anim ../Test

cd ../
rm -rf ./build
