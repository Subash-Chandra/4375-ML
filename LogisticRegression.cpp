#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

double sigmoid(double);
void gradientDescent(vector<double>, vector<int>);
double accuracy(vector<int>, vector<int>);
double sensitivity(vector<int>, vector<int>);
double specificity(vector<int>, vector<int>);

int main() {
	ifstream inputStream;

	string line;

	const int SIZE = 1050;

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

	vector<double> weights(2);
	double dataMatrix[801][2]{};
	double learningRate = 0.001;

	for (int i = 0; i < 2; i++)
		weights.at(i) = (1);
	for (int i = 0; i < 800; i++) {
		dataMatrix[i][0] = 1;
		dataMatrix[i][1] = sex.at(i);
	}
	vector<double> prob(801);
	vector<double> error(801);
	double addValues[2]{};

	auto start = steady_clock::now();

	do {
		addValues[0] = 0;
		addValues[1] = 0;
		for (int x = 0; x < 800; x++) {
			prob.at(x) = (sigmoid(dataMatrix[x][0] * weights.at(0) + dataMatrix[x][1] * weights.at(1)));
			error.at(x) = (survived.at(x) - prob.at(x));
			addValues[0] += dataMatrix[x][0] * error.at(x);
			addValues[1] += dataMatrix[x][1] * error.at(x);
		}
		for (int x = 0; x < 2; x++)
			weights.at(x) += learningRate * addValues[x];
	} while (abs(addValues[0]) > .0001 && abs(addValues[1]) > .0001);

	auto end = steady_clock::now();

	double logOdds[247][2];
	for (int i = 0; i < 246; i++) {
		logOdds[i][0] = 1;
		logOdds[i][1] = sex.at(800 + i);
	}
	vector<int> prediction(247);
	for (int i = 0; i < 246; i++) {
		double temp = logOdds[i][0] * weights.at(0) + logOdds[i][1] * weights.at(1);
		double prob = exp(temp) / (1 + exp(temp));
		prediction.at(i) = prob > .5;
	}
	cout << "Accuracy " << accuracy(survived, prediction) << endl;
	cout << "Sensitivity " << sensitivity(survived, prediction) << endl;
	cout << "Specificity " << specificity(survived, prediction) << endl;
	cout << "Time to train model in milliseconds: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}

double sigmoid(double z) {
	return 1.0 / (1 + exp(-z));
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