#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(N); for (auto& b : B) cin >> b;

		{
			std::pair<int64_t, int> pref{0, 0};
			std::pair<int64_t, int> min_pref{0, 0};
			for (int i = 0; i < N; i++) {
				pref.first += A[i];
				pref.first -= B[i];
				pref.second = i+1;
				min_pref = std::min(min_pref, pref);
			}
			assert(pref.first <= 0);
			std::rotate(A.begin(), A.begin() + min_pref.second, A.end());
			std::rotate(B.begin(), B.begin() + min_pref.second, B.end());
		}
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		std::vector<int64_t> cnt_dist(N+1);
		for (int i = 0; i < N; i++) {
			stk.push_back({A[i], i});
			int b = B[i];
			while (!stk.empty() && b > 0) {
				int t = std::min(stk.back().first, b);
				int dist = i + 1 - stk.back().second;
				cnt_dist[dist] += t;
				stk.back().first -= t;
				b -= t;
				if (stk.back().first == 0) stk.pop_back();
			}
		}
		assert(stk.empty());
		int ans = N;
		while (ans > 0) {
			if (cnt_dist[ans] == 0) ans--;
			else if (K > 0) { 
				int64_t v = std::min(K, cnt_dist[ans]);
				cnt_dist[ans] -= v;
				K -= v;
			} else break;
		}
		cout << ans << '\n';
	}

	return 0;
}
