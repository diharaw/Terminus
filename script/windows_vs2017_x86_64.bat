 cd ..
 IF NOT EXIST projects mkdir projects
 cd projects
 mkdir windows_vs2017_x86_64
 cd windows_vs2017_x86_64

 cmake ../.. -G "Visual Studio 15 Win64"
 timeout /t -1