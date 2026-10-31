#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }

	int num_mismatch = 0;
	int num_parity_mismatch = 0;
	bool cur_parity = false;
	{
		for (int i = 0; i < N; i++) {
			num_mismatch += (A[i] != i);
			num_parity_mismatch += ((A[i]&1) != (i&1));
		}
		std::vector<int> B = A;
		for (int i = 0; i < N; i++) {
			while (B[i] != i) {
				std::swap(B[i], B[B[i]]);
				cur_parity ^= 1;
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		int u, v; cin >> u >> v; u--, v--;

		num_mismatch -= (A[u] != u);
		num_mismatch -= (A[v] != v);
		num_parity_mismatch -= ((A[u]&1) != (u&1));
		num_parity_mismatch -= ((A[v]&1) != (v&1));

		cur_parity ^= 1;
		std::swap(A[u], A[v]);

		num_mismatch += (A[u] != u);
		num_mismatch += (A[v] != v);
		num_parity_mismatch += ((A[u]&1) != (u&1));
		num_parity_mismatch += ((A[v]&1) != (v&1));

		if (cur_parity == 1) {
			cout << N << '\n';
		} else if (num_mismatch == 0) {
			cout << -1 << '\n';
		} else if (num_parity_mismatch == 0) {
			cout << N-2 << '\n';
		} else {
			cout << N-1 << '\n';
		}
	}

	return 0;
}
