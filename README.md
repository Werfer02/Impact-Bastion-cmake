# Impact-Bastion
Gra na zaliczenie z Podstaw Informatyki

## Building

### Windows (Visual Studio)

#### 1. Clone the repo

```
git clone https://github.com/Werfer02/Impact-Bastion-cmake
cd Impact-Bastion-Cmake
```

#### 2. Generate buildfiles with Cmake
Make sure to specify the appropriate VS generator if you have not set it previously:

```
cmake -S . -B build -G "Visual Studio 18 2026"
```

#### 3. Build with cmake
Make sure to specify a build config (debug or release):

```
cmake --build build --config debug
```
The built executable along with the required resources will be located in build/(whatever config you chose)/  

If it seems that the build directory is not visible but you have not encountered any errors in the build process, it is likely because newer versions of Visual Studio automatically hide any files listed in the .gitignore.  
This can be disabled on most versions but if it can't on yours, you can just comment it out.

### Linux

#### 1. Clone the repo

```
git clone https://github.com/Werfer02/Impact-Bastion-cmake
cd Impact-Bastion-Cmake
```

#### 2. Generate buildfiles with Cmake

```
cmake -S . -B build
```

#### 3. Build with cmake

```
cmake --build build
```
The built executable along with the required resources will be located in build/

### Note for all builds

Make sure the executable is run inside the directory it's located in or it won't be able to access the necessary resources
