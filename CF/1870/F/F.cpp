#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, K; cin >> N >> K;
		const int64_t MAX_FIT = N / K;

		std::vector<int64_t> cutoffs;
		cutoffs.push_back(1);
		while (cutoffs.back() <= MAX_FIT) {
			cutoffs.push_back(cutoffs.back() * K);
		}
		cutoffs.push_back(N+1);

		int64_t tot = 0;
		for (int l = 1; l < int(cutoffs.size()); l++) {
			// binary search on lo..hi
			auto sort_idx = [&](int64_t v) {
				assert(cutoffs[l-1] <= v && v < cutoffs[l]);
				int64_t ans = 0;
				{
					int64_t cur = v;
					for (int i = l; i > 0; i--, cur /= K) {
						ans += cur - cutoffs[i-1] + 1;
					}
				}
				{
					int64_t cur = v;
					for (int i = l+1; i < int(cutoffs.size()); i++) {
						cur = (cur > MAX_FIT ? N+1 : cur * K);
						ans += cur - cutoffs[i-1];
					}
				}
				return ans;
			};

			int64_t cut_ge;
			int64_t cut_gt;
			{
				int64_t mi = cutoffs[l-1] - 1, ma = cutoffs[l];
				while (ma - mi > 1) {
					int64_t md = (mi + ma) / 2;
					if (sort_idx(md) >= md) {
						ma = md;
					} else {
						mi = md;
					}
				}
				cut_ge = mi;
			}
			{
				int64_t mi = cutoffs[l-1] - 1, ma = cutoffs[l];
				while (ma - mi > 1) {
					int64_t md = (mi + ma) / 2;
					if (sort_idx(md) > md) {
						ma = md;
					} else {
						mi = md;
					}
				}
				cut_gt = mi;
			}
			tot += cut_gt - cut_ge;
		}
		cout << tot << '\n';
	}

	return 0;
}
