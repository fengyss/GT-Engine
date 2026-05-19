cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: fix one copy error at backup.bat." -m "misc: split pos,offset.. for perspective and orthographic projection at editorcamera."

pause

git push -u origin main
pause