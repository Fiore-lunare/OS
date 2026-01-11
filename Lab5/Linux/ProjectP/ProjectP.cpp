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
        cout << x * x *x << " ";
    }
    cout << "\n";
    return 0;
}
