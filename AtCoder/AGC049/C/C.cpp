#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct ball_type {
		int64_t v, c;
	};
	vector<ball_type> A(N);
	for (auto& a : A) cin >> a.v;
	for (auto& a : A) cin >> a.c;

	const int64_t INF = 1e18;
	int64_t cur_cover = INF;
	int64_t num_uncovered = 0;
	int64_t ans = INF;
	for (int i = N-1; i >= 0; i--) {
		if (A[i].c >= A[i].v) {
			ans = min(ans, max(num_uncovered, A[i].c - A[i].v + 1));
			if (A[i].v < cur_cover) {
				num_uncovered++;
			}
		} else {
			// this guys does covering
			cur_cover = min(cur_cover, A[i].v - A[i].c);
			assert(cur_cover > 0);
		}
	}
	ans = min(ans, num_uncovered);
	cout << ans << '\n';

	return 0;
}
