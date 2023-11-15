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
blue_frost_col = "#AFDAFC";
pink_candy_col = "#E4717A";
yellow_crayola_col = "#FCE883";
summer_salad_col = "#99FF99";
//https://get-color.ru/rich/
dark_blue_col = "#082567";
green_col = "#50C878";
red_col = "#FF4040";

//Dimensions of rendered objects
hpair = 10;
rpair = 3;
wpair = sqrt(PI * rpair ^ 2);
rlink = 1;
raxis = 0.5;

module axis(pos, ort)
{
	axis_scale = 15;
	//X-axis
	color(red_col)
	hull() {
		//1th point
		translate(pos)
		sphere(r = raxis);
		//2nd point
		translate(pos)
		translate([
			ort[0]*axis_scale,
			ort[3]*axis_scale,
			ort[6]*axis_scale
		])
		sphere(r = raxis);
	};
	//Y-axis
	color(green_col)
	hull() {
		//1th point
		translate(pos)
		sphere(r = raxis);
		//2nd point
		translate(pos)
		translate([
			ort[1]*axis_scale,
			ort[4]*axis_scale,
			ort[7]*axis_scale
		])
		sphere(r = raxis);
	};
	//Z-axis
	color(dark_blue_col)
	hull() {
		//1th point
		translate(pos)
		sphere(r = raxis);
		//2nd point
		translate(pos)
		translate([
			ort[2]*axis_scale,
			ort[5]*axis_scale,
			ort[8]*axis_scale
		])
		sphere(r = raxis);
	};
};

module tool_center_point(
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	translate(position)
	color(pink_candy_col)
	sphere(r = rpair);
	axis(position, orientation);
};

module turning_pair_type_1(
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	translate(position)
	color(yellow_crayola_col)
	rotate([
		acos(orientation[0]),
		acos(orientation[4]),
		acos(orientation[8])
	])
	cylinder(h = hpair, r = rpair, center = true);
	axis(position, orientation);
};

module turning_pair_type_2(
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	translate(position)
	color(yellow_crayola_col)
	rotate([
		acos(orientation[0]),
		acos(orientation[4]),
		acos(orientation[8])
	])
	cylinder(h = hpair, r = rpair, center = true);
	axis(position, orientation);
};

module sliding_pair(
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	translate(position)
	color(yellow_crayola_col)
	rotate([
		acos(1),
		acos(1),
		acos(orientation[0])
	])
	cube([wpair, wpair, hpair], center = true);
	axis(position, orientation);
    echo(orientation[0]);
    echo(orientation[4]);
    echo(orientation[8]);
};

module link(pos1 = 0, pos2 = 0) {
	color(summer_salad_col, 0.3)
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
Theta1 = rad2deg(PI/3); //radian
D2 = -10; //centimeter
Theta3 = rad2deg(0); //radian

//Kinematic pair Turning type 2
function position_pair_0() = [
	0,
	0,
	0
];
turning_pair_type_2(position_pair_0());

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
sliding_pair(position_pair_1(), orientation_pair_1());

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
turning_pair_type_1(position_pair_2(), orientation_pair_2());

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
tool_center_point(position_tcp(), orientation_tcp());

link(position_pair_0(), position_pair_1());
link(position_pair_1(), position_pair_2());
link(position_pair_2(), position_tcp());