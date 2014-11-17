3D-Asteroids
============

3D Game Engine

This project is for class CS 525 at DigiPen Institute of Technology. It is a simple implementation of a 3D version of Asteroids. It uses CUDA for GPGPU programming and OpenGL for hardware acceleration.


IMPORTANT!!

This project utilizes CUDA and requires an Nvidia GPU and that the CUDA SDK is installed into Visual Studio.
It targets the CUDA 5.5 build customization for .cu files.



MAKE SURE YOU HAVE THESE REQUIREMENT MET, OTHERWISE YOU CAN'T BUILD THE PROJECT!

- Before beginning, make sure you have an Nvidia GPU, any modern card should do. Make sure the card supports at least compute capability 2.0.
 
- Make sure the CUDA SDK is installed. To verify, in Visual Studio, you should have the option of creating a CUDA runtime project instead of a typical Win32 app. You should also be able to select CUDA 5.5 in the build customization options in Visual Studio.


STEP BY STEP GUIDE TO RUNNING PROJECT.
-Download project solution folder.
-Open the solution, I use VS 2012 and 2013, but 2012 should work fine.
-Build the solution
-Copy the dll's into either the Debug or Release folder.
-If you run the .exe from the Debug or Release folder, copy the Object, Shaders and Texture folders from Win32Projec1/Win32Project1/ into either the Debug or Release.
-If you run from within VS, just build, copy dll's into folder, and then run!


Dependencies
- GLM for math
- DevIL for image loading
- CUDA for GPGPU
- GLEW for extention loading

HIGHLIGHTS

One of the highlights is that I use CUDA to compute my Sphere meshs of varying dimensions. I also leverage OpenGL/CUDA interoperability which means that I map OpenGL memory, Vertex Buffer Objects (VBO's) into CUDA memory space so that the GPU computes the data locally on the GPU and it stays on the GPU. There are no memory transfers from RAM to VRAM! At this point I only create Spheres, but I will be adding many more. 








