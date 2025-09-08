#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// we can set Z[m] = B[m] when max(A[m], B[m]) <= max(A[l..m-1]) or A[m] == B[m]
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		std::vector<int> B(N);
		for (auto& a : A) cin >> a;
		for (auto& b : B) cin >> b;
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int mi = -1, ma = int(stk.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (stk[md].first >= std::max(A[i], B[i])) {
					mi = md;
				} else {
					ma = md;
				}
			}
			int lo = (mi == -1 ? -1 : stk[mi].second);
			// we need l <= lo and r >= i
			ans += int64_t(lo + 1) * int64_t(N - i);
			while (stk.back().first <= A[i]) {
				stk.pop_back();
			}
			stk.push_back({A[i], i});
		}
		cout << ans << '\n';
	}

	return 0;
}
