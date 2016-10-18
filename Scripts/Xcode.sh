mkdir -p ../Build
rm -r ../Build/*
cd ../Build
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ../ -G "Xcode"
