#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(M); for (auto& b : B) cin >> b;
	vector<int> res; res.reserve(N+M);
	for (int i = 0, j = 0; i < N || j < M; ) {
		if (i == N) {
			res.push_back(B[j++]);
		} else if (j == M) {
			res.push_back(A[i++]);
		} else if (A[i] < B[j]) {
			res.push_back(A[i++]);
		} else if (A[i] > B[j]) {
			res.push_back(B[j++]);
		} else {
			i++, j++;
		}
	}
	for (int i : res) { cout << i << ' '; } cout << '\n';

	return 0;
}
