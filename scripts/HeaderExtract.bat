cd ..
robocopy "GT/src" "backup/include" *.h /s
xcopy "GT/mono" "backup/include/mono" /e /y
xcopy "GT/Resources" "backup/include/Resources" /e /y
pause
