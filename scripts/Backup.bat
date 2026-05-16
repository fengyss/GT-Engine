call CalculateGTLine.bat
popd

cd ..

xcopy "GT/src" "backup/GT/src" /e /y



xcopy "GTEditor" "backup/GTEditor" /e /y


xcopy "docs" "backup/docs" /e /y

xcopy "scripts" "backup/scripts" /e /y


copy "GT.sln" "backup"


copy "premake5.lua" "backup"
copy "GT/premake5.lua" "backup/GT"

copy "Readme.md" "backup"


copy "Dependencies.lua" "backup"

pause
