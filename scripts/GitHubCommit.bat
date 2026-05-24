cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: add vld.lib dir to premake, delete some should not exited."

pause

git push -u origin main
pause