.ONESHELL:

build:clean
	conan install . --output-folder=build --build=missing
	cd build
	cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build .
run:
	./build/assembler data/add.asm
clean:
	rm -rf build
test: build FORCE
	cd  ./build && ctest

FORCE: ;