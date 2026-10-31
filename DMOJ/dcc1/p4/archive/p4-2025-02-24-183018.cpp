#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	auto is_good = [&](int64_t G) -> bool {
		// in the dp, all gaps are >= G, so adding a -1 shifts everything up by G and increases the count by 1
		std::vector<int64_t> dp; dp.reserve(N);
		int64_t cur_extra = 0;
		int cnt_extra = 0;
		for (auto a : A) {
			if (a == -1) {
				cnt_extra++;
				cur_extra += G;
			} else {
				a -= cur_extra;
				int mi = -1;
				int ma = int(dp.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (a >= dp[md]) mi = md;
					else ma = md;
				}
				if (mi == -1 || a >= dp[mi] + G) {
					if (ma == int(dp.size())) {
						dp.push_back(a);
					} else {
						dp[ma] = a;
					}
				}
			}
		}
		return int(dp.size()) + cnt_extra >= K;
	};

	int INF_G = int(1e9) + 1;
	int mi = -1;
	int ma = INF_G+1;
	while (ma - mi > 1) {
		int md = mi + (ma - mi) / 2;
		if (is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	if (mi >= INF_G) {
		cout << "Infinity" << '\n';
	} else {
		cout << mi << '\n';
	}

	return 0;
}
