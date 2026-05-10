cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: if sprite don't have .json, will temporarily disable Animation2DComponent."
pause

git push -u origin main
pause