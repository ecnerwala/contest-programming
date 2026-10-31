#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, N; cin >> B >> N;
	vector<int64_t> V(N);
	int64_t all_has = 0;
	for (auto& v : V) {
		int32_t a; cin >> a;
		v = (int64_t(((1 << B) - 1) ^ a) << B) | a;
		all_has |= v;
	}

	array<vector<int>, 2> dist{vector<int>(N), vector<int>(N)};
	for (int z = 0; z < 2; z++) {
		if (z) reverse(V.begin(), V.end());
		for (int b = 0; b < 2*B; b++) {
			if (!((all_has >> b) & 1)) continue;
			int cur_dist = N+1;
			for (int i = 0; i < N; i++) {
				cur_dist++;
				if ((V[i] >> b) & 1) cur_dist = 0;
			}
			assert(cur_dist < N);
			for (int i = 0; i < N; i++) {
				setmax(dist[z][i], cur_dist);
				cur_dist++;
				if ((V[i] >> b) & 1) cur_dist = 0;
			}
		}
		if (z) {
			reverse(V.begin(), V.end());
			reverse(dist[z].begin()+1, dist[z].end());
		}
	}

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += max(0, N - 1 - dist[0][i] - dist[1][i]);
	}
	ans /= 2;
	cout << ans << '\n';

	return 0;
}
