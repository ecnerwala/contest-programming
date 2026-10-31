#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	vector<int> A(N+1); for (auto& a : A) cin >> a;

	int K = abs(A[1] - A[0]);
	for (int i = 0; i < N; i++) {
		K = min(K, abs(A[i+1] - A[i]));
	}

	vector<int> B(N+1);
	cout << K << '\n';
	while (K > 1) {
		for (int i = 0; i <= N; i++) {
			B[i] = 0;
		}
		for (int i = 0; i < N; i++) {
			if (S[i] == '<') {
				B[i+1] = max(B[i+1], B[i] + 1);
			} else {
				B[i+1] = max(B[i+1], B[i] - (A[i] - A[i+1] - (K-1)));
			}
		}
		for (int i = N-1; i >= 0; i--) {
			if (S[i] == '>') {
				B[i] = max(B[i], B[i+1] + 1);
			} else {
				B[i] = max(B[i], B[i+1] - (A[i+1] - A[i] - (K-1)));
			}
		}

		for (int i = 0; i <= N; i++) {
			cout << B[i] << " \n"[i==N];
			A[i] -= B[i];
		}
		K--;
	}

	for (int i = 0; i <= N; i++) {
		cout << A[i] << " \n"[i==N];
	}

	return 0;
}
