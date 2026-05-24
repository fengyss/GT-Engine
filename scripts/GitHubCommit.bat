cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: fix program won't end correctly cause opengl resouse release error." -m "feat: add vld to detect memory leak at debug." -m "fix: release memory at end for some file."

pause

git push -u origin main
pause