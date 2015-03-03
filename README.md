# libseekthermal

## Synopsis

Driver library for the Seek Thermal imaging device.

**Author(s):** Ralf Kaestner

**Maintainer:** Ralf Kaestner <ralf.kaestner@gmail.com>

**License:** GNU Lesser General Public License (LGPL)

**Operating system(s):** Debian-based Linux

**Package PPA:** ppa:ethz-asl/drivers

## Description

This project provides a driver library and basic command-line utilities for
operating Seek Thermal imaging devices under Linux.

## Installation

### Installing from packages (recommended for Ubuntu LTS users)

The maintainers of this project provide binary packages for the latest Ubuntu
LTS releases and commonly used system architectures. To install these packages,
you may follow these instructions:

* Add the project PPA to your APT sources by issuing

  ```
  sudo add-apt-repository ppa:ethz-asl/drivers
  ```

  on the command line

* To re-synchronize your package index files, run 

  ```
  sudo apt-get update
  ```

* Install all project packages through

  ```
  sudo apt-get install libseekthermal*
  ```

  or selected packages using your favorite package management tool

### Building from source

To build from source, this project requires the CMake build system with an
open-source macro extension called ReMake.

#### Preparing the build system

If you already have installed ReMake on your build system, you may
skip this step. Otherwise, before attempting to build this project the
traditional CMake way, you must install ReMake following
[these instructions](https://github.com/kralf/remake).

#### Installing build dependencies

The build dependencies of this project are available from the standard
package repositories of recent Ubuntu releases. To install them, simply
use the command

```
sudo apt-get install doxygen pkg-config
```

#### Building with CMake

Once ReMake is available on your build system, you may attempt to build this
project the CMake way. Assuming that you have cloned the project sources into
`PROJECT_DIR`, a typical out-of-source build might look like this:

* Create a build directory using 

  ```
  mkdir -p PROJECT_DIR/build
  ```

* Switch into the build directoy by 

  ```
  cd PROJECT_DIR/build
  ```

* In the build directory, run 

  ```
  cmake PROJECT_DIR
  ```

  to configure the build

* If you want to inspect or modify the build configuration, issue 

  ```
  ccmake PROJECT_DIR
  ```

* Build the project using 

  ```
  make
  ```

* If you intend to install the project, call 

  ```
  make packages_install
  ```

  (from packages on Debian-based Linux only) or 

  ```
  make install
  ```

## API documentation

This project generates its API documentation from source. To access it, you
may either inspect the build directory tree after the project has been built
using `make` or install the documentation package through

```
sudo apt-get install libseekthermal-doc
```

## Feature requests and bug reports

If you would like to propose a feature for this project, please consider
contributing or send a feature request to the project authors. Bugs may be
reported through the project's issue page.
