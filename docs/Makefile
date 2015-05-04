TEX = latex --shell-escape -interaction=nonstopmode
BIB = bibtex

all: report.pdf

.PHONY: clean purge
	
report.pdf: report.ps
	ps2pdf $<

report.ps: report.dvi
	dvips $<

report.dvi: report.tex biblio.bib macros.tex
	$(TEX) report.tex
	$(BIB) report	
	$(TEX) report.tex
	$(TEX) report.tex

clean:
	@rm -f report.ps report.dvi report.pdf 

purge: clean all
