#include <iostream>
using namespace std;

int main() {
	char line[81];
	bool isAnt = false;
	while (cin.good()) {
		cin.getline(line, sizeof(line));
		if (line == string("END")) {
			if (isAnt) {
				cout << "SUICIDE\n";
				isAnt = false;
			}
			cout << "END\n";
			cout.flush();
		} else if (line == string("BEGIN ANT")) {
			isAnt = true;
		}
	}
}
