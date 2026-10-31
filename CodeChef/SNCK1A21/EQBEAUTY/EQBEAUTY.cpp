#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());

		assert(N >= 2);
		int64_t ans = A[N-1] - A[0];
		for (int l = 1, r = N-2; l < r; ) {
			int64_t s = (A[l] + A[r]) - (A[0] + A[N-1]);
			ans = min(ans, abs(s));
			if (s > 0) {
				r--;
			} else {
				l++;
			}
		}
		auto get_cost = [](auto beg, auto end) {
			int64_t res = 0;
			auto v = *(beg + int(end - beg)/2);
			for (auto it = beg; it != end; ++it) {
				res += abs(v - *it);
			}
			return res;
		};
		ans = min(ans, get_cost(A.begin(), A.end()-1));
		ans = min(ans, get_cost(A.begin()+1, A.end()));
		cout << ans << '\n';
	}

	return 0;
}
