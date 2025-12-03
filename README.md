# qt-cyberCalc

https://github.com/user-attachments/assets/47ece0f4-6d46-4cbf-b9e1-a9adf233b7cc

Futuristic calculator made with Qt6 widgets and C++. This project has been a mere exercise for demonstrating the usage of Qt framework. Feel free to use it as inspiration for your custom distro :) 

# Building project

First of all clone the repository with `git clone https://github.com/zkivo/qt-cyberCalc.git`.

Download Qt6.10.1 or compatible.

## Using Qt Creator 18.0.0
- Launch Qt Creator.
- Click in the top bar File->Open File or Project... and select the CMakeLists.txt
- From the software you should be able to build and run the project. Nonetheless, the software allows you to change building tools depending on your system.


## CMake + MinGW13.1.0
- Open MinGW13.1.0 terminal so that it has all the path to the building tools.
```
cd project
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64/lib/cmake"  <-- this path might be different depending where you installed Qt6
cmake --build .
qt-cyberCalc.exe
```
