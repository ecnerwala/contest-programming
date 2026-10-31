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
	cerr << inds.size() << '\n';
	std::sort(inds.begin(), inds.end());

	std::vector<int> F(inds.size()), G(inds.size());

	int64_t F0, G0, T, M; cin >> F0 >> G0 >> T >> M;
	std::vector<int64_t> queries(T);
	for (auto& q : queries) cin >> q;


	return 0;
}
