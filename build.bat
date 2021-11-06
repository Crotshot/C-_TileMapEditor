@echo off
if exist build del "build\*.*?" 
if exist build echo "Build folder exists deleting old build"
if not exist build "No build folder found, making new build folder"
if not exist build mkdir build 

pushd source

make

popd

echo "Moving exe to build folder"
move "source\*exe*.*" "build"