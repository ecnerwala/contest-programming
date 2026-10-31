#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, M; cin >> N >> M;
	int K; cin >> K;
	unordered_map<int64_t, bool> A;
	unordered_map<int64_t, bool> B;
	for (int k = 0; k < K; k++) {
		int64_t a, b; cin >> a >> b;
		A[a] ^= 1;
		B[b] ^= 1;
	}
	vector<int64_t> X; X.reserve(A.size());
	for (auto [i, v] : A) if (v) X.push_back(i);
	vector<int64_t> Y; Y.reserve(B.size());
	for (auto [i, v] : B) if (v) Y.push_back(i);
	int S = max(int(X.size()), int(Y.size()));
	X.resize(S, 1);
	Y.resize(S, 1);
	cout << S << '\n';
	for (int i = 0; i < S; i++) {
		cout << X[i] << ' ' << Y[i] << '\n';
	}

	return 0;
}
