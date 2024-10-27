#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		// we want to maximize the # of correct tests
		std::vector<int> cnd(N, -1);
		for (int e = 0; e < M; e++) {
			int x, k; cin >> x >> k; x--, k--;
			assert(cnd[x] == -1);
			cnd[x] = k;
		}
		std::vector<int> check_time(N-1);
		std::vector<int> nxt_smaller(N-1);
		std::vector<int> prv_smaller(N-1);
		[&](this auto&& self, int l, int h, int d) -> void {
			if (l == h) return;
			int m = (l + h) / 2;
			check_time[m] = d;
			prv_smaller[m] = l-1;
			nxt_smaller[m] = h;
			self(l, m, d+1);
			self(m+1, h, d+1);
		}(0, N-1, 0);
	}

	return 0;
}
