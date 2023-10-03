#!/bin/bash

pdflatex kurs.tex
qpdf kurs.pdf --underlay frame-titul-sheet-gost2104-68.pdf --to=1 -- final.pdf
qpdf --replace-input --underlay frame-main-sheet-gost2104-68.pdf --to=2 -- final.pdf
qpdf --replace-input --underlay frame-сontent-sheet-gost2104-68.pdf --to=3 -- final.pdf
qpdf --replace-input --underlay frame-main-sheet-gost2104-68.pdf --from= --repeat=1 --to=4-z -- final.pdf
