set PATH=%PATH%;C:\cygwin64\bin
dir > YOU_ARE_HERE.txt
del /S /F /Q Blocks\*.exe > NUL
type bpl_binary\CodeLibrary\RaytracerEngine_main.cpp >> Blocks\main.cpp
gcc -Os -s -o Blocks/test.exe Blocks/*.cpp -lopengl32 -lglu32 -lgdi32
Blocks\test.exe
