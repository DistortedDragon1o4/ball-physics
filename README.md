# 2D Ball Physics

![GitHub license](https://img.shields.io/github/license/DistortedDragon1o4/ball-physics.svg) ![GitHub issues](https://img.shields.io/github/issues/DistortedDragon1o4/ball-physics.svg) ![GitHub forks](https://img.shields.io/github/forks/DistortedDragon1o4/ball-physics) ![GitHub stars](https://img.shields.io/github/stars/DistortedDragon1o4/ball-physics)

This is a free and opensource 2D physics simulation made by me just for testing purposes only.

:warning: 
This is for testing and experimentation only and I am not liable if you break anything while using this. Also, this project uses OpenGL 4.6.

## Downloading:


![GitHub release](https://img.shields.io/github/release/DistortedDragon1o4/ball-physics.svg)

You can download the executabled of GNU/Linux and Windows from the Github Releases page.

## Running:

### For GNU/Linux based systems:

Make sure your graphics drivers are up to date.

Open a Terminal and run `./2D_Ball_Physics` in the directory where you have kept it.

It will ask for the properties file, please provide it the path to the properties file accurately. (It should be in the build directory if you built it from source, or you can download it)

### For Windows based systems:

Download and install the glfw3.dll file from the releases page.

Run it from cmd or powershell.

It will ask for the properties file, please provide it the path to the properties file accurately. (You can download the file from the releases page, it will be out as a pre release version)

(I dont have a windows system so cant confirm if it works)

## Building from source:

### For GNU/Linux based systems:

#### Requirements:

This project uses make, and you are required to have it installed. Also you will need the following build dependencies:
- `libgl1-mesa-dev`
- `libglfw3-dev`
- `libglm-dev`
- There are more, just remind me to mention them before i release this

#### Building the project:

As of now, building is only supported on Linux.

To build the project, go to the root of the project:
`mkdir build`
`make all`

To run the project:
`make run`