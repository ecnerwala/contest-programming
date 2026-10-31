#include "Anna.h"
#include <vector>

namespace {

int N, L, R;
int idx;
int ans;

}  // namespace

void InitA(int N_, int L_, int R_) {
	N = N_;
	L = L_;
	R = R_;
	for (int i = 19; i >= 11; i--) {
		SendA((L >> i) & 1);
	}
	for (int i = 19; i >= 11; i--) {
		SendA((R >> i) & 1);
	}
	if ((L >> 11) == (R >> 11)) {
	}
	idx = L;
	ans = L;
}

void ReceiveA(bool x) {
	++idx;
	if (idx <= R && x) {
		ans = idx;
	}
}

int Answer() {
	return ans;
}
