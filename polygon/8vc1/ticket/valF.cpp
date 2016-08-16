#include "testlib.h"
#include <unordered_map>

int main() {
	registerValidation();

	int N, T, Q;
	N = inf.readInt(1, 200000);
	inf.readSpace();
	T = inf.readInt(1, 200000);
	inf.readSpace();
	Q = inf.readInt(1, 200000);
	inf.readEoln();

	for(int i = 0; i < N; i++) {
		inf.readInt(1, 1000);
		if(i + 1 < N) inf.readSpace();
	}
	inf.readEoln();

	std::unordered_map<int, int> cnt;
	for(int i = 0; i < Q; i++) {
		int t, r;
		t = inf.readInt(1, 2);
		inf.readSpace();
		r = inf.readInt(1, N);
		inf.readEoln();
		if(t == 1) {
			cnt[r] ++;
		} else {
			quitif(cnt[r] == 0, _fail, "Removed non-existent ticket");
			cnt[r] --;
		}
	}

	inf.readEof();

	return 0;
}
