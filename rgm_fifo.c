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

#include "rgm_fifo.h"

void queue_robopair_init(QueueOfRoboPair *queue)
{
	queue->base = NULL;
	queue->tcp = NULL;
}

void queue_robopair_create(QueueOfRoboPair *queue)
{
	if (!queue->base)
	{
		queue->base = malloc(sizeof(RoboPair));
		queue->tcp = queue->base;
	}
	else
	{
		queue->tcp->next = malloc(sizeof(RoboPair));
		queue->tcp = queue->tcp->next;
	}
	queue->tcp->next = NULL;
}

void queue_robopair_remove(QueueOfRoboPair *queue)
{
	RoboPair *tmp;
	tmp = queue->base;
	queue->base = queue->base->next;
	if (!queue->base)
		queue->tcp = NULL;
	free(tmp);
}

int queue_robopair_is_empty(QueueOfRoboPair *queue)
{
	return queue->base == NULL;
}

int queue_robopair_sizeof(QueueOfRoboPair *queue)
{
	int size = 0;
	RoboPair *tmp;
	tmp = queue->base;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return size;
}

RoboPair *queue_robopair_return(QueueOfRoboPair *queue, int index)
{
	int i;
	RoboPair *tmp;
	tmp = queue->base;
	if (index < 0)
		return NULL;
	if (index >= queue_robopair_sizeof(queue))
		return NULL;
	for (i = 0; i <= index; i++)
	{
		tmp = tmp->next;
	}
	return tmp;
}

void queue_robopair_put(QueueOfRoboPair *queue, void *data, int datatype, int index)
{
	RoboPair *tmp;
	tmp = queue_robopair_return(queue, index);
	if (!tmp)
	{
		queue_robopair_create(queue);
		queue_robopair_put(queue, data, datatype, index);
	}
	else
	{
		switch (datatype)
		{
		int i;
		case pvec:
			tmp->type = *((int*)data);
			break;
		case lvec:
			for (i = 0; i < vec3; i++)
				tmp->length[i] = *((double*)data + i);
			break;
		case qvec:
			for (i = 0; i < vec3; i++)
				tmp->vector[i] = *((double*)data + i);
			break;
		case rmtx:
			for (i = 0; i < mtx9; i++)
				tmp->matrix[i] = *((double*)data + i);
			break;
		}
	}
}

void queue_robopair_get(QueueOfRoboPair *queue, void *data, int datatype, int index)
{
	RoboPair *tmp;
	tmp = queue_robopair_return(queue, index);
	if (!tmp)
		return;
	switch (datatype)
	{
	case pvec:
		data = &tmp->type;
		break;
	case lvec:
		data = tmp->length;
		break;
	case qvec:
		data = tmp->vector;
		break;
	case rmtx:
		data = tmp->matrix;
		break;
	default:
		data = NULL;
	}
}
