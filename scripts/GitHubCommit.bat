cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: fix uncorrect ratio of particle when viewport ratio is not 1.0."  -m "fix: fix after reload shader didn's render textures."

pause

git push -u origin main
pause