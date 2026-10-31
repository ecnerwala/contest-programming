#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) cin >> a;

	std::vector<char> D(N-1);
	for (char& c : D) cin >> c;

	int must_good = -1;
	int K = 0;
	std::vector<int> best; best.reserve(N);
	for (int a : A) {
		int mi = must_good, ma = K;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if ((D[K-1] == 'U') == (a > best[md])) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == K) {
			K++;
			best.push_back(a);
			if (K == 1 || (K-1 < N-1 && D[K-1] != D[K-2])) {
				must_good = K-2;
			}
		} else {
			best[ma] = a;
		}
	}

	cout << K-1 << '\n';

	return 0;
}
