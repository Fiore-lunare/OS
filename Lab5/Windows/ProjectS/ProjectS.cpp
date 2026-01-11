#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	std::string str;
	double x;
	double res = 0.0;

	if (!std::getline(cin, str)) {
		return 1;
	}

	stringstream ss(str);
	while (ss >> x) {
		res += x;
	}
	cout << res<< "\n";
	return 0;
}

