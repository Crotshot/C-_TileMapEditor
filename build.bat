@echo off
if not exist build mkdir build
pushd source

make

popd