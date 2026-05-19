cd ..\backup
git status
pause

git add .
pause

git commit -m "fix: fix when render shadowmap pop error for uniform." -m "fix: Add (de)serizlize for Animator2DComponent."

pause

git push -u origin main
pause