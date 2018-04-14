cd ..
mkdir -p projects
cd projects
mkdir -p ios_xcode_sim
cd ios_xcode_sim

cmake .. -DCMAKE_TOOLCHAIN_FILE=../../cmake/ios.cmake -DIOS_PLATFORM=SIMULATOR64 -H. -Bbuild.sim64 -GXcode
cmake --build build.sim64/ --config Release
