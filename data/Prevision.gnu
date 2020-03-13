plot "LinspacedTestSet.dat"	u 1:2 w lp lw 3 title "Test set", \
			"< paste LinspacedTestSet.dat yhat.dat" u 1:3 w lp lw 2 title "Prevision"
pause mouse close
