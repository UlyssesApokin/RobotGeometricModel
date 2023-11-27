/*
Copyright (c) 2023 Ulysses Apokin (Jelisej Apokin)

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
WITH THE SOFTWARE OR THE USE OR OTHER DEALING2S IN THE SOFTWARE.
*/

#ifndef RGM3_REVERSE_KINEMATIC_H_SENTRY
#define RGM3_REVERSE_KINEMATIC_H_SENTRY
#define PI 3.1415926535

/*
 * 
 * name: is_limit_reached
 * @param
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @q_limit - Array of constraints for kinematic pairs
 *   @q - Array of kinematic pairs values	
 * @return
 *   Returns true if the limit is reached
 *   Returns false if the limit is not reached
 */
int is_limit_reached(int q_num, const double* q_limit, const double *q);

/*
 * 
 * name: avoid_position_limiter
 * @param
 *   @f - An array of functions that calculate the forward kinematics problem
 *   @final - Final position matrix
 *   @count_of_pairs - Number of kinematic pairs of the robot
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @q_limit - Array of constraints for kinematic pairs
 *   @q - Array of kinematic pairs values
 * @return
 *   Returns the furthest position from the final position (by orientation)
 */
double avoid_position_limiter(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q);

/*
 * 
 * name: avoid_orientation_limiter
 * @param
 *   @f - An array of functions that calculate the forward kinematics problem
 *   @final - Final position matrix
 *   @count_of_pairs - Number of kinematic pairs of the robot
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @q_limit - Array of constraints for kinematic pairs
 *   @q - Array of kinematic pairs values
 * @return
 *   Returns the furthest position from the final position (by position)
 */
double avoid_orientation_limiter(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q);

/*
 * 
 * name: do_iter_step_position
 * @param
 *   @f - An array of functions that calculate the forward kinematics problem
 *   @final - Final position matrix
 *   @count_of_pairs - Number of kinematic pairs of the robot
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @delta - An array that contains the values of coordinate increments
 *   @q - Array of kinematic pairs values
 * @return
 *   Returns the incremented value of the generalized coordinate (by position)
 *   If the final position could not be approached,
 *     returns the current position of the generalized coordinate
 */
double do_iter_step_position(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q);

/*
 * 
 * name: do_iter_step_orientation
 * @param
 *   @f - An array of functions that calculate the forward kinematics problem
 *   @final - Final position matrix
 *   @count_of_pairs - Number of kinematic pairs of the robot
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @delta - An array that contains the values of coordinate increments
 *   @q - Array of kinematic pairs values
 * @return
 *  Returns the incremented value of the generalized coordinate (by orientation)
 *  If the final position could not be approached,
 *     returns the current position of the generalized coordinate
 */
double do_iter_step_orientation(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q);

/*
 * 
 * name: set_iteration_step
 * @param
 *   @q_num - Number of the kinematic pair being checked (starting from 0)
 *   @q_limit - Array of constraints for kinematic pairs
 *   @division - Divisor that determines the discreteness of the iteration
 * @return
 *   Returns the value of one iteration step
 */
double set_iteration_step(int q_num, const double *q_limit,
	unsigned int division);

/*
 * 
 * name: set_simple_iteration_step
 * @param
 *   @gap - Range of change of generalized coordinate
 *   @division - Divisor that determines the discreteness of the iteration
 * @return
 *   Returns the value of one iteration step
 */
double set_simple_iteration_step(double gap, unsigned int division);

/*
 * 
 * name: get_tcp_matrix
 * @param
 *   @f - An array of functions that calculate the forward kinematics problem
 *   @q - Array of kinematic pairs values
 * @return
 *   Returns a 16-element dynamic array describing the robot's position
 *   (Don't forget to free up memory)
 */
double* get_tcp_matrix(double(**f)(const double*), double *q);

/*
 * 
 * name: get_absolute_error_matrix
 * @param
 *   @tmatrix - Current position matrix
 *   @final - Final position matrix
 * @return
 *   Returns a 16-element dynamic array describing the robot's error position
 *   (Don't forget to free up memory)
 */
double* get_absolute_error_matrix(const double *tmatrix, const double *final);

/*
 * 
 * name: get_relative_error_matrix
 * @param
 *   @tmatrix - Current position matrix
 *   @final - Final position matrix
 * @return
 *   Returns a 16-element dynamic array describing the robot's error position
 *   (by percent)
 *   (Don't forget to free up memory)
 */
double* get_relative_error_matrix(const double *tmatrix, const double *final);

/*
 * 
 * name: get_max_error_of_position
 * @param
 *   @errmatrix - An array describing the robot's error position
 * @return
 *   Returns the maximum error of position
 */
double get_max_error_of_position(double *errmatrix);

/*
 * 
 * name: get_average_error_of_position
 * @param
 *   @errmatrix - An array describing the robot's error position
 * @return
 *   Returns the average error of position
 */
double get_average_error_of_position(double *errmatrix);

/*
 * 
 * name: get_max_error_of_orientaion
 * @param
 *   @errmatrix - An array describing the robot's error orientaion
 * @return
 *   Returns the maximum error of orientaion
 */
double get_max_error_of_orientaion(double *errmatrix);

/*
 * 
 * name: get_average_error_of_orientaion
 * @param
 *   @errmatrix - An array describing the robot's error position
 * @return
 *   Returns the average error of orientaion
 */
double get_average_error_of_orientaion(double *errmatrix);

#endif
