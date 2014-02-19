## Yet Another C Library
The YACL library is written in C99(require anonymous structure feature in C11 or GUN C),
which aims to do something crazy.


## HOWTO Build and Install
1. Obtain CMake from [http://www.cmake.org].
2. Download YACL library and extract it to `<dir>`.
3. `cd <dir>/yacl; mkdir build; cd build`
4. `cmake .. && make install`

## Some useful CMake options

```
-DCMAKE_BUILD_TYPE=Release|Debug
-DCMAKE_INSTALL_PREFIX=/usr
-DCMAKE_C_COMPILER=clang|gcc
-DBUILD_TESTING=ON|OFF
```

## *STILL UNDER HEAVY DEVELOPMENT*

##TODO
0. Performance...
1. Test...
2. Add more algos...
3. Move `_hash_`  out of `object_z` ??
