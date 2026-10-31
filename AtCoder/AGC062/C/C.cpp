#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end());
	A.push_back(int64_t(1e18));

	std::vector<std::array<int64_t, 2>> ranges;
	ranges.push_back({0, 1});

	std::vector<int64_t> res; res.reserve(K);
	for (int64_t a : A) {
		assert(ranges[0][0] == 0);

		res.clear();
		for (int i = 0; i < int(ranges.size()); i++) {
			int64_t l = ranges[i][1];
			int64_t r = (i+1 == int(ranges.size())) ? int64_t(1e18) : ranges[i+1][0];
			if (l >= a) break;
			r = std::min(r, a);
			for (int64_t z = l; z < r; z++) {
				res.push_back(z);
				if (int(res.size()) == K) {
					goto done;
				}
			}
		}

		{
			std::vector<std::array<int64_t, 2>> nranges;
			nranges.reserve(ranges.size()*2);
			for (int i = 0, j = 0; i < int(ranges.size()) || j < int(ranges.size()); ) {
				std::array<int64_t, 2> cur_rng;
				if (i < int(ranges.size()) && (j == int(ranges.size()) || ranges[i][0] < ranges[j][0] + a)) {
					cur_rng = ranges[i];
					i++;
				} else {
					cur_rng = ranges[j];
					cur_rng[0] += a;
					cur_rng[1] += a;
					j++;
				}
				auto [l, r] = cur_rng;
				if (!nranges.empty() && l <= nranges.back()[1]) {
					nranges.back()[1] = std::max(nranges.back()[1], r);
				} else {
					nranges.push_back({l, r});
				}
			}
			ranges = nranges;
		}
	}

done:
	 assert(int(res.size()) == K);
	 for (int k = 0; k < K; k++) {
		 cout << res[k] << " \n"[k+1==K];
	 }

	return 0;
}
