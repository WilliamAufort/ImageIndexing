TEX = latex --shell-escape -interaction=nonstopmode

all: report.pdf

.PHONY: clean purge
	
report.pdf: report.ps
	ps2pdf $<

report.ps: report.dvi
	dvips $<

report.dvi: report.tex macros.tex
	$(TEX) $<

clean:
	@rm -f report.ps report.dvi report.pdf 

purge: clean all
