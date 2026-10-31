#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		if (K >= N && K <= 2*N-1) {
			cout << "YES" << '\n';
			int N2 = K-N+1;
			std::vector<int> A; A.reserve(2*N);
			while (N >= N2) {
				N--;
				A.push_back(N);
				A.push_back(N);
			}
			if (N == 1) {
				A.push_back(0);
				A.push_back(0);
			} else {
				A.push_back(0);
				A.push_back(1);
				for (int i = 1; i+1 < N; i++) {
					A.push_back(i+1);
					A.push_back(i-1);
				}
				A.push_back(N-2);
				A.push_back(N-1);
			}
			for (int i = 0; i < int(A.size()); i++) {
				cout << A[i]+1 << " \n"[i+1==int(A.size())];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
