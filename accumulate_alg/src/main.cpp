//============================================================================
// Script for testing the performance of parallel accumulate algorithm
//============================================================================

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "timer.h"
#include "parallel_accumulate.h"
using namespace std;

void usageMsg(void) {
	string separator(50, '-');
	ostringstream msg;
	msg << separator << endl;
	msg << "Usage: ./accumulate_test kNelements kNthreads kNiter" << endl << endl;
	msg << "Where: " << endl;
	msg << "kNelements = number of elements" << endl;
	msg << "kNthreads = number of threads" << endl;
	msg << "kNiter = number of test runs (iterations)" << endl;
	msg << separator << endl;
	msg << "aborting.." << endl;
	cerr << msg.str() << endl;
	terminate();
}

// Function to calculate mean and std dev of test run timings
string calcMeanStd(const vector<size_t> &results) {

	// mean
	double sum = std::accumulate(results.begin(), results.end(), 0.0);
	double mean = sum / results.size();

	// std dev
	double accum = 0.0;
	std::for_each(results.begin(), results.end(), [&](const double d) {
		accum += (d - mean) * (d - mean);
	});
	double stdev = sqrt(accum / (results.size() - 1));

	// write to string
	ostringstream os;
	os.precision(3);
	os << mean << " ± " << stdev;
	return os.str();
}

template<typename T>
void addElements(vector<T> &v, const size_t Nel) {
	for (size_t ii = 0; ii < Nel; ++ii)
		v.emplace_back(ii);
}

int main(int argc, char *argv[]) {

	if (argc < 3)
		usageMsg();

	// Timer
	Timer timer;

	// Print format parameters
	string separator(50, '-');
	const size_t kNsetwText = 25;
	const size_t kNsetwNumber = 10;

	// Test parameters
	const size_t kNelements = stoi(string(argv[1])); // number of elements to accumulate
	const size_t kNthreads = stoi(string(argv[2])); // number of threads for parallel accumulate
	const size_t kNiter = stoi(string(argv[3])); // number of test runs (iterations)
	vector<size_t> results; // container of results (timings of all test runs)

	cout << "Nelements: " << kNelements << endl;
	cout << "Nthreads: " << kNthreads << endl;
	cout << "Niter: " << kNiter << endl;

	vector<int> elements;
	elements.reserve(kNelements);
	addElements(elements, kNelements);
	size_t finalSum;

	{
		// Serial accumulate
		results.clear();
		for (size_t iterNo = 0; iterNo < kNiter; ++iterNo) {
			timer.start();
			finalSum = accumulate(elements.begin(), elements.end(), 0);
			timer.stop();
			results.push_back(timer.duration());
		}

		// Report result
		cout << separator << endl;
		cout << "Serial accumulate (avg of " << kNiter << " runs)" << endl;
		cout << "Sum: " << finalSum << endl;
		cout << "Test duration: " << setw(kNsetwNumber)
				<< calcMeanStd(results) << " [ms]" << endl;
		cout << separator << endl;
	}

	{
		// Parallel accumulate
		results.clear();
		for (size_t iterNo = 0; iterNo < kNiter; ++iterNo) {
			timer.start();
			finalSum = parallel_accumulate(elements.begin(), elements.end(), 0, kNthreads);
			timer.stop();
			results.push_back(timer.duration());
		}

		// Report result
		cout << separator << endl;
		cout << "Parallel accumulate (avg of " << kNiter << " runs)" << endl;
		cout << "Sum: " << finalSum << endl;
		cout << "Test duration: " << setw(kNsetwNumber)
				<< calcMeanStd(results) << " [ms]" << endl;
		cout << separator << endl;
	}

	return 0;
}
