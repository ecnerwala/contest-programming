#include "registers.h"

#include <bits/stdc++.h>

void construct_instructions(int S, int N, int K, int Q) {
	const int B = 2000;

	{
		std::vector<bool> tmp(B);
		std::fill(tmp.begin()+N*K, tmp.end(), true);
		append_store(1, tmp);
		append_or(0, 0, 1);
	}

	int L = 8*sizeof(N) - __builtin_clz(N-1);
	N = (1 << L);

	if (S == 0) {
		const int K_MSK = 99;
		{
			std::vector<bool> tmp(B);
			for (int i = 0; i < B; i += K) tmp[i] = true;
			append_store(K_MSK, tmp);
		}

		while (N > 1) {
			append_right(1, 0, N/2*K);
			append_xor(2, 0, 1);
			append_not(3, 0);
			append_add(3, 3, 1);
			append_xor(3, 3, 2);
			append_and(3, 3, K_MSK);
			append_right(4, 3, K);
			append_not(4, 4);
			append_add(3, 3, 4);
			append_and(2, 2, 3);
			append_xor(0, 0, 2);
			N /= 2;
		}
	} else if (S == 1) {
		const int K_MSK_OFF = 90;
		for (int p = 0; p < L; p++) {
			std::vector<bool> tmp(B);
			tmp[0] = true;
			for (int i = 0; (i+1)*K < B; i ++) tmp[(i+1)*K] = !((i >> p) & 1);
			append_store(K_MSK_OFF+p, tmp);
		}
		for (int z = 0; z < L; z++) {
			const int REV = 5;
			if (z+1 < L) append_not(REV, K_MSK_OFF+z+1);
			for (int p = z; p >= 0; p--) {
				append_right(1, 0, (1 << p) * K);
				append_xor(2, 0, 1);
				append_not(3, 0);
				append_add(3, 3, 1);
				append_xor(3, 3, 2);
				if (z+1 < L) append_xor(3, 3, REV);
				append_and(3, 3, K_MSK_OFF + p);
				append_right(4, 3, K);
				append_not(4, 4);
				append_add(3, 3, 4);
				append_and(2, 2, 3);
				append_xor(0, 0, 2);
				append_left(2, 2, (1 << p) * K);
				append_xor(0, 0, 2);
			}
		}
	} else assert(false);
}
