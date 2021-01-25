# Circles packaging simulation
## Description
The application simulates a process of sequentially filling the limited space of a two-dimensional box using two-dimensional circles of different diameters. The diameter of each new circle is chosen randomly, considering the probability of its occurrence. The search for a new place in the package for the generated circle (i.e., the coordinates of its center) is carried out using the *particle swarm optimization* (PSO) searching algorithm with a certain set of restrictions:

1. The currently parking circle must not intersect with any other circle in the box.
2. The currently parking circle must not go beyond any boundaries of the box.

The searching algorithm successively tries to find such a position of the circle center *O* at which the parameter *R* is minimized, which is equal to the sum of the distances from the center of the circle to the bottom point of the box *T* and from the center of the circle to some central point at the bottom of the box *P*, considering the restrictions above. This can be shown schematically in Fig. 1.

<img src="/img/schematic_view.png" alt="Schematic view" width=50%>

**Fig. 1.** Representation of the process of searching the optimal place of currently parking circle in the box.

When the application is started, a graphic window opens with auxiliary information displayed in the upper part and a two-dimensional box painted below. The process of circle packaging is shown in Fig. 2, which ends when the box is completely full. The counted number of already packed circles is displayed at the top of the graphics window.

The application is written in С using Win32API. It can successfully be compiled with the Mingw-w64 – GCC and run under Windows (version 7 and higher).

<img src="/img/packing_process_1.png" alt="Packing process 1.png" width=25%>&nbsp;&nbsp;&nbsp;<img src="/img/packing_process_2.png" alt="Packing process 2.png" width=25%>&nbsp;&nbsp;&nbsp;<img src="/img/packing_process_3.png" alt="Packing process 3.png" width=25%>

**Fig. 2.** Stages of circle packaging process.

## Documentstion
When application is running, the process of circle packaging can be controlled using the keyboard as it is indicated by the information at the top of the graphic window:

`Key 1`&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start the circle packaging process. Also starts the new packaging process when the previous one was finished.<br>
`Key 2`&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pause the circle packaging process (to continue `Key 1` should be pressed).

At the current stage of the application development, the probability of a circle with a particular diameter and color to appear is hard-coded. The following lines in the file `src/simulation.c` are responsible for this:

```c
/* Number of different types of circles */
#define CIRCLES_TYPES 5

/* Circles colors */
static const u32 CIRCLES_COL[CIRCLES_TYPES] = {0x6daf43, 0x4870c8, 0xea8036, 0xaa1036, 0xaaff36};

/* Probabilities of appearance */
static const f32 CIRCLES_PROB[CIRCLES_TYPES] = {0.20f, 0.20f, 0.20f, 0.20f, 0.20f};

/* Circles radiuses in pixels */
static const u32 CIRCLES_RADIUSES[CIRCLES_TYPES] = {3, 5, 3, 5, 3};
```

## Install from source
Installation, compilation and run of the application occurs when executing commands in a terminal window (The method is described for a UNIX-like terminal, for example, Bash from MSYS2. It also requires a working Mingw-w64 - GCC compiler and configured paths).

-	Copying the repository to a local directory:
```
$ git clone https://github.com/juvus/Circles_packaging_simulation.git
$ cd Circles_packaging_simulation/
```
-	Compiling of the application:
```
$ mingw32-make
```
-	Running the application:
```
$ cd build/
$ ./Program
```

## Project Organization
- <img src="/img/folder.png" alt="folder" width=18px> `include/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Public header files
- <img src="/img/folder.png" alt="folder" width=18px> `data/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Files necessary for application
- <img src="/img/folder.png" alt="folder" width=18px> `src/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Source code files
- <img src="/img/folder.png" alt="folder" width=18px> `img/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Images, necessary for the documentation

## Licence
Circles packaging simulation code in this project is available under the `GPLv3` license. You can find the license file here: [LICENSE](/LICENSE)