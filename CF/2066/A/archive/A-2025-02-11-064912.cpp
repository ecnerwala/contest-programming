#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		int N; cin >> N;
		std::vector<int> X(N);
		for (auto& x : X) { cin >> x; x--; }

		std::vector<int> invX(N, -1);
		for (int i = 0; i < N; i++) invX[X[i]] = i;
		// if some value v doesn't exist, query v k where x[v] != x[k]; there's no path, so A gives 0 and B gives nonzero
		int v = 0;
		while (v < N && invX[v] != -1) v++;
		if (v < N) {
			int k = 0;
			while (X[k] == X[v]) k++;
			assert(k < N);
			cout << '?' << ' ' << v+1 << ' ' << k+1 << '\n' << flush;
			int res; cin >> res;
			cout << '!' << ' ' << (res == 0 ? 'A' : 'B') << '\n' << flush;
		} else {
			// we're a permutation
			int i = invX[0], j = invX[N-1];
			cout << '?' << ' ' << i+1 << ' ' << j+1 << '\n' << flush;
			int res; cin >> res;
			if (res == N-1) {
				assert(N >= 3);
				cout << '?' << ' ' << j+1 << ' ' << i+1 << '\n' << flush;
				cin >> res;
			}

			if (res >= N-1) {
				cout << '!' << ' ' << 'B' << '\n' << flush;
			} else {
				cout << '!' << ' ' << 'A' << '\n' << flush;
			}
		}
	}

	return 0;
}
