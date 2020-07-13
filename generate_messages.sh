cd external/flatbuffers
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make
cd ../..
./external/flatbuffers/flatc -c -o protocol/messages/ flatbuffers/sample.fbs