#include "souvenirs.h"

#include <bits/stdc++.h>

using i64 = long long;
void buy_souvenirs(int N, i64 P0) {
	std::vector<i64> P(N);
	std::vector<int> freq(N, 0);
	struct tx_t {
		std::vector<int> inds;
		i64 val;
	};
	std::vector<tx_t> txs; txs.reserve(N);
	txs.push_back({{0}, P0});
	int known = N;
	while (known > 0) {
		auto& tx = txs.back();
		while (tx.inds.back() >= known) {
			tx.val -= P[tx.inds.back()];
			tx.inds.pop_back();
		}
		if (tx.inds[0] == known-1) {
			P[tx.inds[0]] = tx.val;
			known--;
			txs.pop_back();
		} else {
			i64 val = (tx.val - 1) / int(tx.inds.size());
			auto [inds, rem] = transaction(val);
			for (auto i : inds) freq[i]++;
			txs.emplace_back(std::move(inds), val-rem);
		}
	}

	for (int i = 0; i < N; i++) {
		for (; freq[i] < i; freq[i]++) {
			transaction(P[i]);
		}
	}
}
