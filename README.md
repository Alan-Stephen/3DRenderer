# 3D Renderer 

## Overview
This project is a custom 3D rendering engine built from scratch using OpenGL. It has been used to render a scene which displays the majority of it's capabilities.

## Features 

Here's an overview of all the feaures i implemented or utilised:
- Custom OBJ file format parser
- Advanced lighting techniques:
  - Point lights
  - Spotlights
  - Directional lights
- Dynamic shadows using shadow mapping
- Procedurally generated meshes (e.g., terrain using Perlin noise)
- Dynamic water simulation with vertex shaders
- Rudimentary buoyancy simulation (pitch, roll, and yaw)
- Pathing system using Catmull-Rom splines
- Instanced rendering
- Percentage-Closer Filtering (PCF)
- Cubemaps
- Transparency
- Texture mapping

## Prerequisites
- Visual Studio (Preferably 2017 or higher)

## Setup
1. Clone the repository:
   ```
   git clone https://github.com/Alan-Stephen/3DRenderer.git
   ```
2. Open the solution file (`Coursework.sln`) in Visual Studio. (all dependencies are included in the repo for convenience)
4. Build and run the solution in visual studio

## Usage
1. Run the compiled executable from Visual Studio or navigate to the build directory and run `3DRenderer.exe`.
2. WASD to move around, hold and drag mouse to pan camera, press E to switch the boat camera.
3. if you find yourself seeing a black screen it may because you're inside an object.

## Project Structure
- `Assessment2/` - Source code files
- `include/` - External Libraries
- `Assessment2/objs/` - 3D models, textures, and other assets

## Acknowledgments
- [Any libraries, resources, or individuals you want to credit]
