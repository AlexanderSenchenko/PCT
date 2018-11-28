set terminal png size 500, 350 font 'Verdana, 10'
set title ""
set ylabel "S(sec)"
set xlabel "p"
set xrange [ 0 : 35 ]
set xtics ("4" 4, "8" 8, "16" 16, "32" 32)

set output 'S_800.png'
plot '800.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title '4--*--800', x

set output 'S_1600.png'
plot '1600.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title '4--*--1600', x

set output 'S_3200.png'
plot '3200.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title '4--*--3200', x

set output 'all.png'
plot '800.txt' using 1:2 with linespoints lw 1 lt rgb 'blue' title '4--*--800', '1600.txt' using 1:2 with linespoints lw 1 lt rgb 'purple' title '4--*--1600', '3200.txt' using 1:2 with linespoints lw 1 lt rgb 'green' title '4--*--3200', x