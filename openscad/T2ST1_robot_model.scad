/*
Copyright (c) <2023> <Ulysses Apokin (Jelisej Apokin)>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

use <draw_robotic_arm.scad>

/*###################################################################
 E X A M P L E   O F  T H E  R O B O T  G E O M E T R I C  M O D E L
						variable part
###################################################################*/

//Robotic arm dimensions
L1 = 60; //length 1 pair
L2 = 50; //length 2 pair
L3 = 25; //length 3 pair 
Theta1 = rad2deg(0); //generalized coordinate 1 pair
D2 = 0; //(+-30)generalized coordinate 2 pair
Theta3 = rad2deg(PI/2); //(+-5*PI/6)generalized coordinate 3 pair
/*
Orientation of the robot arm base coordinate system 
in the standard OpenSCAD coordinate system.
*/
Base = [-1, 0, 0,
		0, -1, 0,
		0, 0, 1];
A01 = [ cos(Theta1), -sin(Theta1), 0,
		sin(Theta1), cos(Theta1), 0,
		0, 0, 1 ];
A02 = [ -cos(Theta1), 0, -sin(Theta1),
		-sin(Theta1), 0, cos(Theta1),
		0, 1, 0 ];
T = [ cos(Theta1)*sin(Theta3), cos(Theta1)*cos(Theta3), -sin(Theta1),
		sin(Theta1)*sin(Theta3), sin(Theta1)*cos(Theta3), cos(Theta1),
		cos(Theta3), -sin(Theta3), 0 ];
P_Base = [0, 0, 0];
P1 = [0, 0, L1];
P2 = [0, 0, D2 + L1 + L2];
P_TCP = [L3 * cos(Theta1) * sin(Theta3),
		L3 * sin(Theta1) * sin(Theta3),
		L3 * cos(Theta3) + D2 + L1 + L2];

extended_pair(Base, "turning", P_Base);
extended_pair(Base, "sliding", P1, A01);
extended_pair(Base, "turning", P2, A02);
extended_pair(Base, "tcpx", P_TCP, T);
tcp_term_output(T, P_TCP);
extended_link(Base, P_Base, P1);
extended_link(Base, P1, P2);
extended_link(Base, P2, P_TCP);