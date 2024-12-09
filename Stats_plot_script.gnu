# Set terminal type (e.g., PNG output or screen display)
set terminal png size 2000,800
set output 'graphs/bestStats.png'   # Output file name

# Add labels and titles
set title "Best Stats Plot"
set xlabel "Generation"
set ylabel "Stats Points"

# Plot multiple .dat files with explicit colors
plot "data/atk.dat" using 1:2 with lines linecolor rgb "#FF0000" title "atk", \
     "data/def.dat" using 1:2 with lines linecolor rgb "#00FF00" title "def", \
     "data/hp.dat" using 1:2 with lines linecolor rgb "#0000FF" title "hp", \
     "data/regen.dat" using 1:2 with lines linecolor rgb "#FFFF00" title "regen", \
     "data/spd.dat" using 1:2 with lines linecolor rgb "#FF00FF" title "spd", \
     "data/intel.dat" using 1:2 with lines linecolor rgb "#00FFFF" title "intel", \
     "data/spatk.dat" using 1:2 with lines linecolor rgb "#888800" title "spatk", \
     "data/spdef.dat" using 1:2 with lines linecolor rgb "#000000" title "spdef", \
     "data/luck.dat" using 1:2 with lines linecolor rgb "#008080" title "luck", \
     "data/agi.dat" using 1:2 with lines linecolor rgb "#800080" title "agi"
