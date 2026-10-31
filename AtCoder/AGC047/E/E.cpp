#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	const __int128_t TEST_A = 1e9;
	const __int128_t TEST_B = 1e9;
	vector<__int128_t> regs(200000, 0);
	regs[0] = TEST_A;
	regs[1] = TEST_B;

	vector<tuple<char, int, int, int>> ops;
	auto plus = [&](int i, int j, int k) {
		regs[k] = regs[i] + regs[j];
		ops.emplace_back('+', i, j, k);
	};
	auto less = [&](int i, int j, int k) {
		regs[k] = regs[i] < regs[j];
		ops.emplace_back('<', i, j, k);
	};

	const int A[2] = {0, 1};
	const int R = 2;
	const int ZERO = 3;
	const int ONE = 4;
	plus(A[0], A[1], ONE);
	less(ZERO, ONE, ONE);

	auto neg = [&](int i, int j) {
		less(i, ONE, j);
	};

	auto either = [&](int i, int j, int k) {
		plus(i, j, k);
		less(ZERO, k, k);
	};
	const int SCRATCH1 = 5;
	const int SCRATCH2 = 6;
	auto both = [&](int i, int j, int k) {
		neg(i, SCRATCH1);
		neg(j, SCRATCH2);
		either(SCRATCH1, SCRATCH2, k);
		neg(k, k);
	};

	const int LG = 30;

	const int A_BITS[2] = {1000, 2000};
	const int SCRATCH = 10000;
	for (int z = 0; z < 2; z++) {
		plus(ZERO, ZERO, SCRATCH);
		for (int i = LG-1; i >= 0; i--) {
			plus(SCRATCH, SCRATCH, SCRATCH);
			plus(SCRATCH, ZERO, A_BITS[z]+i);
			for (int j = 0; j < i; j++) {
				plus(A_BITS[z]+i, A_BITS[z]+i, A_BITS[z]+i);
				plus(A_BITS[z]+i, ONE, A_BITS[z]+i);
			}
			less(A_BITS[z]+i, A[z], A_BITS[z]+i);
			plus(SCRATCH, A_BITS[z]+i, SCRATCH);
		}
	}

	const int R_BITS = 6000;
	for (int i = 0; i < LG; i++) {
		for (int j = 0; j < LG; j++) {
			both(A_BITS[0]+i, A_BITS[1]+j, SCRATCH);
			plus(R_BITS+i+j, SCRATCH, R_BITS+i+j);
		}
	}
	for (int i = 2*(LG-1); i >= 0; i--) {
		plus(R, R, R);
		plus(R, R_BITS+i, R);
	}

	assert(regs[2] == TEST_A * TEST_B);

	cout << ops.size() << '\n';
	for (auto [op, i, j, k] : ops) {
		cout << op << ' ' << i << ' ' << j << ' ' << k << '\n';
	}

	return 0;
}
