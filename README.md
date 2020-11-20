# bmp_rotate

It is a simple program which (depends on plugins) transform bmp images.

# Features

  - Flag parsing by getopt
  - Plugins is dynamic libraries destinated in the "lib" directory


# Flags
  - "-t" or "--transform" contains plugin name
  - "-p" or "--picture" is a path to the image
  - "-o" or "--output" is a  path to the destination image file
  - "-a" or "--arg" contains arguments, which used by plugun (optional)

### Installation

This project available only on Linux

```sh
$ make proj_struct
$ make
```

"make all" already contains compiling of two embedded plugins (rotation and straight), but you can compile it by yourself

```sh
make librotation.so
make libstraight.so
```

### Example

You can run the program, this is an example of usage (plugin is rotation, source pic is res/picture.bmp, output pic is res/result.bmp, the angle is 35)

```sh
build/main -t rotation -p res/picture.bmp -o res/result.bmp -a 35
```

### Plugin

The plugin must contain only one function, same as library name (without prefix lib and .so) and take "struct image*" and "char*", which is arguments
There are two embedded plugins: rotation.c and straight.c

---
**btw I use arch linux**
