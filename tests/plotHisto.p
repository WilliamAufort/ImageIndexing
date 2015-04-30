# To visualize, use
# gnuplot -persist plotHisto.p

reset

### decommenter les 2 lignes ci-dessous pour engendrer un fichier pdf
### plutot qu`un dessin a l`ecran
set term pdfcairo
set output "histo.pdf"

set title "An histogram"
set xlabel "Radius of the ball / Maximal radius"
set ylabel "Frequency"
set key left box

set xrange [-0.33:19.66]


# Dessin en joignant des points
set style data histogram
set style fill solid 0.5 #fillstyle

set boxwidth 3

# on trace deux courbes: avec les colonnes 1 et 2, avec les colonnes 1 et 3
# a chaque fois, le nom de la courbe est lu en tete de colonne
plot "../histograms/database/device0-12.hist" using 2 notitl
