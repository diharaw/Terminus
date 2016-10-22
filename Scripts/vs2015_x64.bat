 cd ..
 IF NOT EXIST Projects mkdir Projects
 cd Projects
 mkdir vs2015_x64
 cd vs2015_x64
 cmake ../.. -G "Visual Studio 14 Win64"
 timeout /t -1
