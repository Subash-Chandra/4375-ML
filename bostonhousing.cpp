#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

double sum(vector<double> x){
    
    double sum = 0;
	int size = x.size();
    for(int i = 0; i <= size;i++){
        sum += x[i];
    }
    return sum;
}

double mean(vector<double> x){

    return sum(x)/x.size();

}

double median(vector<double> x){

    double med;
    sort(x.begin(), x.end());

    if(x.size() % 2 == 0)
        med = (x[x.size()/2 - 1] + x[x.size()/2]) / 2; // average the middle 2 if the vector is even sized
    else
        med = x[x.size()/2]; // if odd, middle is the middle

    return med;

}

double range(vector<double> x){

    double min = x[0];
    double max = x[0];
	int size = x.size();


    for(int i = 0; i <= size; i++){
        if(x[i] < min)
        {
            min = x[i];
        }
        if(x[i] > max)
        {
            max = x[i];
        }
    }

    return max - min;

}

void print_stats(vector<double> x){

    cout << "\t Sum    = " << sum(x) << endl;
    cout << "\t Mean   = " << mean(x) << endl;
    cout << "\t Median = " << median(x) << endl;
    cout << "\t Range  = " << range(x) << endl;

}

double covar(vector<double>  x, vector<double> y){

    double covar = 0;
    double total = 0;
    int size = x.size();

    for(int i = 0; i < size; i++)
    {
        total += ((x[i] - mean(x)) * (y[i] - mean(y)));
    }
    covar = total/(x.size()-1);
    return covar;

}

double cor(vector<double> x, vector<double> y){

    double xSTD = sqrt(covar(x,x));
    double ySTD = sqrt(covar(y,y));
    return covar(x,y)/(xSTD * ySTD);

    //https://www.probabilitycourse.com/chapter5/5_3_1_covariance_correlation.php
    //  covar = SUM[    X-Mean(x)    *   Y-Mean(y)    ]
    //  Cor = COV(X,Y) / SQRT(COV(X,X)) * SQRT(COV(Y,Y))

}

int main(int argc, char** argv)
{
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    vector<double> rm(1000);
    vector<double> medv(1000);

    cout << "Opening file Boston.csv." << endl;

    inFS.open("Boston.csv");
    if(!inFS.is_open()) {
        cout << "Could not open file boston.csv." << endl;
        return 1;
    }

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {

        
        getline(inFS, rm_in, ',');
        getline(inFS,medv_in,'\n');

        rm.at(numObservations) = stof(rm_in);
        cout << "test " << rm_in << endl;
        medv.at(numObservations) = stof(medv_in);
        cout << "test " << medv_in << endl;

        numObservations++;
        cout << "test " << numObservations << endl;

    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length" << rm.size() <<endl;

    cout << "Closing file Boston.csv" << endl;

    inFS.close();

    cout << "Number of Records: " << numObservations << endl;

    cout << "\nStats for rm" << endl;
    print_stats(rm);

    cout << "\nStats for medv" << endl;
    print_stats(medv);

    cout << "\nCovariance = " << covar(rm,medv) << endl;

    cout << "\nCorrelation = " << cor(rm,medv) << endl;

    cout << "\nProgram terminated.";

    return 0;


}


