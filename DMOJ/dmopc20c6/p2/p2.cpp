#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N); for (auto& b : B) cin >> b;

	vector<pair<int, int>> ops; ops.reserve(N);
	int la = N-1;
	while (N--) {
		if (B[N] == A[N]) continue;
		if (B[N] == 0 && A[N] == 1) {
			cout << -1 << '\n';
			exit(0);
		}
		assert(B[N] == 1 && A[N] == 0);
		int c1 = 0;
		while (B[N - c1] == 1) c1++;
		assert(c1 > 0);
		int nxt = N;
		la = min(la, N);
		while (c1 > 0) {
			if (la < 0) {
				cout << -1 << '\n';
				exit(0);
			}
			if (A[la] == 1) {
				A[la] = 0;
				A[nxt] = 1;
				nxt--;
				c1--;
			}

			la--;
		}
		ops.push_back({la+1, N});
		assert(A[N] == B[N]);
	}

	cout << ops.size() << '\n';
	for (auto [l, r] : ops) {
		cout << l+1 << ' ' << r+1 << '\n';
	}

	return 0;
}
