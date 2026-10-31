#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// We first try to find any single pair, which costs 10 queries.
	// Then, we can do batch tests.
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		auto query = [&](std::vector<int> inds) -> int {
			assert(int(inds.size()) <= 1000);
			cout << '?' << ' ' << int(inds.size());
			for (int i : inds) {
				cout << ' ' << i+1;
			}
			cout << '\n' << flush;
			int resp; cin >> resp;
			return resp;
		};

		std::string ans(N, '?');
		// 0 1 2 ... N-1 0 must contain a () pair, so let's just find one
		int open_ind, closed_ind;
		{
			int mi = 0;
			int ma = N;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				// check mi..md
				std::vector<int> cnd(md - mi + 1);
				std::iota(cnd.begin(), cnd.end(), mi);
				assert(md < N);
				if (query(cnd) > 0) {
					ma = md;
				} else {
					mi = md;
				}
			}
			open_ind = mi;
			closed_ind = (mi+1)%N;
		}
		// now, we can just test each thing against it
		auto check_2 = [&](int i1, int i2) {
			int resp = query({i1, closed_ind, open_ind, i2, open_ind, i2});
			if (resp == 0) {
				ans[i1] = ')';
				ans[i2] = '(';
			} else if (resp == 1) {
				ans[i1] = '(';
				ans[i2] = '(';
			} else if (resp == 3) {
				ans[i1] = ')';
				ans[i2] = ')';
			} else if (resp == 6) {
				ans[i1] = '(';
				ans[i2] = ')';
			} else assert(false);
		};
		for (int i = 0; i < N; i += 2) {
			if (i+1 < N) {
				check_2(i, i+1);
			} else {
				check_2(i, i);
			}
		}

		cout << '!' << ' ' << ans << '\n' << flush;
	}

	return 0;
}
