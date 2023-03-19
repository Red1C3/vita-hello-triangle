# vita-hello-triangle
A vitaGL sample that renders a triangle.

This sample uses GLSL 1.00, which gets translated to Nvidia's Cg, vitaGL's support for GLSL translation is still expiremental by the time of this commit.

### How do I quit?
Press any button.

## Perequisites
### For running
- libshacccg installed.
### For building
- vitaSDK installed
- [Northfear's SDL2](https://github.com/Northfear/SDL/tree/vitagl)
- [vitaGL built with HAVE_GLSL_SUPPORT=1](https://github.com/Rinnegatamante/vitaGL)

## Building
```
mkdir build
cd build
cmake ..
make
```
Then install the resultant ```./build/vita-hello-triangle``` using vitaShell.

## License
[Unlicense](./LICENSE)