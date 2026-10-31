#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int A1, K; cin >> A1 >> K;
	struct seg { int c, x, y, z, a; };
	vector<seg> V(K);
	int A = A1;
	for (int i = 0; i < K; i++) {
		int c, x, y, z; cin >> c >> x >> y >> z;
		V[i] = {c, 1, (y?z-y:0), z, 0};

		int e = z-2;
		while (c || e) {
			if (c & 1) A = int((int64_t(A) * int64_t(x) + y) % z);
			if (e & 1) V[i].x = int(int64_t(V[i].x) * int64_t(x) % z);
			y = int(int64_t(x+1) * int64_t(y) % z);
			x = int(int64_t(x) * int64_t(x) % z);
			c >>= 1, e >>= 1;
		}
		V[i].a = A;
	}

	int ans = 0;
	auto process = [&](int a) {
		if (!M) N--;
		else if (a < M) N--, M--;
		else if (N == M) N--, M--, ans++;
		else N--;
	};

	for (int i = K-1; i >= 0; i--) {
		auto [c, x, y, z, a] = V[i];
		while (c--) {
			process(a);
			a = int(int64_t(a + y) * int64_t(x) % z);
		}
	}
	process(A1);

	assert(!N && !M);
	cout << ans << '\n';

	return 0;
}
