#include<bits/stdc++.h>
using namespace std;

int main(int argc, char** argv){

    // string file = argv[14];
    // string ext = ".txt"; //file extension
    // file += ext; //appends ext to file  
    ofstream fout (argv[14]);

// MSS value to be set 1 KB throughout all the emulations
    const double MSS = 1; 
// RWS value to be set 1 MB throughout all the emulations
    const double RWS = 1000;
// ssthresh is to be intitated here, hence we keep it as 2 KB
    double ssthresh = 2; 

    double ki, km, kn, kf, ps;
    int n;
    
    ki = stod(argv[2]);
    km = stod(argv[4]);
    kn = stod(argv[6]);
    kf = stod(argv[8]);
    ps = stod(argv[10]);
    n = stoi(argv[12]);

    double CW = ki * MSS * 1.0;

    // // X -> Time & Y -> CW for gnuplot
    // fout << "X" << " " << "Y" << endl;

    // Intial CWnew line
    fout << "0 " << CW << endl;

    //NOS -> Number of segments
    const int NOS = n;

    for(int i = 0; i < NOS; i++){
        
        int N = ceil(CW/MSS);

        for (int i = 0; i < N; i++) {

            double randomNumber = (1.0*(rand()%10))/10;

            if (randomNumber < ps) {
                ssthresh = CW/2;
                CW = max((double)4, kf*CW);
            }
            else {
                // randomeNumber >= Ps
                if (CW < ssthresh) {
                    CW = min(CW + km*MSS, RWS);
                }
                else {
                    CW = min(CW + kn*MSS*MSS/CW, RWS);
                }
            }
        }
        fout << i+1 << " " << CW << '\n'; 
    }
    return 0;
}

/*
---------------------Directions------------------------------
CLI:

     g++ main.cpp -o cw

    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_1.txt
    ./cw -ki 2 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_2.txt
    ./cw -ki 3 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_3.txt
    ./cw -ki 4 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o ki_4.txt

    ./cw -ki 1 -km 0.5 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_1.txt
    ./cw -ki 1 -km 1 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_2.txt
    ./cw -ki 1 -km 1.5 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_3.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o km_4.txt

    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o kn_1.txt
    ./cw -ki 1 -km 2 -kn 1 -kf 0.5 -ps 0.03 -n 40 -o kn_2.txt
    ./cw -ki 1 -km 2 -kn 1.5 -kf 0.5 -ps 0.03 -n 40 -o kn_3.txt
    ./cw -ki 1 -km 2 -kn 2 -kf 0.5 -ps 0.03 -n 40 -o kn_4.txt

    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.1 -ps 0.03 -n 40 -o kf_1.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.2 -ps 0.03 -n 40 -o kf_2.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.3 -ps 0.03 -n 40 -o kf_3.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.03 -n 40 -o kf_4.txt

    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.1 -n 40 -o ps_1.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.2 -n 40 -o ps_2.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.3 -n 40 -o ps_3.txt
    ./cw -ki 1 -km 2 -kn 0.5 -kf 0.5 -ps 0.4 -n 40 -o ps_4.txt

*/
