set terminal post eps enhanced color "Helvetica" 20
set output "diff3_t.eps"
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
set xlabel font "Helvetica,25"
set ylabel font "Helvetica,25"
set border lw 10.0
set cbtics format "%.1f"
set cbrange [-3.5:3.5]
set cbtics -3.5,1.0,3.5
splot "t1_d3.dat" u ($2*cos($3)/0.5):($2*sin($3)/0.5):4 w p ps 0.8 pt 7 palette t ""


