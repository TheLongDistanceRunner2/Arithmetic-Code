#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Data {
	char letter;
	double probability;
	int occurencesNumber;
};

int main() {
	cout << "Choose option: 1 - coding, 2 - decoding: " << endl;
	int option;
	cin >> option;

	//====================================================================================================
	// coding:
	if (option == 1) {
		std::fstream _file;
		string fileName = "input.txt";
		_file.open(fileName, std::ios::in | std::ios::out);

		vector<Data> data;
		vector<double> cumulatedProbabilities;
		cumulatedProbabilities.push_back(0.0);

		int alphabetSize = 0;

		if (_file.good() == true)
		{
			cout << endl << "Read data from file: " << fileName << endl;

			// read the size of alphabet:
			_file >> alphabetSize;

			// read the letter and its probability:
			for (int i = 0; i < alphabetSize; i++) {
				Data tmp;
				_file >> tmp.letter;
				_file >> tmp.probability;

				// count cumulated probabilities:
				double previousProbability = cumulatedProbabilities.at(i);
				double nextProbability = previousProbability + tmp.probability;
				cumulatedProbabilities.push_back(nextProbability);

				// add tmp object to vector:
				data.push_back(tmp);
			}

			// read text to encode:
			string wejscie = "";
			_file >> wejscie;

			// define range:
			double L = 0.0;
			double R = 1.0;

			// define new range:
			double p1 = 0;
			double p2 = 0;

			int positionsNumber = 0;

			//=========================================================
			for (int i = 0; i < wejscie.length(); i++) {
				char c = wejscie.at(i);

				// for the first character:
				if (i == 0) {
					p1 = 0.0;
					p2 = cumulatedProbabilities.at(1);

					double tmp_L = L + (R - L) * p1;
					R = L + (R - L) * p2;
					L = tmp_L;

					positionsNumber++;
				}
				// for the last character:
				else if (i == wejscie.size() - 1) {
					p1 = cumulatedProbabilities.at(cumulatedProbabilities.size() - 2);
					p2 = cumulatedProbabilities.at(cumulatedProbabilities.size() - 1);

					double tmp_L = L + (R - L) * p1;
					R = L + (R - L) * p2;
					L = tmp_L;
				}
				// for others:
				else {
					// if the previous character was the same, don't change p1 and p2:
					if (c == wejscie.at(i - 1)) {
						double tmp_L = L + (R - L) * p1;
						R = L + (R - L) * p2;
						L = tmp_L;
					}
					// if we have a new character, set new values of p1 and p2:
					else {
						p1 = cumulatedProbabilities.at(positionsNumber);
						p2 = cumulatedProbabilities.at(positionsNumber + 1);

						double tmp_L = L + (R - L) * p1;
						R = L + (R - L) * p2;
						L = tmp_L;

						positionsNumber++;
					}
				}
			}

			double result = (double)(R + L) / (double)2.0;
			cout << endl << "-> encoded: " << result << endl;

			// remember to close the file:
			_file.close();
		}
	}
	//====================================================================================================
	// decoding:
	else if (option == 2) {
		std::fstream _file;
		string fileName = "output.txt";
		_file.open(fileName, std::ios::in | std::ios::out);

		vector<Data> data;
		vector<double> cumulatedProbabilities;
		cumulatedProbabilities.push_back(0.0);

		int alphabetSize = 0;
		
		if (_file.good() == true)
		{
			cout << endl << "Read data from file: " << fileName << endl;

			// read alphabet size:
			_file >> alphabetSize;
			
			// read the letter and its probability:
			for (int i = 0; i < alphabetSize; i++) {
				Data tmp;
				_file >> tmp.letter;
				_file >> tmp.occurencesNumber;
				_file >> tmp.probability;

				// count cumulated probabilities:
				double previousProbability = cumulatedProbabilities.at(i);
				double nextProbability = previousProbability + tmp.probability;
				cumulatedProbabilities.push_back(nextProbability);

				// add tmp object to vector:
				data.push_back(tmp);
			}

			// read length of the text:
			int textLength = 0;
			_file >> textLength;

			// read number for decoding:
			double in = 0.0;
			_file >> in;

			double L = 0.0;
			double R = 0.0;

			string result = "";
			char c = '-';
			double difference = 0.0;

			//==============================================

			for (int i = 0; i < textLength; i++) {
				int upperLimit = 0;

				// search cumulatedProbabilities tab to find appropriate section for the number:
				for (int j = 0; j < alphabetSize; j++) {
					if (in < cumulatedProbabilities.at(j)) {
						break;
					}
					else {
						upperLimit++;
					}
				}

				// the letter is in this section:
				R = cumulatedProbabilities.at(upperLimit);
				L = cumulatedProbabilities.at(upperLimit - 1);

				// read the letter:
				c = data.at(upperLimit - 1).letter;

				// add it to result string:
				result += c;

				// set new values of cumulated probabilities:
				cumulatedProbabilities.at(0) = L;
				cumulatedProbabilities.at(cumulatedProbabilities.size() - 1) = R;

				difference = R - L;

				// middle ones:
				for (int j = 1; j < cumulatedProbabilities.size() - 1; j++) {
					cumulatedProbabilities.at(j) = cumulatedProbabilities.at(j -1) + difference * data.at(j - 1).probability;
				}
			}

			cout << endl << "-> decoded: " << result << endl;

			// remember to close the file:
			_file.close();
		}
		else {
			cout << "CANNOT READ THE FILE !!!" << endl;
		}
	}
	// error message:
	else {
		cout << "ENTERED WRONG NUMBER!!! THE END OF THE PROGRAM !!!" << endl;
	}

	cout << endl << endl;
	system("pause");
	return 0;
}