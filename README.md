# Traffic light simulator
## Description
The application simulates the operation of a single traffic light consisting of three sections (green, yellow and red) in *manual* and *automatic* modes. In manual mode any section can be tuned on and off arbitrarily. In automatic mode the traffic light works according to its standard program.
When the application is started, a graphic window opens (Fig. 1) in which the traffic light is programmatically drawn using the simple graphic primitives. The application is written in С using Win32API. It can successfully be compiled with the Mingw-w64 – GCC and run under Windows (7 and higher).

<img src="/img/traffic_light.png" alt="Traffic light view" width=30%>

**Fig.1.** Traffic light view with all sections turned off (left) and turned on (right).

## Documentstion
When the application is running, the traffic light can be controlled using the keyboard. Control occurs by pressing the following keys and their combinations:

`Ctrl + M` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Switch to the manual operation mode<br>
`Ctrl + A`	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Switch to the automatic operation mode

When switching to the automatic operation mode, the traffic light starts its work from the state “green light is on”. State diagram for the automatic mode is shown in Fig. 2. Duration of the states can be easily changed by modifying constants in `simulation.c` file. When switching to the manual operation mode, sections preserve their states that were before the switch. In this mode, sections are switched on and off as follows:

`F1` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Turning green section on/off<br>
`F2`	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Turning yellow section on/off<br>
`F3`	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Turning red section on/off

Regardless of the operation mode, pressing the `ESC` key terminates the application.

<img src="/img/state_diagram.png" alt="Traffic light state diagram" width=100%>

**Fig.2.** Set of possible states of the traffic light working in automation mode.

## Install from source
Installation, compilation and run of the application occurs when executing commands in a terminal window (The method is described for a UNIX-like terminal, for example, Bash from MSYS2. It also requires a working Mingw-w64 - GCC compiler and configured paths).

-	Copying the repository to a local directory:
```
$ git clone https://github.com/juvus/Traffic_light_simulator.git
$ cd Traffic_light_simulator/
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
- <img src="/img/folder.png" alt="folder" width=18px> `resources/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Application resources (Accelerator table)
- <img src="/img/folder.png" alt="folder" width=18px> `src/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Source code files
- <img src="/img/folder.png" alt="folder" width=18px> `img/` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Images, necessary for the documentation

## Licence
Traffic light simulator code in this project is available under the `GPLv3` license. You can find the license file here: [LICENSE](/LICENSE)
