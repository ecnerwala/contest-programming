#include "souvenirs.h"

#include <bits/stdc++.h>

using i64 = long long;
void buy_souvenirs(int N, i64 P0) {
	std::vector<i64> P(N);
	std::vector<int> freq(N, 0);
	std::vector<std::pair<std::vector<int>, i64>> txs;
	txs.reserve(N);
	txs.push_back({{0}, P0});
	int known = N;
	while (known > 0) {
		while (txs.back().first.back() >= known) {
			txs.back().second -= P[txs.back().first.back()];
			txs.back().first.pop_back();
		}
		if (txs.back().first[0] == known-1) {
			P[--known] = txs.back().second;
			txs.pop_back();
		} else {
			i64 v = (txs.back().second - 1) / int(txs.back().first.size());
			txs.emplace_back(transaction(v));
			txs.back().second = v - txs.back().second;
			for (auto i : txs.back().first) freq[i]++;
		}
	}

	for (int i = 0; i < N; i++) {
		for (; freq[i] < i; freq[i]++) {
			transaction(P[i]);
		}
	}
}
