#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		int tot_xor = 0;
		for (auto a : A) tot_xor ^= a;

		std::array<int, 33> B_effect; B_effect.fill(0);
		auto get_B_effect = [&](int a) -> int {
			if (a == 0) return 32;
			else return __builtin_ctz(a);
		};
		for (auto a : A) {
			B_effect[get_B_effect(a)]++;
		}

		int ans = 0;
		for (auto a : A) {
			tot_xor ^= a;
			tot_xor ^= a+1;
			B_effect[get_B_effect(a)]--;
			B_effect[get_B_effect(a+1)]++;

			int cur_val = (1 << 31) - 1;
			for (int v = 0; v < 32; v++) {
				if (B_effect[v] > 0) {
					cur_val = std::min(cur_val, tot_xor ^ ((1 << (v+1)) - 1));
				}
			}

			ans = std::max(ans, cur_val);

			B_effect[get_B_effect(a+1)]--;
			B_effect[get_B_effect(a)]++;
			tot_xor ^= a+1;
			tot_xor ^= a;
		}
		cout << ans << '\n';
	}

	return 0;
}
