.ONESHELL:

build: FORCE
	conan install .  --build=missing
	cd build/Release
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build .
build-debug: FORCE
	conan install . --build=missing --settings=build_type=Debug
	cd build/Debug
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -Dtools.system.package_manager:mode=install
	cmake --build .
run: build-debug
	 ./build/Debug/image_converter

clean:
	rm -rf build
test: build FORCE
	cd  ./build/Release && ctest
test-debug: build FORCE
	cd  ./build/Debug && ctest

install: build FORCE
	cmake --install ./build/Release --config Release
test-prog: build FORCE
	./build/Release/test_prog
FORCE: ;