.ONESHELL:

build: FORCE
	conan install . --output-folder=build --build=missing
	cd build
	cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build .
build-debug: FORCE
	conan install . --output-folder=build --build=missing --settings=build_type=Debug
	cd build
	cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
	cmake --build .
build-layout: FORCE
	conan install .  --build=missing
	cd build/Release
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build .
build-debug-layout: FORCE
	conan install . --build=missing --settings=build_type=Debug
	cd build/Debug
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
	cmake --build .
run:
	./build/Debug/assembler data/add.asm
clean:
	rm -rf build
test: build FORCE
	cd  ./build && ctest

FORCE: ;