#include <bits/stdc++.h>
int main() {
	using namespace std;
	int K = 1;
	int N = 10;
	int Q = 100;

	cout << K << ' ' << N << ' ' << Q << '\n';
	mt19937_64 mt(48);
	std::uniform_int_distribution<int> vdist(0, 10);
	std::uniform_int_distribution<int> ddist(0, 10);

	for (int k = 0; k < K; k++) {
		for (int i = 0; i < N; i++) {
			cout << vdist(mt) << " \n"[i+1==N];
		}
	}

	std::vector<bool> alive(Q+1);
	alive[0] = true;
	for (int q = 1; q <= Q; q++) {
		int op = std::uniform_int_distribution<int>(2, 3)(mt);
		int t;
		do {
			t = std::uniform_int_distribution<int>(0, q-1)(mt);
		} while (!alive[t]);
		if (op == 1 || op == 2) {
			int p = std::uniform_int_distribution<int>(1, K)(mt);
			int l = std::uniform_int_distribution<int>(1, N)(mt);
			int r = std::uniform_int_distribution<int>(1, N)(mt);
			if (l > r) std::swap(l, r);
			int x = (op == 1 ? ddist : vdist)(mt);
			cout << op << ' ' << t << ' ' << p << ' ' << l << ' ' << r << ' ' << x << '\n';
			alive[q] = true;
		} else if (op == 3) {
			int l = std::uniform_int_distribution<int>(1, N)(mt);
			int r = std::uniform_int_distribution<int>(1, N)(mt);
			if (l > r) std::swap(l, r);
			cout << op << ' ' << t << ' ' << l << ' ' << r << '\n';
		} else assert(false);
	}

}
