##  Copyright (c) <2023> <Ulysses Apokin (Jelisej Apokin)>
##
##  Permission is hereby granted, free of charge, to any person obtaining
##  a copy of this software and associated documentation files (the
##  "Software"), to deal in the Software without restriction, including
##  without limitation the rights to use, copy, modify, merge, publish,
##  distribute, sublicense, and/or sell copies of the Software, and to
##  permit persons to whom the Software is furnished to do so, subject to
##  the following conditions:
##
##  The above copyright notice and this permission notice shall be
##  included in all copies or substantial portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
##  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
##  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
##  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
##  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
##  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
##  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

disp('---------------')
disp('     RGM       ')
pairLenght = struct('L1', 6, 'L2', 3, 'L3', 2);
d1 = 0;
theta1 = 0;
theta3 = 0;
aDHParam = struct('P1', 0, 'P2', 0, 'P3', pairLenght.L3);
alphaDHParam = struct('P1', 0, 'P2', pi/2, 'P3', 0);
dDHParam = struct('P1', pairLenght.L1 + d1, 'P2', pairLenght.L2, 'P3', 0);
thetaDHParam = struct('P1', theta1, 'P2', pi, 'P3', theta3 + pi/2);
A01 = createDHmatrix(thetaDHParam.P1, dDHParam.P1, alphaDHParam.P1, aDHParam.P1);
A12 = createDHmatrix(thetaDHParam.P2, dDHParam.P2, alphaDHParam.P2, aDHParam.P2);
A23 = createDHmatrix(thetaDHParam.P3, dDHParam.P3, alphaDHParam.P3, aDHParam.P3);
A03 = A01 * A12 * A23;
disp('theta1 = ');
disp(theta1);
disp('theta3 = ');
disp(theta3);
disp('d1 = ');
disp(d1);
disp('A03 = ');
disp(A03);
disp('----------------');
