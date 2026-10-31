#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::mt19937_64 mt(42);
	std::map<int, std::mt19937_64::result_type> vals;

	std::array<std::vector<std::mt19937_64::result_type>, 2> A;
	for (auto& a : A) {
		a.resize(N+1);
		std::set<int> seen;
		for (int i = 0; i < N; i++) {
			int x; cin >> x;
			a[i+1] = a[i];
			if (seen.insert(x).second) {
				auto& v = vals[x];
				while (!v) v = mt();
				a[i+1] ^= v;
			}
		}
	}

	int Q; cin >> Q;
	while (Q--) {
		int x, y; cin >> x >> y;
		cout << (A[0][x] == A[1][y] ? "Yes" : "No") << '\n';
	}

	return 0;
}
