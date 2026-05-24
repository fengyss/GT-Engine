cd ..\backup
git status
pause

git add .
pause

git commit -m "feat: add more event test." -m "misc: refactor dependencies.lua for lib to match configuration."

pause

git push -u origin main
pause