# What is this?

## EN

Method for constructing geometric models of manipulation systems of industrial robots and multi-axis machines.


One of the most important sections of computer modeling of mechatronic systems is the development of control systems for multi-axis robotic manipulators.
The main purpose of a robotic manipulator is to move the working element in space along a given trajectory.
Therefore, among the systems that make up such a robot, the executive one is of particular importance.


Due to their flexible configurability, manipulation robots are capable of performing a wide range of various tasks in industrial production conditions without the need to purchase expensive specialized equipment, which leads to cost savings in the long term.
This causes high demand for such systems from industry, which confirms the relevance of the problem.


The purpose of the course work is to develop methods for constructing and analyzing mathematical models of manipulation systems that connect the generalized coordinates of the robot and the Cartesian coordinates of the characteristic point on the third link _(TCP -- Tool Center Point)_.


To achieve this goal, it is necessary to solve the following tasks:

* Analyze methods for solving the direct kinematics problem for multi-link robotic systems.
* Select the method that best suits the purpose of the work and justify the choice of a specific method.
* Select the kinematic diagram of the manipulation robot and, using the chosen method, build its kinematic model.
* Check the correctness of the model using a graphical-analytical method.
* Develop an application with a CLI interface that provides visualization of the kinematic diagram of a handling robot.

## RU

Метод для построения геометрических моделей манипуляционных систем промышленных роботов и многоосных машин.


Одним из важнейших разделов компьютерного моделирования мехатронных систем является разработка систем управления многозвенными роботами-манипуляторами.
Основное назначение робота-манипулятора состоит в перемещении рабочего органа в пространстве по заданной траектории.
Поэтому среди систем, входящих в состав такого робота, исполнительная имеет особое значение.


Манипуляционные роботы за счет гибкой конфигурируемости способны выполнять широкий круг всевозможных задач в условиях промышленного производства без необходимости закупки дорогостоящего специализированного оборудования, что ведет к экономии средств в перспективе.
Это вызывает высокий спрос на подобные системы со стороны промышленности, что подтверждает актуальность проблемы.


Целью курсовой работы является разработка методов построения и анализа математических моделей манипуляционных систем, связывающих обощенные координаты робота и декартовы координаты характерной точки на третьем звене _(TCP -- Tool Center Point)_.


Для достижения поставленной цели необходимо решить следующие задачи:

* Проанализировать методы решения прямой задачи кинематики для многозвенных робототехнических систем.
* Выбрать метод, наиболее удовлетворяющий цели работы, и обосновать выбор конкретного метода.
* Выбрать кинематическую схему манипуляционного робота и посредством избранного метода построить её кинематическую модель.
* Проверить правильность модели графо--аналитическим способом.
* Разработать приложение с CLI-интерфейсом, предоставляющее визуализацию кинематической схемы манипуляционного робота.

# How does the repository work?

## EN

The "source" directory contains the source codes for the **rgm2** and **opengl_rgm** programs.

* rgm2 is a program that reads information about the geometric characteristics of the robot and performs calculations on them, that is, builds a geometric model. Example configuration files for rgm2 are located in the files _robot1.rgm_ and _robot2.rgm_.

* opengl_rgm is a program that renders the geometric model of a robot using OpenGL and freeglut. Adding your model to this program is done by editing the source code of the program. At the moment this program is no longer being developed.

The files **rgm_fifo.c**, **rgm_fifo.h** and **rgm_file.c**, **rgm_file.h** are header files for the _rgm2_ program. The first two are responsible for organizing the data structure about the geometric model of the robot. The latter are responsible for reading and interpreting information from the configuration files _robot1.rgm_ and _robot2.rgm_.

The "docs" directory contains documentation on the modeling method and the source code of the program.
Only in Russian.
The file **kurs.tex** is a latex file according to the documentation.
Files **frame-content-sheet-gost2104-68.svg**, **frame-main-sheet-gost2104-68.svg**, **frame-titul-sheet-gost2104-68.svg** -- These are ESKD frames made in accordance with GOST 2.104-68.
The **makepdf.sh** file is designed to translate all of the above files into a single pdf file.
Don't forget to transfer svg to pdf.

## RU

В директории "source" расположены исходные тексты программ **rgm2** и **opengl_rgm**.

* rgm2 -- это программа, которая считывает информацию о геометрических характеристиках робота, и выполняет над ними вычисления, то есть строит геометрическую модель. Пример конфигурационных файлов для rgm2 расположены в файлах _robot1.rgm_ и _robot2.rgm_.

* opengl_rgm -- это программа, которая визуализирует геометрическую модель робота с помощью OpenGL и freeglut. Добавление своей модели в эту программу производится с помощью редактиривания исходных текстов программы. В данный момент эта программа больше не разваивается.

Файлы **rgm_fifo.c**, **rgm_fifo.h**  и **rgm_file.c**, **rgm_file.h** это заголовочные файлы для программы _rgm2_. Первые два отвечают за организацию  структуры данных о геометрической модели робота. Последние отвечают за чтение и интерпритацию информации из конфигурационных файлов _robot1.rgm_ и _robot2.rgm_.

В директории "docs" расположена документация по способу моделирования и исходным текстам программы.
Только на русском языке.
Файл **kurs.tex** это latex-файл по документации.
Файлы **frame-content-sheet-gost2104-68.svg**, **frame-main-sheet-gost2104-68.svg**, **frame-titul-sheet-gost2104-68.svg** -- это рамки ЕСКД, выполненные по ГОСТ 2.104-68.
Файл **makepdf.sh** предназначен для трансляции всех выше перечисленных файлов в единый pdf-файл.
Не забудьте перед этим svg транслировать в pdf с помощью inkscape или любого другого редактора.

# How to get?

Clone the repository to your machine:

````
$ git clone https://github.com/UlyssesApokin/RobotGeometricModel.git
````
Navigate to the source directory:

````
$ cd RobotGeometricModel/source
````

## Compile the opengl_rgm

**To compile you need the following packages:**

* gcc (any version in the distro)

* freeglut3-dev (+=3.0 version) _(The package name on your distro may vary)_

Run the command:

````
$ gcc -Wall -g -ansi opengl_rgm.c -lm -lGL -lglut -o opengl_rgm
````
## Compile the rgm2

**To compile you need the following packages:**

* gcc (any version in the distro)

Run the command:

````
$ gcc -Wall -g -ansi -c rgm_fifo.c && gcc -Wall -g -ansi -c rgm_file.c && gcc -Wall -g -ansi rgm2.c rgm_fifo.o rgm_file.o -o rgm2
````

# How to use?

````
$ ./rgm2 robot1.rgm
````

````
$ ./opengl_rgm
````
