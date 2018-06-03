# Qt Creator Go Plugin

A Qt Creator plugin for the **Go** programming language.

> This plugin is not well maintained, since I don't have the time for it.

## Features

- Managing projects (work in progress)
- Syntax Highlighting

More features like code completion, go to definition and more are coming in the future.

## Build

### Requirements

- Qt 5.6 (other versions not tested)
- QtCreator 4.4.x

### Instructions (\*NIX)

The following instructions guide you through the process of building the plugin All instructions are run from the project root.
Create the build directory.

```sh
$ mkdir <PROJECT_ROOT>/build
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



