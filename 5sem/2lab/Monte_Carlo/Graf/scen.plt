set terminal png size 500, 350 font 'Verdana, 10'
set output 'out.png'
plot 'out10_7.txt' using 1:2 with linespoints lw 1 lt rgb 'purple', 'out10_8.txt' using 1:2 with linespoints lw 1 lt rgb 'green', x