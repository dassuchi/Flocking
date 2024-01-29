set terminal post eps enhanced color "Helvetica" 20
set output "t1.eps"
set size square 1,1
set palette defined (0 "#000090", 1 "#000fff", 2 "#0090ff", 3 "#0fffee", 4 "#90ff70", 5 "#ffee00", 6 "#ff7000", 7 "#ee0000", 8 "#7f0000")  # Define the "jet" colormap
set view map
set xtics font "Helvetica,20"
set ytics font "Helvetica,20"
set cbtics font"Helvetica,20"
set key font "Helvetica,20"
set key spacing 1.4
set key bottom left
set xrange [-12:12]
set yrange [-12:12]
set cbrange [:4.0]
set xlabel font "Helvetica,25"
set ylabel font "Helvetica,25"
set border lw 10.0
set cbtics format "%.1f"
set object 1 circle at 0,0 radius 1 fillcolor rgb "grey" fillstyle empty border 1 front lw 2
set object 2 circle at 0,0 radius 3.36864 fillcolor rgb "black" fillstyle empty border 2 front lw 5.5
splot "t1.dat" u ($4/0.5):($5/0.5):7 w p ps 0.8 pt 7 palette t ""


