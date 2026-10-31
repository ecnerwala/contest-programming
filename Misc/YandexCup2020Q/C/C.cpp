#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		array<vector<int>, 2> A;
		for (vector<int>& v : A) {
			int N; cin >> N; v.resize(N);
			for (auto& x : v) cin >> x;
			sort(v.begin(), v.end());

			if (!v.empty()) {
				int lo = v.front();
				int g = 0;
				for (auto& x : v) {
					x -= lo;
					g = gcd(g, x);
				}
				for (auto& x : v) {
					if (g == 0) {
						assert(x == 0);
					} else {
						x /= g;
					}
				}
			}
		}
		bool is_good;
		if (A[0] == A[1]) {
			is_good = true;
		} else {
			reverse(A[1].begin(), A[1].end());
			if (!A[1].empty()) {
				int lo = A[1].front();
				for (auto& x : A[1]) {
					x = lo - x;
				}
			}
			is_good = (A[0] == A[1]);
		}

		cout << (is_good ? "YES" : "NO") << '\n';
	}

	return 0;
}
