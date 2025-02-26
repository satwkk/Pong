# Pong

The classic pong game created from scratch using OpenGL and C. 
This project includes a 2D renderer with texture loading capabilities. 
The engine code is abstracted from the game code. game.c file contains all the scripting logic relative to pong game.

# Compile

Below command will generate a visual studio project.
```
git clone https://github.com/satwkk/Pong.git
cd Pong
cmake . -G "Visual Studio 17 2022"
```

If you want to compile it without visual studio - 
1. Add cl.exe to your environment path.
2. Open cmd.exe and go inside game directory.
3. Run the build.bat script, it will generate a game.exe file.

# Dependencies

1. cglm
2. GLFW
3. GLEW