# Set terminal type (e.g., PNG output or screen display)
set terminal png size 2000,800
set output 'graphs/Score.png'   # Output file name

# Add labels and titles
set title "Score Plot"
set xlabel "Generation"
set ylabel "Score"

# Plot multiple .dat files
plot "data/fitness.dat" using 1:2 with lines title "Best", \
     "data/mean.dat" using 1:2 with lines title "Mean", \
     "data/mut.dat" using 1:2 with lines title "Mutation", \
     