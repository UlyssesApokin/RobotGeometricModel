# What is this?

## EN

Method for constructing geometric models of manipulation systems of industrial robots and multi-axis machines.


One of the most important sections of computer modeling of mechatronic systems is the development of control systems for multi-axis robotic manipulators.
The main purpose of a robotic manipulator is to move the working element in space along a given trajectory.
Therefore, among the systems that make up such a robot, the executive one is of particular importance.


Due to their flexible configurability, manipulation robots are capable of performing a wide range of various tasks in industrial production conditions without the need to purchase expensive specialized equipment, which leads to cost savings in the long term.
This causes high demand for such systems from industry, which confirms the relevance of the problem.


The purpose of the course work is to develop methods for constructing and analyzing mathematical models of manipulation systems that connect the generalized coordinates of the robot and the Cartesian coordinates of the characteristic point on the third link _(TCP~--- Tool Center Point)_.


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

* **/RobotGeometricModel**
	* ../docs<sup>documentation<\sup>
		* _frame-content-sheet-gost2104-68.svg_<sup>formalization<\sup>
		* _frame-main-sheet-gost2104-68.svg_<sup>formalization<\sup>
		* _frame-titul-sheet-gost2104-68.svg_<sup>formalization<\sup>
		* _kurs.tex_<sup>specification<\sup>
		* _makepdf.sh_<sup>makepdf<\sup>
		* _rgm_T2_S_T1.dxf_<sup>scheme<\sup>
		* _rgm_T2_S_T1.dxf~_<sup>scheme<\sup>
	* ../source<sup>source code and examples<\sup>
		* _opengl_rgm.c_<sup>opengl_rgm<\sup>
		* _rgm2.c_<sup>rgm2<\sup>
		* _rgm_fifo.c_<sup>rgm2_header<\sup>
		* _rgm_fifo.h_<sup>rgm2_header<\sup>
		* _rgm_file.c_<sup>rgm2_header<\sup>
		* _rgm_file.h_<sup>rgm2_header<\sup>
		* _robot1.rgm_<sup>example<\sup>
		* _robot2.rgm_<sup>example<\sup>


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
