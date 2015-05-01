# pour visualiser le dessin trace par ce script gnuplot, taper
# gnuplot -persist script-plot.p
#  (en s`assurant que le fichier comparaison.dat est present dans le repertoire)

reset

### decommenter les 2 lignes ci-dessous pour engendrer un fichier pdf
### plutot qu`un dessin a l`ecran
set term pdfcairo
set output "plot.pdf" # le nom du fichier qui est engendre

set title "TOUT"
set xlabel "Index"
set ylabel "Note"
set key outside
set key right top


# Dessin en joignant des points
set style data linespoints

set pointsize 0.1   # la taille des points


# on trace deux courbes: avec les colonnes 1 et 2, avec les colonnes 1 et 3
# a chaque fois, le nom de la courbe est lu en tete de colonne
plot "plot/plot.dat" using 1:2 title columnheader(2), \
     "plot/plot.dat" using 1:3 title columnheader(3), \
     "plot/plot.dat" using 1:4 title columnheader(4), \
     "plot/plot.dat" using 1:5 title columnheader(5), \
     "plot/plot.dat" using 1:6 title columnheader(6), \
     "plot/plot.dat" using 1:7 title columnheader(7), \
     "plot/plot.dat" using 1:8 title columnheader(8), \
     "plot/plot.dat" using 1:9 title columnheader(9), \
     "plot/plot.dat" using 1:10 title columnheader(10), \
     "plot/plot.dat" using 1:11 title columnheader(11), \
     "plot/plot.dat" using 1:12 title columnheader(12), \
     "plot/plot.dat" using 1:13 title columnheader(13), \
     "plot/plot.dat" using 1:14 title columnheader(14), \
     "plot/plot.dat" using 1:15 title columnheader(15), \
     "plot/plot.dat" using 1:16 title columnheader(16)
     
plot "plot/plot.dat" using 1:17 title columnheader(17), \
     "plot/plot.dat" using 1:18 title columnheader(18), \
     "plot/plot.dat" using 1:19 title columnheader(19), \
     "plot/plot.dat" using 1:20 title columnheader(20), \
     "plot/plot.dat" using 1:21 title columnheader(21)
