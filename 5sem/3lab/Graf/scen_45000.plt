set terminal png size 500, 350 font 'Verdana, 10'
set title ""
set ylabel "S(sec)"
set xlabel "p"
set xrange [ 0 : 65 ]
set xtics ("8" 8,"16" 16, "32" 32, "64" 64)
set output 'S_45000.png'
plot '45000.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title '45000', x