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
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef RGM3_OUTPUT_H_SENTRY
#define RGM3_OUTPUT_H_SENTRY

void term_print_matrix(const char *subs, const double *matrix,
	int row, int col);

void term_print_error(int percent,
	double max_error_pos, double aver_error_pos,
	double max_error_ort, double aver_error_ort);

void term_print_gen_coord(int num_of_iter, int num_of_q, double *q);

void csv_init_file(const char *filename, int *file_descriptor);

void csv_close_file(int file_descriptor);

void csv_create_field(int file_descriptor, const char *label);

void csv_print_info(int file_descriptor, const char *label, double data);

#endif
