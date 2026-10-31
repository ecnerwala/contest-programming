#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::vector<int> A(N); for (auto& a : A) cin >> a;

	int64_t ans = 0;

	auto c2 = [&](int a) -> int64_t {
		assert(a >= 0);
		return int64_t(a) * int64_t(a+1) / 2;
	};
	std::vector<std::array<int, 2>> ranges; ranges.reserve(N);
	auto get_num_segs = [&](int lo, int hi) -> int64_t {
		int64_t res = 0;
		for (int i = lo; i < hi; i++) {
			res += c2(ranges[i][1] - ranges[i][0]);
		}
		for (int i = lo; i+1 < hi; i++) {
			res -= c2(ranges[i][1] - ranges[i+1][0]);
		}
		return res;
	};
	struct val_t {
		int st;
		int64_t num_segs;
		int v;
	};
	std::vector<val_t> vals; vals.reserve(N);
	for (int z = 0; z <= N; z++) {
		while (!vals.empty() && (z == N || vals.back().v < A[z])) {
			int en = int(ranges.size());
			if (en - vals.back().st == 1) {
				if (vals.size() >= 2 && (z == N || vals.end()[-2].v <= A[z])) {
					int64_t num_segs = vals.back().num_segs;
					vals.pop_back();
					vals.back().num_segs += num_segs;
					continue;
				} else {
					if (z < N) {
						vals.back().v = A[z];
					}
					break;
				}
			}

			vals.back().v++;
			int n_en = vals.back().st;
			for (int i = vals.back().st, j = i+1; j < en; ) {
				while (ranges[i][1] < ranges[j][0]) i++;
				assert(i < j);
				while (j+1 < en && ranges[j+1][0] <= ranges[i][1]) j++;
				ranges[n_en++] = {ranges[i][0], ranges[j][1]};
				j++;
				i++;
			}
			en = n_en;
			ranges.resize(en);
			int64_t n_num_segs = get_num_segs(vals.back().st, en);
			ans += (n_num_segs - vals.back().num_segs) * vals.back().v;
			vals.back().num_segs = n_num_segs;

			if (vals.size() >= 2 && vals.end()[-2].v == vals.back().v) {
				int64_t num_segs = vals.back().num_segs;
				vals.pop_back();
				vals.back().num_segs += num_segs;
			}
		}
		if (z < N) {
			ans += A[z];
			ranges.push_back({z, z+1});
			if (!vals.empty() && vals.back().v == A[z]) {
				vals.back().num_segs++;
			} else {
				assert(vals.empty() || vals.back().v > A[z]);
				vals.push_back({int(ranges.size())-1, 1, A[z]});
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
