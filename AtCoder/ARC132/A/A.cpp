#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> R(N);
	for (auto& r : R) cin >> r;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;
	int Q; cin >> Q;
	while (Q--) {
		int r, c; cin >> r >> c; r--, c--;
		cout << ".#"[R[r] + C[c] >= N+1];
	}
	cout << '\n';

	return 0;
}
