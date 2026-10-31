#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t K; cin >> N >> K;
	vector<pair<int64_t, array<int64_t, 2>>> pts(N);
	for (auto& p : pts) {
		cin >> p.first >> p.second[0] >> p.second[1];
		p.first *= K;
	}

	int64_t ans = 0;
	array<int64_t, 2> incr{pts[0].second[0] - pts[0].first, pts[0].second[1] - pts[0].first};
	array<int64_t, 2> decr{pts[0].second[0] + pts[0].first, pts[0].second[1] + pts[0].first};
	for (int i = 1; i < N; i++) {
		for (int z = 0; z < 2; z++) {
			setmin(incr[z], pts[i].second[z] - pts[i].first);
			setmax(ans, (pts[i].second[z] - pts[i].first) - incr[z]);

			setmax(decr[z], pts[i].second[z] + pts[i].first);
			setmax(ans, decr[z] - (pts[i].second[z] + pts[i].first));
		}
	}
	cout << ans << '\n';

	return 0;
}
