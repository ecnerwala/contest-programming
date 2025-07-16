#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A; cin >> A;
	int64_t N; cin >> N;
	std::vector<int64_t> p10;
	p10.push_back(1);
	int64_t ans = 0;
	for (int L = 1; p10.back() <= N; L++) {
		p10.push_back(p10.back() * 10);
		[&](this auto&& self, int i, int j, int64_t cur) -> void {
			if (i > j) {
				int64_t rev_A = 0;
				for (int64_t v = cur; v; v /= A) rev_A = rev_A * A + v % A;
				if (rev_A == cur) {
					ans += cur;
				}
				return;
			}
			for (int d = (i == 0 ? 1 : 0); d < 10; d++) {
				self(i+1, j-1, cur + (i < j ? p10[i] + p10[j] : p10[i]) * d);
			}
		}(0, L-1, 0);
	}
	cout << ans << '\n';

	return 0;
}
