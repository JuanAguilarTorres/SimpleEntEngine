# SimpleEntEngine

SimpleEntEngine is a flexible multimedia engine that bridges the power of C++ with the simplicity of Lua scripting. Built on top of SDL2, it provides a robust foundation for creating games, interactive applications, and various media projects.

## Overview

## Features
- **Entity-Component-System (ECS) Architecture** – flexible and scalable structure for game and media projects.
- **Lua Integration** – scripting with assets separated from engine logic (written in C++).
- **Media Support** – handle images, videos, music, sound effects, and 3D models.
- **Built-in Components** – animations, colliders, camera, clickable, collision, properties, rigid body, script, text, transform, and more.
- **Lua Bindings for C++** – useful functions exposed for scripting, extendable with your own.
- **Engine Design Encourages Flyweight Pattern** – engine structure and functions promote using a flyweight approach in Lua scripts.
- **Scene Manager** – organize and switch between different parts of your project easily.
- **Customizable Engine** – works out-of-the-box but open for modifications to fit your needs.
- **Good Practices** – follows good practices such as SOLID principles and careful memory management.

## Index

- [Technical Overview](./resources/markdown/technical_overview.md)  
  Technical aspects of the project, including a UML diagram with the overall infrastructure of the game, design patterns, and other details.
  
- [Project Documentation](./resources/markdown/doxygen.md)  
  Access the full Doxygen-generated documentation for the codebase, including detailed descriptions of classes, functions, and dependencies.

- [Error Q&A](./resources/markdown/error.md)  
 If you have any error/glitch in your program, check this.

## Requirements

### Linux
To build and run Engine on Linux, you need the following dependencies:

1. **G++ (GNU C++ compiler)**
   - Make sure `g++` is installed on your system. You can install it with:
     ```bash
     sudo apt install g++
     ```

2. **SDL2 and Related Libraries such as LibTinyXML2**
   - The game relies on several SDL2 libraries. You can install them with:
     ```bash
     sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-gfx-dev libtinyxml2-dev
     ```
    - Note: Make sure to install SDL2_gfx, as its a newly added SDL2 library to this project, just like LibTinyXML2.

3. **Lua 5.3**
   - Install Lua 5.3 with:
     ```bash
     sudo apt install liblua5.3-dev
     ```

4. **FFmpeg Libraries**
   - You'll need FFmpeg libraries for video decoding. Install them with:
     ```bash
     sudo apt install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
     ```

5. **Linking Requirements**
   - When compiling, make sure to link the following libraries:
     ```
     -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -llua5.3 -lavcodec -lavformat -lavutil -lswscale -ltinyxml2
     ```

### Windows
To build and run Engine on Windows, you can use MinGW with similar requirements as Linux:

1. **MinGW Installation**
   - Make sure MinGW is installed and added to your system's PATH.
   - [Download MinGW](https://www.mingw-w64.org/) if not already installed.

2. **SDL2 and Related Libraries**
   - Download SDL2, SDL2_image, SDL2_ttf, SDL2_gfx, and SDL2_mixer development libraries for Windows and add the include and lib paths to your project configuration.
   - Libraries can be found on the [SDL website](https://www.libsdl.org/).
   - Use ```pacman -S mingw-w64-x86_64-SDL2_gfx``` to install the new SDL2 Library.
   - Use ```pacman -S mingw-w64-x86_64-tinyxml``` to install the new XML library.

3. **Lua 5.3**
   - Download Lua binaries for Windows or compile from source. Add Lua include and lib paths accordingly.

4. **FFmpeg Libraries**
   - Download the precompiled FFmpeg libraries for Windows, and ensure the include and lib directories are properly configured.

5. **Linking Requirements**
   - Link the following libraries during compilation:
     ```
     -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf  -lSDL2_mixer -lSDL2_gfx -llua53  -lavcodec -lavformat -lavutil -lswscale -ltinyxml2
     ```
### Required Common Tools
- **CMake or Make (Optional):** You may use CMake or Make for managing the build process.

Included in the source files, theres a `Makefile` file that works for both Linux and MinGW, so if you fulfill the requirements, please run Make in the engine folder.

## Licenses

This project is licensed under the **Zlib license**.

- [Licenses](./resources/markdown/licenses.md)  
Please contact me if there are any issues regarding credit or the licenses used in this project.

## Contributing

Whether it's bug reports, feature suggestions, or code improvements, your help is appreciated.

### How to Contribute
1. **Fork the Repository**
   - Click the "Fork" button at the top right of this repository page to create your own copy.

2. **Create a New Branch**
   - Use a descriptive name for your branch:
     ```bash
     git checkout -b feature/your-feature-name
     ```

3. **Make Changes**
   - Add your code, fix bugs, or improve documentation.

4. **Commit Your Changes**
   - Provide a clear and concise commit message:
     ```bash
     git commit -m "Add your description here"
     ```

5. **Push to Your Branch**
   - Push the changes to your forked repository:
     ```bash
     git push origin feature/your-feature-name
     ```

6. **Open a Pull Request**
   - Go to the original repository and open a pull request from your branch.

### Guidelines
- Follow a standardized coding style.
- Include comments and documentation for new features.
- Make sure your code builds and runs without errors.

## Acknowledgments

Special thanks to:
- **Professor Alberto Rojas** for his lessons in the "Creación de Videojuegos" course at the University of Costa Rica. These served as a foundation for developing the engine.
- **Sara Echeverria** for her GitHub repository 3dModelsFlatShading, which helped me better understand SDL2's 3D capabilities.