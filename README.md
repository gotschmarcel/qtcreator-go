# Qt Creator Go Plugin

A Qt Creator plugin for the **Go** programming language.

## Features

- Managing projects (work in progress)
- Syntax Highlighting

More features like code completion, go to definition and more are coming in the future.

## Build

### Requirements

- Qt 5.6 (other versions not tested)

### Instructions (\*NIX)

After cloning into this repository, you need to checkout the Qt Creator source code using:

```sh
$ git submodule update --init --recursive
```

Create the Qt Creator build output directory

```sh
$ mkdir -p build/release/qt-creator
$ cd build/release/qt-creator
```

Build Qt Creator

```sh
$ qmake ../../../qt-creator/qt-creator.pro -spec <spec> CONFIG+=release CONFIG+=x86_64
$ make qmake_all
$ make
```

Build the plugin

```sh
$ cd ..
$ qmake ../../go.pro -spec <spec> CONFIG+=release CONFIG+=x86_64
$ make qmake_all
$ make
```

> - To install the plugin into the user directory also pass `USE_USER_DESTDIR=yes` to `qmake`.
> - The `-spec` depends on the system, see QMAKESPEC for more details.



