#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;
	std::vector<int> A = {1,1,1,1,2,2,2,1,1,2,2};

	int T;
	if (TESTING) {
		T = 1;
	} else {
		cin >> T;
	}
	while (T--) {
		int N;
		if (TESTING) {
			N = int(A.size());
		} else {
			cin >> N;
		}
		auto query = [&](int l, int r) -> int64_t {
			assert(0 <= l && l < r && r <= N);
			cout << '?' << ' ' << l+1 << ' ' << r << '\n' << flush;
			if (TESTING) {
				return std::accumulate(A.begin() + l, A.begin() + r, int64_t(0));
			}
			int64_t v; cin >> v;
			return v;
		};
		int l = 0, r = N;
		int64_t tot = query(l, r);
		while (r-l > 1) {
			int mi = l, ma = r-1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				int64_t v = query(l, md);
				if (2 * v >= tot) ma = md;
				if (2 * v <= tot) mi = md-1;
			}
			tot /= 2;
			if (r - ma <= ma - l) l = ma;
			else r = ma;
		}
		cout << '!' << ' ' << tot << '\n' << flush;
		if (TESTING) {
			assert(tot == *std::max_element(A.begin(), A.end()));
		}
	}

	return 0;
}
