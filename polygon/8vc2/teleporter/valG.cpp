#include "testlib.h"

int main() {
	registerValidation();
	int N = inf.readInt(3, 399);
	inf.readEoln();
	quitif(N % 2 == 0, _fail, "Size of grid must be odd");

	bool has_robot = false;

	for(int i = 0; i < N; i++) {
		std::string row = inf.readToken();
		inf.readEoln();
		quitif(int(row.size()) != N, _fail, "Expected a string of length n");
		for(int j = 0; j < N; j++) {
			quitif(!(row[j] == '.' || row[j] == 'X' || row[j] == 'O'), _fail, "Invalid character in square");

			bool is_center = (i == (N - 1) / 2 && j == (N - 1) / 2);
			quitif((row[j] == 'O') != is_center, _fail, "Teleporter must be in the center square");

			if(row[j] == 'X') has_robot = true;
		}
	}

	quitif(!has_robot, _fail, "Must have at least one robot");

	inf.readEof();

	return 0;
}
