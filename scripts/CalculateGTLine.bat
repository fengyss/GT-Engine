
@echo off
setlocal enabledelayedexpansion

set CSV=..\docs\documents\code_diary.csv

for /f "tokens=1-3 delims=/- " %%a in ("%date%") do (
    set YYYY=%%a
    set MM=%%b
    set DD=%%c
)
set TODAY=%YYYY%-%MM%-%DD%

if not exist "%CSV%" (
    echo Date,GT_CodeLines,GTEditor_CodeLines,Sum > "%CSV%"
)

for /f "skip=3 tokens=5 delims=," %%i in (
    'cloc ..\GT\src --exclude-dir=vendor --match-f="\.(cpp|h)$" --csv'
) do (
    set GT_LINES=%%i
    goto :next1
)
:next1

for /f "skip=3 tokens=5 delims=," %%i in (
    'cloc ..\GTEditor\src --exclude-dir=vendor --match-f="\.(cpp|h)$" --csv'
) do (
    set GTE_LINES=%%i
    goto :next2
)
:next2

set /a TOTAL=GT_LINES+GTE_LINES

echo %TODAY%,!GT_LINES!,!GTE_LINES!,!TOTAL! >> "%CSV%"

echo.
echo Calculate : %TODAY%
echo GT : !GT_LINES! line
echo GTEditor : !GTE_LINES! line
echo.

pause











