#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, R; cin >> N >> K >> R;
	vector<int> D(N); for (auto& d : D) cin >> d;

	vector<int> req(K, 0);
	for (int r = 0; r < R; r++) {
		int b; cin >> b; cin >> req[b];
	}

	int num_bad = R;
	assert(num_bad > 0);
	int ans = N+1;
	for (int l = 0, r = 0; true; ) {
		if (!num_bad) {
			ans = min(ans, r-l);
			// increment l
			assert(l < r);

			num_bad -= (req[D[l]] > 0);
			req[D[l]]++;
			num_bad += (req[D[l]] > 0);

			l++;
		} else {
			if (r == N) break;

			num_bad -= (req[D[r]] > 0);
			req[D[r]]--;
			num_bad += (req[D[r]] > 0);

			r++;
		}
	}

	if (ans == N+1) {
		cout << "impossible" << '\n';
	} else {
		cout << ans << '\n';
	}

	return 0;
}
