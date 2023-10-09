#!/bin/bash

# Applications used during translation
tex2pdf=PDFLATEX
svg2pdf=INKSCAPE
dxf2pdf=LIBRECAD
underlay4pdf=QPDF

# Configuring the trasnlate
if [ "$1" == "full" ]; then
	STATUS=$tex2pdf'_'$svg2pdf'_'$dxf2pdf'_'$underlay4pdf
elif [ "$1" == "tex" ]; then
	STATUS=$tex2pdf'_'$underlay4pdf
elif [ "$1" == "frame" ]; then
	STATUS=$svg2pdf'_'$underlay4pdf
elif [ "$1" == "draft" ]; then
	STATUS=$tex2pdf'_'$dxf2pdf'_'$underlay4pdf
else
	STATUS=$tex2pdf'_'$underlay4pdf
fi

# LibreCAD: draft in .dxf to PDF-file
if [[ "$STATUS" == *"$dxf2pdf"* ]]; then
	librecad dxf2pdf rgm_T2_S_T1.dxf
	librecad dxf2pdf rgm_T2_S_T1_DH-param.dxf
fi

# Inkscape: draft in .dxf to PDF-file
if [[ "$STATUS" == *"$svg2pdf"* ]]; then
	inkscape frame-content-sheet-gost2104-68.svg --export-area-drawing --export-type=pdf --export-filename=frame-content-sheet-gost2104-68.pdf
	inkscape frame-main-sheet-gost2104-68.svg --export-area-drawing --export-type=pdf --export-filename=frame-main-sheet-gost2104-68.pdf
	inkscape frame-titul-sheet-gost2104-68.svg --export-area-drawing --export-type=pdf --export-filename=frame-titul-sheet-gost2104-68.pdf
fi

# PDFLaTeX
if [[ "$STATUS" == *"$tex2pdf"* ]]; then
	pdflatex kurs.tex
fi

# Applying a frame to a document
if [[ "$STATUS" == *"$underlay4pdf"* ]]; then
	qpdf kurs.pdf --underlay frame-titul-sheet-gost2104-68.pdf --to=1 -- final.pdf
	qpdf --replace-input --underlay frame-main-sheet-gost2104-68.pdf --to=2 -- final.pdf
	qpdf --replace-input --underlay frame-content-sheet-gost2104-68.pdf --to=3 -- final.pdf
	qpdf --replace-input --underlay frame-main-sheet-gost2104-68.pdf --from= --repeat=1 --to=4-z -- final.pdf
fi

sleep 1

# Debug output
if [[ "$STATUS" == *"$dxf2pdf"* ]]; then echo "LibreCAD was launched"; fi

if [[ "$STATUS" == *"$svg2pdf"* ]]; then echo "Inkscape was launched"; fi

if [[ "$STATUS" == *"$tex2pdf"* ]]; then echo "LaTeX was launched"; fi

if [[ "$STATUS" == *"$underlay4pdf"* ]]; then echo "qPDF was launched"; fi

echo "Done!..."
