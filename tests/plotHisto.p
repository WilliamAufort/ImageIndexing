# To visualize, use
# gnuplot -persist plotHisto.p

reset

set term pdfcairo
set output "histo.pdf"

set title "An histogram"
set xlabel "Radius of the ball / Maximal radius"
set ylabel "Frequency"
set key left box

set xrange [-0.33:19.66]

set style data histogram
set style fill solid 0.5 #fillstyle

set boxwidth 3

plot "../histograms/database/device0-12.hist" using 2 notitl
