call CalculateGTLine.bat
popd

cd ..

xcopy "GT/src" "backup/GT/src" /e /y
copy "GT/premake5.lua" "backup/GT"


xcopy "GT/vendor/assimp" "backup/GT/vendor/assimp" /e /y

xcopy "GT/vendor/Box2D/include" "backup/GT/vendor/Box2D/include" /e /y
xcopy "GT/vendor/Box2D/src" "backup/GT/vendor/Box2D/src" /e /y
copy "GT/vendor/Box2D/premake5.lua" "backup/GT/vendor/Box2D/premake5.lua"
copy "GT/vendor/Box2D/LICENSE" "backup/GT/vendor/Box2D/LICENSE"


xcopy "GT/vendor/efsw/include" "backup/GT/vendor/efsw/include" /e /y
xcopy "GT/vendor/efsw/src" "backup/GT/vendor/efsw/src" /e /y
copy "GT/vendor/efsw/premake5.lua" "backup/GT/vendor/efsw/premake5.lua"
copy "GT/vendor/efsw/LICENSE" "backup/GT/vendor/efsw/LICENSE"


xcopy "GT/vendor/entt/entt" "backup/GT/vendor/entt/entt" /e /y
copy "GT/vendor/entt/premake5.lua" "backup/GT/vendor/entt/premake5.lua"
copy "GT/vendor/entt/LICENSE" "backup/GT/vendor/entt/LICENSE"


xcopy "GT/vendor/freetype/**.h" "backup/GT/vendor/freetype" /e /y


xcopy "Test/src" "backup/Test/src" /e /y
copy "Test/premake5.lua" "backup/Test"



xcopy "GTEditor/projects" "backup/GTEditor/projects" /e /y
xcopy "GTEditor/src" "backup/GTEditor/src" /e /y
xcopy "GTEditor/Resources" "backup/GTEditor/Resources" /e /y
copy "GTEditor/premake5.lua" "backup/GTEditor"
copy "GTEditor/imgui.ini" "backup/GTEditor"



xcopy "GTScriptCore/Source" "backup/GTScriptCore/Source" /e /y
copy "GTScriptCore/premake5.lua" "backup/GTScriptCore"



xcopy "docs" "backup/docs" /e /y

xcopy "scripts" "backup/scripts" /e /y

xcopy "vendor" "backup/vendor" /e /y


copy "premake5.lua" "backup"


copy "Readme.md" "backup"


copy "Dependencies.lua" "backup"


cd GT
copy "premake5.lua" "../backup/GT"
pause
