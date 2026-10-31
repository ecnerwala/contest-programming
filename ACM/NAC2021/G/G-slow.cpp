#include <bits/stdc++.h>
int main() {
	using namespace std;
	int N; cin >> N;
	int S; cin >> S;
	int Q; cin >> Q;
	vector<int> P(N); for (auto& p : P) { cin >> p; p--; }

	vector<int> A; A.push_back(N-1);
	for (int s = 0; s < S; s++) {
		vector<int> B; B.reserve(A.size()*N);
		for (int a : A) {
			for (int p : P) if (p <= a) B.push_back(p);
		}
		A = std::move(B);
		for (int a : A) { cerr << a << ' '; } cerr << '\n';
	}

	cerr << A.size() << '\n';
	for (int q = 0; q < Q; q++) {
		int k, a; cin >> k >> a; k--;
		assert(0 <= a && a <= int(A.size()));
		int ans = 0;
		for (int i = 0; i < a; i++) ans += A[i] == k;
		cout << ans << '\n';
	}
}
