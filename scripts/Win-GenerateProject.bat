pushd ..\
call vendor\premake\bin\premake5.exe vs2022
popd
pause

call Backup.bat
popd