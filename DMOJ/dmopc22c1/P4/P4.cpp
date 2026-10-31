#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, T; cin >> N >> K >> T;
	while (T--) {
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;
		std::vector<int> A; A.reserve(N);
		for (int i = 0; i < N; i++) {
			A.insert(A.begin() + B[i], i);
		}
		std::vector<int> cnts; cnts.reserve(K);
		int prv = 0;
		for (int i = 1; i < N; i++) {
			if (A[i] > A[i-1]) {
				cnts.push_back(i - prv);
				prv = i;
			}
		}
		cnts.push_back(N - prv);
		while (int(cnts.size()) < K) {
			cnts.push_back(0);
		}
		for (int i = 0; i < K; i++) {
			cout << cnts[i] << " \n"[i+1==K];
		}
	}

	return 0;
}
