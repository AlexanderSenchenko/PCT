set terminal png size 900, 850
set xrange [0:0.5]
set yrange [0:0.05]
set output 'out.png'
plot 'out4.txt' using 1:2 with linespoints lw 1 lt rgb 'purple', 'out16.txt' using 1:2 with linespoints lw 1 lt rgb 'red', 'out28.txt' using 1:2 with linespoints lw 1 lt rgb 'green', 'out40.txt' using 1:2 with linespoints lw 1 lt rgb 'black', 'out52.txt' using 1:2 with linespoints lw 1 lt rgb 'blue', 'out64.txt' using 1:2 with linespoints lw 1 lt rgb 'red', x