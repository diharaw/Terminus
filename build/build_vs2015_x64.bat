 cd ..
 IF NOT EXIST projects mkdir projects
 cd projects
 mkdir vs2015_x64
 cd vs2015_x64
 cmake ../.. -G "Visual Studio 14 Win64"
 timeout /t -1
