cd ..
mkdir -p projects
cd projects
mkdir -p android_make_armv8a
cd android_make_armv8a

cmake .. -DCMAKE_TOOLCHAIN_FILE=../../cmake/android.cmake