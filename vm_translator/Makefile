.ONESHELL:

build: clean FORCE
	conan install .  --build=missing 
	cd build/Release
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build .
build-debug: clean FORCE
	conan install . --build=missing --settings=build_type=Debug
	cd build/Debug
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja
	cmake --build . 
run: build-debug
	 ./build/Debug/vm_translator ./data/advanced/dino/Main.vm -c config_example.toml --debug

install: build-debug FORCE
	cmake --install ./build/Debug --config Debug
clean:
	rm -rf build
test: build FORCE
	cd  ./build/Release && ctest
test-debug: build FORCE
	cd  ./build/Debug && ctest


test-prog: build FORCE
	./build/Release/test_prog
FORCE: ;