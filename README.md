# Sci-Fi Pong

The classic pong game created from scratch using OpenGL and C. 
This project includes a 2D renderer with texture loading capabilities. 
The engine code is abstracted from the game code. game.c file contains all the scripting logic relative to pong game.

# POC

https://github.com/user-attachments/assets/89767106-30e0-4412-b33d-4bb051055f2a

# Compile

Below command will generate a visual studio project.
```
git clone https://github.com/satwkk/Pong.git
cd Pong
cmake . -G "Visual Studio 17 2022"
```

If you want to compile it without visual studio - 
1. Run vcvars64.bat batch file to load into visual studio developer console
2. Go to game directory and run the build.bat script.
3. Run game.exe !!

# Dependencies

1. cglm
2. GLFW
3. GLEW
