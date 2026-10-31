#include <bits/stdc++.h>

int sushi(int N, int B, std::vector<int> A) {
	assert(N == int(A.size()));
	using bs = std::bitset<100001>;
	assert(B <= 100000);
	bs poss;
	poss[0] = true;
	for (int z = 1; N > 0; z++) {
		int NN = 0;
		for (int i = 0; i < N; i++) {
			bs nposs = poss << A[i];
			nposs |= poss;
			if (nposs == poss) {
				continue;
			}
			A[NN++] = A[i];
			poss = nposs;
			if (poss[B]) return z;
		}
		N = NN;
	}
	return -1;
}
