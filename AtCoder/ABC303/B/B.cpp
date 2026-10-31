#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::set<std::array<int, 2>> res;
	for (int j = 0; j < M; j++) {
		int a; cin >> a;
		for (int i = 1; i < N; i++) {
			int b; cin >> b;
			res.insert({std::min(a, b), std::max(a, b)});
			a = b;
		}
	}
	cout << N * (N-1) / 2 - res.size() << '\n';

	return 0;
}
