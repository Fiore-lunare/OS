#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    std::string str;
    double x;

    getline(cin, str);
    stringstream ss(str);
    while (ss >> x) {
        cout << x*7<< " ";
    }
    cout << "\n";
    return 0;
}
