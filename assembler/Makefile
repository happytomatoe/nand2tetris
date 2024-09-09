.ONESHELL:

build: FORCE
	conan install .  --build=missing
	cd build/Release
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build .
build-debug: FORCE
	conan install . --build=missing --settings=build_type=Debug
	cd build/Debug
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja
	cmake --build .
run:
	 ./build/Debug/assembler  ./data/MaxL.asm


install: build FORCE
	cmake --install ./build/Release --config Release

perf: FORCE
	#sudo perf record -g -F 99 -a ./build/Debug/assembler  ./data/PongLong.asm -o Pong.hack
	sudo perf mem record -g -F 99 -a ./build/Debug/assembler  ./data/PongLong.asm -o Pong.hack
	sudo chmod 777 perf.data
	mkdir -p perf
	cp perf.data perf/
	cd perf
	perf script -i perf.data | ~/git/FlameGraph/stackcollapse-perf.pl > coll.out
	~/git/FlameGraph/flamegraph.pl coll.out > flamegraph.svg
	firefox flamegraph.svg
clean:
	rm -rf build
test: build FORCE
	cd  ./build/Release && ctest
test-debug: build FORCE
	cd  ./build/Debug && ctest
benchmark: build-debug FORCE
	 ./build/Debug/benchmark

FORCE: ;