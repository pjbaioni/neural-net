plot "TestSet300.dat"	u 1:2 w lp lw 4 title "Test Data", \
			"< paste TestSet300.dat yhat300.dat" u 1:3 w lp lw 1.5 title "Prevision"
pause mouse "Click any mouse button on selected data point to close\n"
