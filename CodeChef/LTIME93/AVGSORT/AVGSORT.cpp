#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N); for (auto& a : A) cin >> a;
		bool is_good = false;
		for (int i = 0; i+1 < N; i++) {
			if (A[i] < A[i+1]) is_good = true;
		}
		cout << (is_good ? "Yes" : "No") << '\n';
	}

	return 0;
}
