#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int M, N, K; cin >> M >> N >> K;
	vector<bool> A(M);
	vector<bool> B(N);
	int cA = 0, cB = 0;
	while (K--) {
		char c; cin >> c;
		int v; cin >> v; v--;
		if (c == 'R') {
			if (A[v]) {
				cA--;
				A[v] = false;
			} else {
				cA++;
				A[v] = true;
			}
		} else if (c == 'C') {
			if (B[v]) {
				cB--;
				B[v] = false;
			} else {
				cB++;
				B[v] = true;
			}
		} else assert(false);
	}

	cout << int64_t(cA) * int64_t(N - cB) + int64_t(M - cA) * int64_t(cB) << '\n';

	return 0;
}
