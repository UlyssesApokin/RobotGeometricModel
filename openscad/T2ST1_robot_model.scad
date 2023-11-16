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

function rad2deg(rad) = (rad * 180) / PI;

//Colors of rendered objects
//https://get-color.ru/pastel/
tcp_pair_color = "#E4717A";
turning_pair_color = "#FCE883";
sliding_pair_color = "#FCE883";
link_color = "#99FF99";
//https://get-color.ru/rich/
x_axis_color = "#FF4040";
y_axis_color = "#50C878";
z_axis_color = "#082567";

//Dimensions of rendered objects
hpair = 10;
rpair = 3;
wpair = sqrt(PI * rpair ^ 2);
rlink = 1;
raxis = 0.5;

module system_of_axes(pos, ort)
{
	module axis (pos, ort, p1, p2, p3, axcol)
	{
		axis_scale = 15;
		color(axcol)
		hull() {
			//1th point
			translate(pos)
			sphere(r = raxis);
			//2nd point
			translate(pos)
			translate([
				ort[p1]*axis_scale,
				ort[p2]*axis_scale,
				ort[p3]*axis_scale
			])
			sphere(r = raxis);
		};
	};
	//X-axis
	axis(pos, ort, 0, 3, 6, x_axis_color);
	//Y-axis
	axis(pos, ort, 1, 4, 7, y_axis_color);
	//Z-axis
	axis(pos, ort, 2, 5, 8, z_axis_color);
};

module pair(
	pair_type = "turning",
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	translate(position)
	rotate([
		atan2(orientation[7], orientation[8]),
		atan2(-orientation[6], sqrt((orientation[7])^2 + (orientation[8])^2)),
		atan2(orientation[3], orientation[0])
	])
	if (pair_type == "turning") {
		color(turning_pair_color)
		cylinder(h = hpair, r = rpair, center = true);
	}
	else if (pair_type == "sliding") {
		color(sliding_pair_color)
		cube([wpair, wpair, hpair], center = true);
	}
	else if (pair_type == "tcp") {
		color(tcp_pair_color)
		rotate([0, 90, 0])
		cylinder(h = hpair, r1 = rpair/2, r2 = rpair*1.5);
	}
	else {
		assert(false, "undefined sequence");
	}
	system_of_axes(position, orientation);
};

module link(pos1 = 0, pos2 = 0) {
	color(link_color, 0.3)
	hull() {
		translate(pos1)
		sphere(r = rlink);
		translate(pos2)
		sphere(r = rlink);
	};
};

//ROBOT_GEOMETRIC_MODEL

//Robotic arm dimensions
L1 = 60; //centimeter
L2 = 50; //centimeter
L3 = 25; //centimeter
Theta1 = rad2deg(-PI/4); //radian
D2 = 5*sin(rad2deg(-20)); //centimeter
Theta3 = rad2deg(PI/2); //radian

//Kinematic pair Turning type 2
function position_pair_0() = [
	0,
	0,
	0
];
pair("turning", position_pair_0());

//Kinematic pair Sliding
function position_pair_1() = [
	0,
	0,
	L1
];
function orientation_pair_1() = [
	cos(Theta1), //0
	-sin(Theta1), //1
	0, //2
	sin(Theta1), //3
	cos(Theta1), //4
	0, //5
	0, //6
	0, //7
	1 //8
];
pair("sliding", position_pair_1(), orientation_pair_1());

//Kinematic pair Turning type 1
function position_pair_2() = [
	0,
	0,
	D2 + L1 + L2
];
function orientation_pair_2() = [
	-cos(Theta1), //0
	0, //1
	-sin(Theta1), //2
	-sin(Theta1), //3
	0, //4
	cos(Theta1), //5
	0, //6
	1, //7
	0 //8
];
pair("turning", position_pair_2(), orientation_pair_2());

//Tool Center Point
function position_tcp() = [
	L3 * cos(Theta1) * sin(Theta3),
	L3 * sin(Theta1) * sin(Theta3),
	L3 * cos(Theta3) + D2 + L1 + L2
];
function orientation_tcp() = [
	cos(Theta1) * sin(Theta3), //0
	cos(Theta1) * cos(Theta3), //1
	-sin(Theta1), //2
	sin(Theta1) * sin(Theta3), //3
	sin(Theta1) * cos(Theta3), //4
	cos(Theta1), //5
	cos(Theta3), //6
	-sin(Theta3), //7
	0 //8
];
pair("tcp", position_tcp(), orientation_tcp());

link(position_pair_0(), position_pair_1());
link(position_pair_1(), position_pair_2());
link(position_pair_2(), position_tcp());