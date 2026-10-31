#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	// After 2k moves, you go from 2*n to floor(2n / 2^k)
	// So number of moves is first k so that 2^k > 2n
	// So number of moves is 2*(ceiling(log_2(n))+2)
	// Each pile after the first is 2*(ceiling(log_2(n-1))+2) because of a forced rebalance
	// You get -1 at the end as well
	array<int, 34> remove_cnt; remove_cnt.fill(0);
	int64_t ans = 0;
	for (auto a : A) {
		a *= 2;
		int hi_bit = 31 - __builtin_clz(a);
		ans += hi_bit+1;
		// cost to reduce number of moves by 1 is # of leading 1s in the binary representation
		int remove_cost = hi_bit - (31 - __builtin_clz((1 << hi_bit) ^ a ^ (a>>1)));
		remove_cnt[remove_cost]++;
	}
	for (int i = 32; true; ) {
		while (!remove_cnt[i]) i--;
		if (i > 1 && remove_cnt[i] >= 2) {
			remove_cnt[i-1] += remove_cnt[i] / 2;
			remove_cnt[i] %= 2;
			continue;
		}
		remove_cnt[i]--;
		while (i > 1 && !remove_cnt[i]) i--;
		if (i == 1) {
			ans -= remove_cnt[i];
			break;
		}
		remove_cnt[i]--;
		remove_cnt[i-1]++;
	}
	ans *= 2;
	ans--;
	cout << ans << '\n';

	return 0;
}
