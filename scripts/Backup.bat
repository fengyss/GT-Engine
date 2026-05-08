
cd ..

xcopy "GT/src" "backup/GT/src" /e /y
xcopy "GT/vendor" "backup/GT/vendor" /e /y



xcopy "GTEditor" "backup/GTEditor" /e /y



xcopy "scripts" "backup/scripts" /e /y


copy "GT.sln" "backup"


copy "premake5.lua" "backup"


copy "Dependencies.lua" "backup"
pause
