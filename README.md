# What is this?

![Visualization of a robot model using OpenSCAD](https://codeberg.org/UlyssesApokin/RobotGeometicModel/raw/main/T2ST1_prog_example.gif)


Method for constructing geometric models of manipulation systems of industrial robots and multi-axis machines.


One of the most important sections of computer modeling of mechatronic systems is the development of control systems for multi-axis robotic manipulators.
The main purpose of a robotic manipulator is to move the working element in space along a given trajectory.
Therefore, among the systems that make up such a robot, the executive one is of particular importance.


Due to their flexible configurability, manipulation robots are capable of performing a wide range of various tasks in industrial production conditions without the need to purchase expensive specialized equipment, which leads to cost savings in the long term.
This causes high demand for such systems from industry, which confirms the relevance of the problem.


The purpose of the course work is to develop methods for constructing and analyzing mathematical models of manipulation systems that connect the generalized coordinates of the robot and the Cartesian coordinates of the characteristic point on the third link _(TCP - Tool Center Point)_.


To achieve this goal, it is necessary to solve the following tasks:

* Analyze methods for solving the direct kinematics problem for multi-link robotic systems.
* Select the method that best suits the purpose of the work and justify the choice of a specific method.
* Select the kinematic diagram of the manipulation robot and, using the chosen method, build its kinematic model.
* Check the correctness of the model using a graphical-analytical method.
* Develop an application with a CLI interface that provides visualization of the kinematic diagram of a handling robot.

# Requirements

To compile the documentation into a pdf file, you will need the following programs or their equivalents (the names of the packages in your distro may vary):

* texlive-full (+= 2022.20230122-3)
* Inkscape (+= 1.2.2)
* LibreCad (+= 2.2.0)
* qpdf (+= 11.3.0)

To compile a program from source texts you will need (the names of the packages in your distro may vary):

* gcc (any version that supports C89/ANSI C)
* freeglut3-dev (+= 3.4.0)

To run a program written in MatLab you need to have the following package versions:

* GNU Octave (+= 8.3.0)

To run a program written in Maxima you need to have the following package versions:

* Maxima (+= 5.46.0)

# How does the repository work?

## Repository structure

* **/RobotGeometricModel**
	* ../docs <sup>documentation</sup>
		* _frame-content-sheet-gost2104-68.svg_ <sup>formalization</sup>
		* _frame-main-sheet-gost2104-68.svg_ <sup>formalization</sup>
		* _frame-titul-sheet-gost2104-68.svg_ <sup>formalization</sup>
		* _kurs.tex_ <sup>specification</sup>
		* _makepdf.sh_ <sup>makepdf</sup>
		* _rgm_T2_S_T1.dxf_ <sup>scheme</sup>
		* _rgm_T2_S_T1.dxf~_ <sup>scheme</sup>
	* ../source <sup>source code and examples</sup>
		* _opengl_rgm.c_ <sup>opengl_rgm</sup>
		* _rgm2.c_ <sup>rgm2</sup>
		* _rgm_fifo.c_ <sup>rgm2_header</sup>
		* _rgm_fifo.h_ <sup>rgm2_header</sup>
		* _rgm_file.c_ <sup>rgm2_header</sup>
		* _rgm_file.h_ <sup>rgm2_header</sup>
		* _robot1.rgm_ <sup>example</sup>
		* _robot2.rgm_ <sup>example</sup>
	* ../octave <sup>matlab</sup>
		* _crDHmatrix.m_ <sup>matlab</sup>
		* _rgm.m_ <sup>matlab</sup>
	* ../maxima <sup>maxima</sup>
		* _rgm.maxima_ <sup>maxima</sup>

## Description

* documentation - This is the directory where the files are located that are used to generate specifications for the project’s source code and the modeling methods used.
* formalization - This is a page frame made in accordance with GOST 2.104-68. To use this one, you need to stream it to pdf. For example, using Inkscape.
* specification - This is the TeX file that contains the specification. It describes the method used to model the robot. It also describes the software development process and comments on the source code of this software. _(in progress! only Russian!)_
* makepdf - This is the file for assembling the specification into a pdf file. To run the script, install the qpdf console utility.
* scheme - This is the robot kinematic diagram that is used in the specification. The file was created in LibreCad. In order for this file to be used in the specification, it must be translated into a pdf file. For example, using LibreCad.

* source code and examples - This is the directory in which the source codes of programs and examples are located.
* opengl_rgm - is a program that provides visualization of a robot's kinematic diagram using OpenGL and freeGlut. Adding your own for visualization is possible only by editing the source text of the program. _(the program is no longer being developed!)_
* rgm2 - this is a program that builds a geometric model of a robot using a configuration file that it receives as input. _(in progress!)_
* rgm2_header - these are split compilation files that are used to compile the rgm2 program.
* example - these are examples of configuration files for rgm2, which contain a text description of the geometric characteristics of the robot.

* octave - this is a directory for prototyping a program in MatLab using the free GNU Octave software.
* maxima - this is the directory that stores the program written for Maxima-CAS (Computer Algebra System).

# License

The source code for software contained in this repository is licensed under the MIT License unless otherwise noted in the file preamble.


Any other files, including documentation, vector graphics files, etc., are distributed without any license. The author disclaims rights and liability for these documents. With the exception of those files in the preamble of which it is indicated otherwise. For example, in the file kurs.tex

# How to get?

Clone the repository to your machine:

````
$ git clone https://github.com/UlyssesApokin/RobotGeometricModel.git
````


## Compile the opengl_rgm

**To compile you need the following packages:**

* gcc (any version that supports C89/ANSI C)
* freeglut3-dev (+= 3.4.0)

Navigate to the source directory:

````
$ cd RobotGeometricModel/source
````

Run the command:

````
$ gcc -Wall -g -ansi opengl_rgm.c -lm -lGL -lglut -o opengl_rgm
````

## Compile the rgm2

**To compile you need the following packages:**

* gcc (any version that supports C89/ANSI C)

Navigate to the source directory:

````
$ cd RobotGeometricModel/source
````

Run the command:

````
$ gcc -Wall -g -ansi -c rgm_fifo.c && gcc -Wall -g -ansi -c rgm_file.c && gcc -Wall -g -ansi rgm2.c rgm_fifo.o rgm_file.o -o rgm2
````

# Make the documentation

**To compile you need the following packages:**

* texlive-full (+= 2022.20230122-3)
* Inkscape (+= 1.2.2)
* LibreCad (+= 2.2.0)
* qpdf (+= 11.3.0)

Navigate to the docs directory:

````
$ cd RobotGeometricModel/docs
````

Run the command:

````
$ ./makepdf.sh full
````

And again (LaTeX generates a table of contents):

````
$ ./makepdf.sh tex
````

# How to use?

````
$ ./rgm2 robot1.rgm
````

````
$ ./opengl_rgm
````
