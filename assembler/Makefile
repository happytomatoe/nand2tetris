build:clean
	conan install . --output-folder=build --build=missing
	cmake -Bbuild -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build build
run:
	./build/build/Release/assembler data/add.asm
clean:
	rm -rf build
test: build FORCE
	cd  ./build && ctest

FORCE: ;