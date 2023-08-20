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

#ifndef RGM_H_SENTRY
#define RGM_H_SENTRY

#include <stdlib.h>

struct _RoboPair
{
	int type;
	double length;
	double vector;
	double matrix;
	struct _RoboPair *next;
};

typedef struct _RoboPair RoboPair;

struct _QueueOfRoboPair
{
	int num;
	struct _RoboPair *base, *tcp;
};

typedef struct _QueueOfRoboPair QueueOfRoboPair;

typedef struct
{
	int nump;
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtx;
} Robot;

void queue_robopair_init(QueueOfRoboPair *queue);

void queue_robopair_put(QueueOfRoboPair *queue);

void queue_robopair_get(QueueOfRoboPair *queue);

int queue_robopair_is_empty(QueueOfRoboPair *queue);

#endif
