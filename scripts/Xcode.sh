mkdir -p ../projects
rm -r ../projects/*
cd ../projects
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ../ -G "Xcode"
