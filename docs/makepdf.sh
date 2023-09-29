#!/bin/bash

pdflatex kurs.tex
qpdf kurs.pdf --underlay frame-titul-sheet-gost2104-68.pdf --to=1 -- final.pdf
qpdf final.pdf --underlay frame-main-sheet-gost2104-68.pdf --to=2 -- final2.pdf
qpdf final2.pdf --underlay frame-сontent-sheet-gost2104-68.pdf --to=3 -- final.pdf
qpdf final.pdf --underlay frame-main-sheet-gost2104-68.pdf --from= --repeat=1 --to=4-14 -- final2.pdf
mv final2.pdf final.pdf
