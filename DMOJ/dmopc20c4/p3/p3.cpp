#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<pair<int, int>> R(N);
	for (auto& r : R) cin >> r.first >> r.second;
	vector<pair<int, int>> S(M);
	for (auto& s : S) cin >> s.first >> s.second;

	vector<int> Ys; Ys.reserve(N);
	for (auto r : R) Ys.push_back(r.second);
	sort(Ys.begin(), Ys.end());

	vector<int> bit(N);

	int64_t ans = 0;

	sort(R.begin(), R.end());
	sort(S.begin(), S.end());
	auto it = S.begin();
	for (auto& [rx, ry] : R) {
		while (it != S.end() && it->first <= rx) {
			// insert this guy
			int y = int(upper_bound(Ys.begin(), Ys.end(), it->second) - Ys.begin());
			for (int a = y; a; a &= a-1) {
				bit[a-1]++;
			}
			++it;
		}
		ry = int(lower_bound(Ys.begin(), Ys.end(), ry) - Ys.begin());
		for (int a = ry; a < N; a |= a+1) {
			ans += bit[a];
		}
	}

	cout << ans << '\n';

	return 0;
}
