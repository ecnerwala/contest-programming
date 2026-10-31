#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		// we have nxt smaller, nxt bigger
		std::vector<int> bad_right;
		std::vector<int> bad_left;
		for (int dir = 0; dir < 2; dir++, std::reverse(A.begin(), A.end()), std::swap(bad_right, bad_left)) {
			bad_right.assign(N, N);
			std::vector<std::pair<int, int>> stk_lo; stk_lo.reserve(N+1);
			std::vector<std::pair<int, int>> stk_hi; stk_hi.reserve(N+1);
			stk_lo.push_back({-1, -1});
			stk_hi.push_back({N, -1});
			for (int i = 0; i < N; i++) {
				int v = A[i];
				while (v < stk_lo.back().first) stk_lo.pop_back();
				while (v > stk_hi.back().first) stk_hi.pop_back();

				int b = std::min(stk_lo.back().second, stk_hi.back().second);
				if (b >= 0) {
					bad_right[b] = std::min(bad_right[b], i);
				}

				stk_lo.push_back({v, i});
				stk_hi.push_back({v, i});
			}
			for (int i = N-1; i > 0; i--) {
				bad_right[i-1] = std::min(bad_right[i-1], bad_right[i]);
			}
		}
		std::reverse(bad_left.begin(), bad_left.end());
		for (auto& a : bad_left) { a = N-1-a; }

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += int64_t(bad_right[i] - i) * int64_t(i - bad_left[i]);
		}
		for (int i = 0; i+1 < N; i++) {
			if (bad_right[i] >= i+1 && bad_left[i+1] <= i) {
				ans -= int64_t(bad_right[i] - i+1) * int64_t(i - bad_left[i+1]);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
