#include<bits/stdc++.h>

#define TESTING 0

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

#if TESTING
	const int test_N = 320;
	vector<int> test_R(test_N);
	mt19937 mt(48);
	test_R[0] = mt() & 255;
	test_R[1] = mt() & 255;
	test_R[2] = mt() & 255;
	for (int i = 3; i < test_N; i++) {
		test_R[i] = test_R[i-2] ^ test_R[i-3];
	}
	vector<int> test_M(256);
	iota(test_M.begin(), test_M.end(), 0);
	shuffle(test_M.begin(), test_M.end(), mt);

	int test_ind = 0;
#endif

	vector<uint32_t> basis;

	vector<uint32_t> msk_rel(7);
	msk_rel[0] = 1 << 8;
	msk_rel[1] = 2 << 8;
	msk_rel[2] = 4 << 8;
	for (int i = 3; i < 7; i++) {
		msk_rel[i] = msk_rel[i-2] ^ msk_rel[i-3];
	}
	for (int i = 7; i < 10; i++) {
		assert((msk_rel[(i-2)%7] ^ msk_rel[i-3]) == msk_rel[i-7]);
	}

	vector<bool> asked(256 * 8);

	const uint32_t BAD = ~uint32_t(0);
	vector<uint32_t> invM(256, BAD);
	int num_fixed = 0;

	for (int z = 0; int(basis.size()) < 3 || num_fixed < 256; z++) {
		uint32_t m = msk_rel[z%7];
		uint32_t target = BAD;
		if ((m >> 8) == 1) {
			for (int i = 0; i < 256; i += 16) {
				uint32_t cnd = ((m >> 8) << 8) | i;
				if (!asked[cnd]) {
					target = cnd;
					break;
				}
			}
		}
		if (target == BAD) {
			for (int i = 0; i < 256; i++) {
				uint32_t cnd = ((m >> 8) << 8) | i;
				if (!asked[cnd]) {
					target = cnd;
					break;
				}
			}
		}
		assert(target != BAD);
		asked[target] = true;
		cout << (m ^ target) << '\n' << flush;
#if TESTING
		uint32_t resp = test_M[(m ^ target) ^ test_R[test_ind++]];
		cerr << "> " << resp << '\n';
#else
		uint32_t resp; cin >> resp;
#endif
		if (invM[resp] == BAD) {
			invM[resp] = target;
			num_fixed++;
		} else {
			uint32_t vec = target ^ invM[resp];
			for (auto b : basis) vec = min(vec, b ^ vec);
			for (auto& b : basis) b = min(b, b ^ vec);

			basis.push_back(vec);

			for (int i = 0; i < int(asked.size()); i++) {
				if (asked[i]) asked[i^vec] = true;
			}

			for (auto& v : msk_rel) {
				v = min(v ^ 256, v ^ 256 ^ vec) ^ 256;
			}
		}
	}

	cout << "SOLUTION" << '\n';
	sort(basis.begin(), basis.end());
	assert((basis[0] >> 8) == 1);
	assert((basis[1] >> 8) == 2);
	assert((basis[2] >> 8) == 4);
	cout << (basis[0] & 255) << '\n';
	cout << (basis[1] & 255) << '\n';
	cout << (basis[2] & 255) << '\n';
#if TESTING
	assert((basis[0] & 255) == uint32_t(test_R[0]));
	assert((basis[1] & 255) == uint32_t(test_R[1]));
	assert((basis[2] & 255) == uint32_t(test_R[2]));
#endif

	vector<int> M(256);
	for (int i = 0; i < 256; i++) {
		assert(invM[i] != BAD);
		for (auto b : basis) invM[i] = min(invM[i], invM[i] ^ b);
		assert(invM[i] < 256);
		M[invM[i]] = i;
	}
	for (int i = 0; i < 256; i++) {
		cout << M[i] << '\n';
	}
#if TESTING
	assert(M == test_M);
#endif

	return 0;
}

// [a, b, c, a^b, b^c, a^b^c, a^c, a, b, c]
