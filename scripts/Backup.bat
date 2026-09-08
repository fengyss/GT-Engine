
cd ..

xcopy "GT" "backup/GT" /e /y
xcopy "Test" "backup/Test" /e /y



xcopy "GTEditor" "backup/GTEditor" /e /y
xcopy "GTScriptCore" "backup/GTScriptCore" /e /y



xcopy "docs" "backup/docs" /e /y

xcopy "scripts" "backup/scripts" /e /y




copy ".clangd" "backup"
copy "premake5.lua" "backup"
copy "Readme.md" "backup"
copy "Dependencies.lua" "backup"
copy "LICENSE" "backup"
copy "GT.sln" "backup"
copy "imgui.ini" "backup"


pause
