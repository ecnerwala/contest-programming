#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int L = __builtin_ctz(N);
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::array<int64_t, 2>> cnt_inv(L, std::array<int, 2>({0, 0}));

	std::vector<int> trie(2*N);

	for (int a : A) {
		int cur = N + a;
		for (int i = 0; i < L; i++, cur >>= 1) {
			trie[cur]++;
			int ocnt = trie[cur ^ 1];
			cnt_inv[i][(cur & 1)] += ocnt;
		}
	}

	int64_t ans_0 = 0;
	int64_t ans_1 = 1;
	for (int i = 0; i < L; i++) {
		ans_0 += cnt_inv[i][0];
		ans_1 += std::min(cnt_inv[i][0], cnt_inv[i][1]);
	}
	cout << std::min(ans_0, ans_1) << '\n';

	return 0;
}
