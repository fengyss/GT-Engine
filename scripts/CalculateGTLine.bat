@echo off

echo ========================================
echo Counting GT directory
echo ========================================
cloc ..\GT\src --exclude-dir=vendor --match-f="\.(cpp|h)$"


echo.
echo ========================================
echo Counting GTEditor directory
echo ========================================
cloc ..\GTEditor\src --exclude-dir=vendor --match-f="\.(cpp|h)$"

pause