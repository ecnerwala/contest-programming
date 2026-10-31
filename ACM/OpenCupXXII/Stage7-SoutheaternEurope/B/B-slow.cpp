#include <bits/stdc++.h>
using namespace std;

int main() {
	std::ios_base::sync_with_stdio(0), std::cin.tie(nullptr);
	int K, N, Q; cin >> K >> N >> Q;

	std::vector<std::vector<std::vector<int>>> A(Q+1);
	A[0] = std::vector<std::vector<int>>(N, std::vector<int>(K));
	for (int k = 0; k < K; k++) {
		for (int i = 0; i < N; i++) {
			cin >> A[0][i][k];
		}
	}

	for (int q = 1; q <= Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int t; cin >> t;
			assert(0 <= t && t < q);
			assert(!A[t].empty());
			int p; cin >> p; p--;
			assert(0 <= p && p < K);
			int l, r; cin >> l >> r; l--;
			assert(0 <= l && l < r && r <= N);
			int x; cin >> x;
			A[q] = A[t];
			for (int i = l; i < r; i++) A[q][i][p] += x;

			for (int i = 0; i < N; i++) {
				int b = 0;
				for (int k = 0; k < K; k++) {
					b += A[q][i][k];
				}
				cout << b << " \n"[i+1==N];
			}
		} else if (op == 2) {
			int t; cin >> t;
			assert(0 <= t && t < q);
			assert(!A[t].empty());
			int p; cin >> p; p--;
			assert(0 <= p && p < K);
			int l, r; cin >> l >> r; l--;
			assert(0 <= l && l < r && r <= N);
			int x; cin >> x;
			A[q] = A[t];
			for (int i = l; i < r; i++) A[q][i][p] = x;

			for (int i = 0; i < N; i++) {
				int b = 0;
				for (int k = 0; k < K; k++) {
					b += A[q][i][k];
				}
				cout << b << " \n"[i+1==N];
			}
		} else if (op == 3) {
			int t; cin >> t;
			assert(0 <= t && t < q);
			assert(!A[t].empty());
			int l, r; cin >> l >> r; l--;
			assert(0 <= l && l < r && r <= N);
			int best = 2e9;
			for (int i = l; i < r; i++) {
				int b = 0;
				for (int k = 0; k < K; k++) {
					b += A[t][i][k];
				}
				best = std::min(best, b);
			}
			cout << best << '\n';
		} else assert(false);
	}
}
