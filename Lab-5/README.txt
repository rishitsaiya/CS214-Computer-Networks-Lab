The code is in 180010027.cpp
To compile it and make the output file named cw
 ->   g++ 180010027.cpp -o cw

To make all the output files with varying ki parameter:

 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_1.txt
 ->   ./cw -ki 2 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_2.txt
 ->   ./cw -ki 3 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_3.txt
 ->   ./cw -ki 4 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_4.txt

To make all the output files with varying km parameter:

 ->   ./cw -ki 1 -km 0.5 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_1.txt
 ->   ./cw -ki 1 -km 1 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_2.txt
 ->   ./cw -ki 1 -km 1.5 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_3.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_4.txt

To make all the output files with varying kn parameter:

 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o kn_1.txt
 ->   ./cw -ki 1 -km 2 -kn 1 -kf 0.5 -ps 0.03 -n 40 -o kn_2.txt
 ->   ./cw -ki 1 -km 2 -kn 1.5 -kf 0.5 -ps 0.03 -n 40 -o kn_3.txt
 ->   ./cw -ki 1 -km 2 -kn 2 -kf 0.5 -ps 0.03 -n 40 -o kn_4.txt

To make all the output files with varying kf parameter:

 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.1 -ps 0.03 -n 40 -o kf_1.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.2 -ps 0.03 -n 40 -o kf_2.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.3 -ps 0.03 -n 40 -o kf_3.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o kf_4.txt

To make all the output files with varying ps parameter:

 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.1 -n 40 -o ps_1.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.2 -n 40 -o ps_2.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.3 -n 40 -o ps_3.txt
 ->   ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.4 -n 40 -o ps_4.txt

After all the output files are ready, we go to plotting them.

 -> gnuplot

 To plot ki.png

 -> set title "Different Values of ki , step = 1"
 -> set ylabel "Congestion Window Size"
 -> set xlabel "Time"
 -> plot 'ki_1.txt' w l, 'ki_2.txt' w l, 'ki_3.txt' w l, 'ki_4.txt' w l

 To plot km.png

 -> set title "Different Values of km , step = 0.5"
 -> set ylabel "Congestion Window Size"
 -> set xlabel "Time"
 -> plot 'km_1.txt' w l, 'km_2.txt' w l, 'km_3.txt' w l, 'km_4.txt' w l

To plot kn.png
 
 -> set title "Different Values of kn , step = 0.5"
 -> set ylabel "Congestion Window Size"
 -> set xlabel "Time"
 -> plot 'kn_1.txt' w l, 'kn_2.txt' w l, 'kn_3.txt' w l, 'kn_4.txt' w l

To plot kf.png 
 
 -> set title "Different Values of kf , step = 0.1"
 -> set ylabel "Congestion Window Size"
 -> set xlabel "Time"
 -> plot 'kf_1.txt' w l, 'kf_2.txt' w l, 'kf_3.txt' w l, 'kf_4.txt' w l

To plot ps.png

 -> set title "Different Values of ps , step = 0.1"
 -> set ylabel "Congestion Window Size"
 -> set xlabel "Time"
 -> plot 'ps_1.txt' w l, 'ps_2.txt' w l, 'ps_3.txt' w l, 'ps_4.txt' w l
