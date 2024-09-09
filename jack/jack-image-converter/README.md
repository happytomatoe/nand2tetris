# Convert a PNG file into Jack code
1) To install run the next command. By default, it will copy the binary to `~/bin`. 
You can change this in [CmakeLists.txt](CMakeLists.txt) by changing `install(TARGETS ${PROJECT_NAME} DESTINATION ~/bin)` 
```shell
make install
```
2) Run the utility. For example 
```shell
jack-image-converter data/cactus.png
```
Type `jack-image-converter --help` for all available options
## Dependencies:
By default, this utility will copy the output to clipboard. To use this you would probably need to install some OS package(-s).
More info on https://github.com/dacap/clip?tab=readme-ov-file#platform-specific-details

You can use `-o` flag so  that this utility will write to standard output.