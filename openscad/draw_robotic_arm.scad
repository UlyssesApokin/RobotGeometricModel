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

/*###################################################################
			I M P L E M E N T A T I O N
		Don't change it unless necessary!
###################################################################*/

//Colors of rendered objects
//https://get-color.ru/pastel/
tcp_pair_color = "#E4717A";
turning_pair_color = "#FCE883";
sliding_pair_color = "#FCE883";
link_color = "#F5F5DC";
link_alpha = 0.3;
//https://get-color.ru/rich/
x_axis_color = "#FF4040";
y_axis_color = "#00A86B";
z_axis_color = "#082567";
//size axis name label
text_size = 2;
//offset axis name label
offset_text = 2;

//Dimensions of rendered objects
hpair = 10; //height of the kinematic pair
rpair = 3; //radius of the kinematic pair
wpair = sqrt(PI * rpair ^ 2); //width of the kinematic pair
rlink = 1; //radius of the link between kinematic pair
raxis = 0.5; //radius of the axes in the system of axes

/*
function cast_Rmatrix:
--Multiplies two rotation matrix--
@_b = this is a matrix.
		The zero element of the vector is M(1,1).
		The eighth element of the vector is M(3,3).
@_c = this is a matrix.
		The zero element of the vector is M(1,1).
		The eighth element of the vector is M(3,3).
*/
function cast_Rmatrix(
	_b = [1, 0, 0,
		0, 1, 0,
		0, 0, 1],
	_c = [1, 0, 0,
		0, 1, 0,
		0, 0, 1]
) = 
[
	_b[0] * _c[0] + _b[1] * _c[3] + _b[2] * _c[6],
	_b[0] * _c[1] + _b[1] * _c[4] + _b[2] * _c[7],
	_b[0] * _c[2] + _b[1] * _c[5] + _b[2] * _c[8],
	_b[3] * _c[0] + _b[4] * _c[3] + _b[5] * _c[6],
	_b[3] * _c[1] + _b[4] * _c[4] + _b[5] * _c[7],
	_b[3] * _c[2] + _b[4] * _c[5] + _b[5] * _c[8],
	_b[6] * _c[0] + _b[7] * _c[3] + _b[8] * _c[6],
	_b[6] * _c[1] + _b[7] * _c[4] + _b[8] * _c[7],
	_b[6] * _c[2] + _b[7] * _c[5] + _b[8] * _c[8]
];

/*
function cast_3Dvector:
--Multiply base matrix by vector of position--
@_m = this is a matrix.
		The zero element of the vector is M(1,1).
		The eighth element of the vector is M(3,3).
@_v = this is a 3D vector 
*/
function cast_3Dvector(
	_m = [1, 0, 0,
		0, 1, 0,
		0, 0, 1],
	_v = [0, 0, 0]
) =
[
	_m[0] * _v[0] + _m[1] * _v[1] + _m[2] * _v[2],
	_m[3] * _v[0] + _m[4] * _v[1] + _m[5] * _v[2],
	_m[6] * _v[0] + _m[7] * _v[1] + _m[8] * _v[2]
];

//Convert radian to degrees
function rad2deg(_rad) = (_rad * 180) / PI;

//Convert combine rotation matrix 
//as a Product of Three Rotation Matrices Rz*Ry*Rx
//to Euler angles
function Rmatrix2EulerAngle(mtxeul) =
(mtxeul[6] < 1) ?
(
	(mtxeul[6] > -1) ?
		[atan2(mtxeul[7], mtxeul[8]),
		asin(-mtxeul[6]), atan2(mtxeul[3],
		mtxeul[0])]
	:
		[0, PI/2, -atan2(-mtxeul[5], mtxeul[4])]
)
:
	[0, -PI/2, atan2(-mtxeul[5], mtxeul[4])];

/*
module pair:
--Draws a kinematic pair--
@pair_type
	= "turning" for turning kinematic pair
		of the first type or the second
	= "sliding" for the sliding kinematic pair
	= "tcp" for the tool center point (last kinematic pair"
@position = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
@orientation = this is the rotation matrix
		of the current link relative to the zero link.
		The zero element of the vector is M(1,1).
		The eighth element of the vector is M(3,3).
*/
module pair(
	pair_type = "turning",
	position = [0,0,0],
	orientation =
		[1,0,0,
		0,1,0,
		0,0,1]
	)
{
	module system_of_axes(pos, ort)
	{
		module axis (pos, ort, p1, p2, p3, axcol)
		{
			module name_of_axis(pos, ort, axcol)
			{
				color(axcol)
				translate(pos)
				translate([
					ort[p1]*axis_scale,
					ort[p2]*axis_scale,
					ort[p3]*axis_scale
				])
				translate([offset_text, offset_text, offset_text])
				rotate(Rmatrix2EulerAngle(orientation))
				if (axcol == x_axis_color) {
					text("X", size = text_size);
				}
				else if (axcol == y_axis_color) {
					text("Y", size = text_size);
				}
				else {
					text("Z", size = text_size);
				}
			};
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
				name_of_axis(pos, ort, axcol);
		};
		//X-axis
		axis(pos, ort, 0, 3, 6, x_axis_color);
		//Y-axis
		axis(pos, ort, 1, 4, 7, y_axis_color);
		//Z-axis
		axis(pos, ort, 2, 5, 8, z_axis_color);
	};
	translate(position)
	rotate(Rmatrix2EulerAngle(orientation))
	echo("ATAN2:", Rmatrix2EulerAngle(orientation))
	if (pair_type == "turning") {
		color(turning_pair_color)
		cylinder(h = hpair, r = rpair, center = true);
	}
	else if (pair_type == "sliding") {
		color(sliding_pair_color)
		cube([wpair, wpair, hpair], center = true);
	}
	else if (pair_type == "tcpz") {
		rotate([0, 0, 0])
		color(tcp_pair_color)
		cylinder(h = hpair, r1 = rpair/2, r2 = rpair*1.5);
	}
	else {
		assert(false, "undefined sequence");
	}
	system_of_axes(position, orientation);
};	

/*
module link:
--Draws connections between kinematic pairs--
@pos1 = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
		of the first kinematic pair
@pos2 = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
		of the second kinematic pair
*/
module link(pos1 = 0, pos2 = 0) {
	color(link_color, link_alpha)
	hull() {
		translate(pos1)
		sphere(r = rlink);
		translate(pos2)
		sphere(r = rlink);
	};
};

/*
module extended_pair:
--Draws a kinematic pair--
@base_cs = orientation of the robot arm base coordinate system 
		in the standard OpenSCAD coordinate system.
@pair_type
	= "turning" for turning kinematic pair
		of the first type or the second
	= "sliding" for the sliding kinematic pair
	= "tcp" for the tool center point (last kinematic pair"
@position = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
@orientation = this is the rotation matrix
		of the current link relative to the zero link.
		The zero element of the vector is M(1,1).
		The eighth element of the vector is M(3,3).
*/
module extended_pair(
	base_cs =
		[1, 0, 0,
		0, 1, 0, 
		0, 0, 1],
	pair_type = "turing", 
	position =
		[0,
		0,
		0],
	orientation =
		[1, 0, 0,
		0, 1, 0, 
		0, 0, 1]
)
{
	base_position = cast_3Dvector(base_cs, position);
	base_orientation = cast_Rmatrix(base_cs, orientation);
	pair(pair_type, base_position, base_orientation);
};

/*
module extended_link:
--Draws connections between kinematic pairs--
@base_cs = orientation of the robot arm base coordinate system 
		in the standard OpenSCAD coordinate system.
@pos1 = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
		of the first kinematic pair
@pos2 = this is a 3D vector (last column
		of the homogeneous coordinate transformation matrix)
		of the second kinematic pair
*/
module extended_link(
	base_cs =
		[1, 0, 0,
		0, 1, 0, 
		0, 0, 1],
	pos1 = 0,
	pos2 = 0
)
{
	base_pos1 = cast_3Dvector(base_cs, pos1);
	base_pos2 = cast_3Dvector(base_cs, pos2);
	link(base_pos1, base_pos2);
};