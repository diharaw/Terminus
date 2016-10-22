mkdir -p ../Projects
rm -r ../Projects/*
cd ../Projects
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ../ -G "Xcode"
