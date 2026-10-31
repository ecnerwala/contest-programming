#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int i = 1; i <= 100; i++) {
		if (i / 2 + i / 3 + i / 5 + i / 7 < i) cerr << i << '\n';
	}
	const int64_t MAXV = int64_t(1.1e15);
	std::vector<int64_t> inds; inds.reserve(10);
	for (int64_t a = 1; a <= MAXV; a *= 2) {
		for (int64_t b = 1; a * b <= MAXV; b *= 3) {
			for (int64_t c = 1; a * b * c <= MAXV; c *= 5) {
				for (int64_t d = 1; a * b * c * d <= MAXV; d *= 7) {
					int64_t v = a * b * c * d;
					cerr << v << '\n';
					inds.push_back(a * b * c * d);
				}
			}
		}
	}
	inds.push_back(0);
	std::sort(inds.begin(), inds.end());

	int64_t F0, G0, T, M; cin >> F0 >> G0 >> T >> M;
	std::vector<int64_t> queries(T);
	for (auto& q : queries) cin >> q;

	std::vector<int64_t> F(inds.size()), G(inds.size());
	std::vector<int64_t> F_sat(inds.size()), G_sat(inds.size());
	F[0] = F0 % M;
	F_sat[0] = F0;
	G[0] = G0 % M;
	G_sat[0] = G0;
	for (int i = 1, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j7 = 0; i < int(inds.size()); i++) {
		while (inds[j2+1] <= inds[i] / 2) j2++;
		while (inds[j3+1] <= inds[i] / 3) j3++;
		while (inds[j4+1] <= inds[i] / 4) j4++;
		while (inds[j5+1] <= inds[i] / 5) j5++;
		while (inds[j7+1] <= inds[i] / 7) j7++;
		assert(j2 < i);
		assert(j3 < i);
		assert(j4 < i);
		assert(j5 < i);
		assert(j7 < i);
		F[i] = (G[j2] + G[j3] + G[j5] + G[j7]) % M;
		G[i] = (F[j2] + F[j3] + F[j4] + F[j5]) % M;
		F_sat[i] = G_sat[j2] + G_sat[j3] + G_sat[j5] + G_sat[j7];
		G_sat[i] = F_sat[j2] + F_sat[j3] + F_sat[j4] + F_sat[j5];
		if (F_sat[i] > MAXV) F_sat[i] = MAXV;
		if (F_sat[i] < inds[i]) {
			F[i] = inds[i] % M;
			F_sat[i] = inds[i];
		}
		if (G_sat[i] > MAXV) G_sat[i] = MAXV;
		if (G_sat[i] < inds[i]) {
			G[i] = inds[i] % M;
			G_sat[i] = inds[i];
		}
	}

	for (auto q : queries) {
		int a = int(std::upper_bound(inds.begin(), inds.end(), q) - inds.begin()) - 1;
		cout << F[a] << ' ' << G[a] << '\n';
	}

	return 0;
}
