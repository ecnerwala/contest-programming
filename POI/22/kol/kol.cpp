#include "ckollib.h"

#include<bits/stdc++.h>

int main() {
	int first_card = -1;
	int first_cnt = 0;
	int second_card = -1;
	int second_cnt = 0;

	std::array<int, 32> bit_cnt; bit_cnt.fill(0);

	for (int v; (v = karta()) != 0; ) {
		if (first_card == -1 || v == first_card) {
			first_card = v;
			first_cnt++;
		} else if (second_card == -1 || v == second_card) {
			second_card = v;
			second_cnt++;
		}
		for (; v; v &= v-1) {
			bit_cnt[__builtin_ctz(v)]++;
		}
	}

	assert(first_card != -1);
	if (second_card == -1) {
		odpowiedz(first_card);
		exit(0);
	}

	int L = std::max(first_cnt, second_cnt);
	int ans = 0;
	for (int i = 0; i < 32; i++) {
		if (bit_cnt[i] % L) {
			ans |= (1 << i);
		}
	}
	odpowiedz(ans);
	exit(0);

	return 0;
}
