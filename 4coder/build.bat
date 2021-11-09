@echo off

DEL "source\*.exe" /S /Q 
DEL "source\*.o" /S /Q 
DEL "source\*.gch" /S /Q

pushd source

make

popd
