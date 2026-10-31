#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> vals;
		for (int p = 2; N > 1; p++) {
			if (p * p > N) p = N;
			int e = 0;
			while (N % p == 0) {
				N /= p;
				if (e == int(vals.size())) vals.push_back(p);
				else vals[e] *= p;
				e++;
			}
		}
		cout << std::accumulate(vals.begin(), vals.end(), int64_t(0)) << '\n';
	}

	return 0;
}
