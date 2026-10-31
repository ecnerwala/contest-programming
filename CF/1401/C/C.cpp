#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N);
		for (auto& a : A) cin >> a;
		vector<int> B = A;
		sort(B.begin(), B.end());
		bool good = true;
		for (int i = 0; i < N; i++) {
			if (B[i] != A[i] && B[i] % B[0]) {
				good = false;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
