#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;

double accuracy(vector<int>, vector<int>);
double sensitivity(vector<int>, vector<int>);
double specificity(vector<int>, vector<int>);
double calc_age_lh(double, double, double,double);


int main()
{
	ifstream inputStream;

	string line;

	const int SIZE = 1050;
	const double PI = 3.141592653589793238462643383279502884;

	vector<int> pclass(SIZE);
	vector<int> age(SIZE);
	vector<int> survived(SIZE);
	vector<int> sex(SIZE);

	inputStream.open("titanic.csv");

	if (!inputStream.is_open()) {
		cout << "There was a problem opening the file" << endl;
		return 1;
	}
	getline(inputStream, line);

	int numObs = 0;

	while (inputStream.good()) {
		getline(inputStream, line, ',');

		getline(inputStream, line, ',');
		pclass.at(numObs) = (stoi(line));

		getline(inputStream, line, ',');
		survived.at(numObs) = (stoi(line));

		getline(inputStream, line, ',');
		sex.at(numObs) = (stoi(line));

		getline(inputStream, line, '\n');
		age.at(numObs) = (stoi(line));

		numObs++;
	}
	inputStream.close();

	double apriori[2];
	double pClassPrior[2][3];
	double lhAge = 0;
	double sexPrior[2][2];
	double countSurv[2];
	double ageMean[2];
	double ageVar[2];

	for (int x = 0; x < 2; x++) {
		apriori[x] = 0;
		countSurv[x] = 0;
		ageMean[x] = 0;
		ageVar[x] = 0;
		for (int y = 0; y < 3; y++) {
			pClassPrior[x][y] = 0;
		}
		for (int y = 0; y < 2; y++) {
			sexPrior[x][y] = 0;
		}
	}

	auto start = steady_clock::now();

	for (int i = 0; i < 1046; i++) {
		countSurv[0] += 1 - survived[i]; // number of people that died
		countSurv[1] += survived[i]; // number of people that lived
	}
	apriori[0] = countSurv[0] / 1046.0;
	apriori[1] = countSurv[1] / 1046.0;
	for (int i = 0; i < 1046; i++) {
		pClassPrior[survived[i]][pclass[i] - 1]++;
		sexPrior[survived[i]][sex[i]]++;
		ageMean[survived[i]] += age[i];
	}
	for (int x = 0; x < 2; x++) {
		ageMean[x] /= countSurv[x];
		for (int y = 0; y < 3; y++) {
			pClassPrior[x][y] /= countSurv[x];
		}
		for (int y = 0; y < 2; y++) {
			sexPrior[x][y] /= countSurv[x];
		}
	}
	for (int i = 0; i < 1046; i++) {
		ageVar[survived[i]] += (age[i] - ageMean[survived[i]]) * (age[i] - ageMean[survived[i]]);
	}
	for (int x = 0; x < 2; x++) {
		ageVar[x] /= countSurv[x] - 1;
	}

	auto end = steady_clock::now();
	vector<int> prediction(247);
	for (int i = 800; i < 1046; i++) {
		double prob_survived = pClassPrior[1][pclass[i] - 1] * sexPrior[1][sex[i]] * apriori[1] * calc_age_lh(age[i], ageMean[1], ageVar[1], PI);
		double prob_died = pClassPrior[0][pclass[i] - 1] * sexPrior[0][sex[i]] * apriori[0] * calc_age_lh(age[i], ageMean[0], ageVar[0], PI);
		double denominator = pClassPrior[1][pclass[i] - 1] * sexPrior[1][sex[i]] * apriori[1] * calc_age_lh(age[i], ageMean[1], ageVar[1], PI) +
			pClassPrior[0][pclass[i] - 1] * sexPrior[0][sex[i]] * apriori[0] * calc_age_lh(age[i], ageMean[0], ageVar[0], PI);
		if ((prob_survived / denominator) > 0.5)
			prediction[i - 800] = 1;
		else
			prediction[i - 800] = 0;
	}
	cout << "Accuracy " << accuracy(survived, prediction) << endl;
	cout << "Sensitivity " << sensitivity(survived, prediction) << endl;
	cout << "Specificity " << specificity(survived, prediction) << endl;
	cout << "Time to train model in nanoseconds: " << duration_cast<nanoseconds>(end - start).count() << " ns" << endl;
}

double calc_age_lh(double age, double mean, double variance, double PI) {
	return 1.0 / sqrt(2 * PI * variance) * exp(-((age - mean) * (age - mean)) / (2 * variance));
}
double accuracy(vector<int> survived, vector<int> prediction) {
	double TP = 0;
	double TN = 0;
	double FP = 0;
	double FN = 0;
	for (int x = 800; x < 1046; x++) {
		if (survived.at(x) == 1 && prediction.at(x - 800) == 1)
			TP++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 0)
			TN++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 1)
			FP++;
		if (survived.at(x) == 1 && prediction.at(x - 800) == 0)
			FN++;
	}
	return (TP + TN) / (TP + TN + FP + FN);
}
double sensitivity(vector<int> survived, vector<int> prediction) {
	double TP = 0;
	double TN = 0;
	double FP = 0;
	double FN = 0;
	for (int x = 800; x < 1046; x++) {
		if (survived.at(x) == 1 && prediction.at(x - 800) == 1)
			TP++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 0)
			TN++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 1)
			FP++;
		if (survived.at(x) == 1 && prediction.at(x - 800) == 0)
			FN++;
	}
	return (TP) / (TP + FN);
}
double specificity(vector<int> survived, vector<int> prediction) {
	double TP = 0;
	double TN = 0;
	double FP = 0;
	double FN = 0;
	for (int x = 800; x < 1046; x++) {
		if (survived.at(x) == 1 && prediction.at(x - 800) == 1)
			TP++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 0)
			TN++;
		if (survived.at(x) == 0 && prediction.at(x - 800) == 1)
			FP++;
		if (survived.at(x) == 1 && prediction.at(x - 800) == 0)
			FN++;
	}
	return (TN) / (TN + FP);
}