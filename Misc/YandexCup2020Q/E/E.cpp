#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> W(N); for (auto& w : W) cin >> w;

	vector<int> coords = W;
	sort(coords.begin(), coords.end());
	coords.resize(unique(coords.begin(), coords.end()) - coords.begin());

	int S = int(coords.size());
	vector<pair<int64_t, int>> bit(S);

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int k = int(lower_bound(coords.begin(), coords.end(), W[i]) - coords.begin());
		for (int a = k; a < S; a |= a+1) {
			ans += bit[a].first;
		}
		cout << ans << " \n"[i+1==N];
		for (int a = k; a; a &= a-1) {
			bit[a-1].first += W[i];
		}
	}

	return 0;
}
