#include<bits/stdc++.h>
using namespace std;

// returns mustSurrender
bool go() {
	int N, M, K; cin >> N >> M >> K;
	assert(N >= 3 && M >= 3);
	int A, B; cin >> A >> B;
	vector<pair<int, int>> P(K);
	for (int i = 0; i < K; i++) cin >> P[i].first >> P[i].second;

	if (K == 1) {
		return false;
	} else if (K == 2) {
		if ((P[0].first + P[0].second) % 2 == (A + B) % 2 && (P[1].first + P[1].second) % 2 == (A + B) % 2) {
			// both are on the right parity
			// they can corner him
			return true;
		} else {
			return false;
		}
	} else assert(false);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		bool ans = go();
		cout << "Case #" << t << ": " << (ans ? "Y" : "N") << '\n';
	}

	return 0;
}
