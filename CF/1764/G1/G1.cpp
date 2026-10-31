#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [](int N, auto query) -> int {
		int lo = 0;
		int hi = N;
		while (hi - lo > 1) {
			int md = (lo + hi) / 2;
			int x = query(0, md, 2);
			int y = query(md, N, 2);
			if ((x - y)*2 > md - (N - md)) {
				hi = md;
			} else if ((x - y)*2 < md - (N - md)) {
				lo = md;
			} else {
				assert((x - y)*2 == md - (N - md));
				assert(N % 2 == 0);
				bool is_right_of_N;
				assert(N >= 3);
				if (md > (N - md)) {
					is_right_of_N = query(0, md, N) == 2;
				} else {
					is_right_of_N = query(md, N, N) == 1;
				}
				if (is_right_of_N) {
					lo = md;
				} else {
					hi = md;
				}
			}
		}
		return lo;
	};

	if (false) {
		for (int N = 3; N <= 6; N++) {
			std::vector<int> P(N);
			std::iota(P.begin(), P.end(), 1);
			do {
				cerr << "Solving N = " << N << '\n';
				for (int v : P) { cerr << v << ' '; } cerr << '\n';
				int res = solve(N, [&](int l, int r, int k) -> int {
					cerr << "? " << l+1 << ' ' << r << ' ' << k << '\n';
					std::set<int> s;
					for (int i = l; i < r; i++) s.insert(P[i] / k);
					cerr << int(s.size()) << '\n';
					return int(s.size());
				});
				cerr << "! " << res+1 << '\n';
				if (P[res] != 1) {
					cerr << "Bad " << '\n';
					assert(false);
				}
			} while (std::next_permutation(P.begin(), P.end()));
		}
	}

	int N; cin >> N;
	auto query = [&](int l, int r, int k) -> int {
		cout << '?' << ' ' << l+1 << ' ' << r << ' ' << k << '\n' << flush;
		int res; cin >> res;
		return res;
	};

	int res = solve(N, query);
	cout << '!' << ' ' << res+1 << '\n' << flush;
	return 0;
}
