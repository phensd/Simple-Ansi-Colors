# Simple-Ansi-Colors

- [Use](#use)
- [Syntax](simple-syntax-overview)
- [Installation](#installation)
- [Linking with your project](#using-in-a-project)

## Use

Currently one public function is provided:
```sansic::form``` <br>
This takes a string literal, and interprets simple patterns in its text to replace with ANSI color escape sequences. <br>


A simple example: <br>
```sansic::form("(F255,0,0) Some text here");``` <br>

This will replace `(F255,0,0)` with the escape sequence `ESC[38;2;255;0;0m` causing the text to appear red in terminals which support [24 bit colors](https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit).


## Simple syntax overview

```
//Set foreground color to 255,255,255. Does not modify background color of this text.
sansic::form("(F255,255,255) Some text here")

//Same as above, modifies background instead.
sansic::form("(B255,255,255) Some text here")

//Modify both foreground and background at the same time.
//Note: the order of F/B does not matter.
sansic::form("(F10,30,10,B200,100,200) Some text here")

//Modify foreground color, using *8 bit* colors.
sansic::form("(F255) Some text here")

//Same as above, but background.
sansic::form("(B255) Some text here")

//This 8 bit syntax can be combined aswell.
sansic::form("(B255,F200) Some text here")

//In both forms, 24 bit and 8 bit, the integers will naturally wrap around.
//Here, it is the same as "300%256".
sansic::form("(B300,F300) Some text here")

//For both forms, 24 bit and 8 bit, many different delimiters can be used.
sansic::form("(B100|F200) Some text here")
sansic::form("(B255-100_200|F100!200~100) Some text here")
```
More examples can be seen in [test.cpp](https://github.com/phensd/Simple-Ansi-Colors/blob/main/test.cpp).

## Installation
```
#Clone the repo
git clone https://github.com/phensd/Simple-Ansi-Colors.git

#Go into the repo's directory and create a build directory, go to it
cd Simple-Ansi-Colors
mkdir build
cd build

#Generate build files
cmake -DCMAKE_BUILD_TYPE=Release ..
#Or, if you want to build the tests aswell:
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release ..

#Build and install it
make -j4
sudo make install
```

## Using in a project
Simple-Ansi-Colors can be used with CMake as simply as so:
```
project(your_project)

#Finds Simple-Ansi-Colors
find_package(sansic)

#Create some executable
add_executable(your_project some_source_file.cpp)

#Link it with your target
target_link_libraries(your_project PRIVATE sansic)
```
Or, linked with `g++` as so:
```
g++ some_source_file.cpp -lsansic
```
The public header `sansic.hpp` can then be `#include`-d in your source files.
