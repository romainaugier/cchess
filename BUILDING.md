# cchess

cchess uses CMake as a buildsystem, and has utility scripts to build on each supported platform (Windows and Linux for now).

Clone submodules and build them, for now there is only libromano as a dependency.

Building debug and release to link properly when building cchess in debug/release:
```bash
git submodule update --init
cd libromano
./build.sh --debug
./build.sh
```

Building cchess (pass --debug to build in debug mode, --tests to build and run the unit tests) is then straightforward:
```bash
./build.sh --debug --tests
```