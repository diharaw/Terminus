cd ..
mkdir -p projects
cd projects
mkdir -p ios_xcode_arm
cd ios_xcode_arm

cmake .. -DCMAKE_TOOLCHAIN_FILE=../../cmake/ios.cmake -DIOS_PLATFORM=OS -H. -Bbuild.ios -GXcode