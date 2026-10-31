#include "Bruno.h"
#include <vector>

namespace {

int N;
std::vector<int> P;

int L = 0;
int idx = 0;

}  // namespace

void InitB(int N_, std::vector<int> P_) {
	::N = N_;
	::P = std::move(P_);
}

void ReceiveB(bool y) {
	if (idx < 15) {
		L |= (y << idx);
		++idx;
	}
	if (idx == 15) {
		int cur_min = P[L];
		for (int i = L+1; i < N; i++) {
			if (P[i] < cur_min) {
				cur_min = P[i];
				SendB(true);
			} else {
				SendB(false);
			}
		}
	}
}
