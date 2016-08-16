#include "testlib.h"
#include <unordered_set>

int main() {

	registerValidation();

	int N = inf.readInt(1, 200000);
	inf.readEoln();

	std::unordered_set<int> vals;
	for(int i = 0; i < N; i++) {
		int v = inf.readInt(0, 1000000000);
		if(i + 1 < N) inf.readSpace();

		quitif(vals.count(v), _fail, "Elements should be distinct");
		vals.insert(v);
	}

	inf.readEoln();

	inf.readEof();

	return 0;
}
