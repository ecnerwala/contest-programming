#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(2*N);
	for (auto& p : P) cin >> p;
	std::vector<int> res; res.reserve(N);
	for (int i = 0; i < 2*N; i += 2) {
		if (i + 2 == 2*N) {
			if (P[i] > P[i+1]) {
				res.push_back(i);
				std::swap(P[i], P[i+1]);
			}
		} else {
			if (P[i] > P[i+1] && P[i] > P[i+2]) {
				res.push_back(i);
				std::swap(P[i], P[i+1]);
			} else if (P[i+2] > P[i] && P[i+2] > P[i+1]) {
				res.push_back(i+1);
				std::swap(P[i+1], P[i+2]);
			}
		}
	}
	cout << res.size() << '\n';
	for (int z = 0; z < int(res.size()); z++) {
		cout << res[z]+1 << " \n"[z+1==int(res.size())];
	}

	return 0;
}
