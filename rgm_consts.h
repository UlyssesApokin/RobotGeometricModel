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

#ifndef RGM_CONSTS_SENTRY_H
#define RGM_CONSTS_SENTRY_H

/**
 * @pvec is vector number defining the type and sequence 
 * of kinematic pairs
 * @lvec is the number of the array of vectors defining the shape
links
 * @qvec is vector number defening the generalized coordinates
 * of kinematic pairs
 * @rmtx is the number of the array of rotation matix
 * @inar is the number of error of invalid argument
 * @misb is the number of error of misspelled square bracket
 */
enum {pvec, lvec, qvec, rmtx, inar, misb};

enum {vec3 = 3, mtx9 = 9};

#endif
