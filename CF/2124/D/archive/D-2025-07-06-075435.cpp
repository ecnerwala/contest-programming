#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		K--;
		if (K == 0) {
			cout << "YES" << '\n';
			continue;
		}
		std::vector<int> V = A;
		std::nth_element(V.begin(), V.begin() + (K-1), V.end());
		int thresh = *(V.begin() + (K-1));
		std::vector<int> freq; freq.reserve(N+1); freq.push_back(0);
		std::vector<int> vals; vals.reserve(N);
		for (int v : A) {
			if (v < thresh) {
				vals.push_back(v);
				freq.push_back(0);
			} else if (v == thresh) {
				freq.back()++;
			}
		}
		bool good = true;
		for (int i = 0; i < int(vals.size()); i++) {
			if (vals[i] != vals[int(vals.size()) - 1 - i]) {
				good = false;
			}
		}
		int cnt = int(vals.size());
		for (int i = 0, j = int(freq.size()) - 1; i <= j; i++, j--) {
			if (i == j) {
				cnt += freq[i];
			} else {
				cnt += 2 * std::min(freq[i], freq[j]);
			}
		}
		if (cnt < K) good = false;

		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
