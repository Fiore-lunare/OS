#include <iostream>
#include <string>
#include <sstream>


const int N = 2;
using namespace std;

int main() {
	std::string str;
	double x;

	if (!std::getline(cin, str)) {
		return 1;
	}

	stringstream ss(str);
	while (ss >> x) {
		cout << x + N<< " ";
	}
	cout << "\n";
	return 0;
}