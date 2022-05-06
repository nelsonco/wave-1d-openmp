#include <iostream>
#include <list>
#include <omp.h>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;
int main (){
    
    double start = omp_get_wtime(); // Start timer

    // Initialize the vector arrays
    vector<double> y0; 
    vector<double> yt0;
    vector<double> y1;
    vector<double> yt1;

    // Initialize (declare) Constants and Variables
    double k = 0.00062831853; 
    double alpha = 0.9;	
    double x;
    double xt;

    // Fill the first vector array 
    y0.push_back(0); // Set y0[0] = 0
    yt0.push_back(0);// Set yt0[0] = 0
    # pragma omp parallel // parallelize the loop
    for(int i=1; i<10000; i++){
        y0.push_back(sin(k*i));
        yt0.push_back(0);
    }
    y0.push_back(0);// Set y0[10000] = 0
    yt0.push_back(0);// Set yt0[10000] = 0

    // Initialized the second vector array
    y1.push_back(0); // Set y1[0] = 0
    yt1.push_back(0); // Set yt1[0] = 0

    # pragma omp parallel // parallelize the loop
    for(int i=1; i<10000; i++){
        x = .5*pow(alpha,2)*y0[i-1] + (1.0-pow(alpha,2)*y0[i]+.5*pow(alpha,2)*y0[i+1]+yt0[i]); // Calculate value of y1[i]
	y1.push_back(x); // Insert x into y1 at position i
	xt = y1[i]-y0[i]; // Calculate value of yt1[i]
	yt1.push_back(xt); // Insert xt into yt1 at posion i
    }
    y1.push_back(0); // Set y1[10000] = 0
    yt1.push_back(0); // Set yt1[10000] = 0

    // Take 1000 Steps where the ith state depends on the i-1 state (Should not be parallelized)
    for(int i=1; i<1000; i++){
        //Set the current state y1,yt1 to the previous state y0,yt0
        y0.clear(); 
        yt0.clear();
        y0 = y1; 
        yt0 = yt1;

        //Clear the current state so it can be calculated using the new previous state
	yt1.clear();
	y1.clear(); // Clear y1

        // Populate y1 with the new current state based on the previous state.
        y1.push_back(0);
        yt1.push_back(0);
	# pragma omp parallel // Parallelize the filling of the vector
        for(int i=1; i<10000; i++){
             x = .5*pow(alpha,2)*y0[i-1] + (1.0-pow(alpha,2)*y0[i]+.5*pow(alpha,2)*y0[i+1]+yt0[i]);
             y1.push_back(x);
	     xt = y1[i]-y0[i];
	     yt1.push_back(xt);
        }
        y1.push_back(0); 
	yt1.push_back(0);

    }
    double end = omp_get_wtime(); // Stop the timer
    double time_elapsed = end-start;

    // Save Results to Output Files
    ofstream out;
    ofstream runtimes;
    out.open("out.txt");
    for(int i=0; i<1001; ++i){
	    out<<yt1[i]<<" "<<y1[i]<<endl;
    }
    out.close();
    runtimes.open("runtimes.txt", ios_base::app);
    runtimes << "Run Time " << time_elapsed << endl;
    runtimes.close();

};
