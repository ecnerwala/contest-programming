#include "dna.h"

#include <bits/stdc++.h>

namespace ecnerwala {
std::vector<std::array<int, 9>> pref;
}

void init(std::string A, std::string B) {
	using namespace ecnerwala;
	assert(A.size() == B.size());
	int N = int(A.size());
	pref.resize(N+1);
	pref[0].fill(0);
	auto char_to_val = [](char c) -> int {
		switch (c) {
			case 'A': return 0;
			case 'C': return 1;
			case 'T': return 2;
			default: assert(false);
		}
	};
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i];
		++pref[i+1][char_to_val(A[i]) * 3 + char_to_val(B[i])];
	}
}

int get_distance(int x, int y) {
	using namespace ecnerwala;
	y++;
	assert(0 <= x && x < y && y < int(pref.size()));

	std::array<int, 9> cnt;
	for (int i = 0; i < 9; i++) cnt[i] = pref[y][i] - pref[x][i];

	int d1 = cnt[1] - cnt[3];
	int d2 = cnt[5] - cnt[7];
	int d3 = cnt[6] - cnt[2];
	if (d1 != d2 || d2 != d3) return -1;
	int ans = 0;
	ans += std::min(cnt[1], cnt[3]);
	ans += std::min(cnt[2], cnt[6]);
	ans += std::min(cnt[7], cnt[5]);
	ans += std::abs(d1) * 2;
	return ans;
}
