#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> seq;
	int64_t sum = 0;
	int len = 0;
	for (int i = 1; true; i++) {
		if (i * (i+1) / 2 > sum) {
			len += 2 * i + 1;
			sum += i * (i+1) / 2;
			if (len > 1000) break;
			seq.push_back(i);
			cerr << int(seq.size()) << ' ' << i << ' ' << len << ' ' << sum << '\n';
		}
	}

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
		const int L = int(seq.size());
		for (int i = 0; i < N; i += L) {
			std::vector<int> inds(L);
			for (int z = 0; z < L; z++) {
				inds[z] = std::min(i + z, N-1);
			}
			std::vector<int> cnd; cnd.reserve(1000);
			for (int z = 0; z < L; z++) {
				for (int a = 0; a < seq[z]; a++) {
					cnd.push_back(open_ind);
				}
				for (int a = 0; a < seq[z]; a++) {
					cnd.push_back(inds[z]);
				}
				cnd.push_back(open_ind);
			}
			int resp = query(cnd);
			for (int z = L-1; z >= 0; z--) {
				int cnt = seq[z] * (seq[z]+1) / 2;
				if (resp >= cnt) {
					ans[inds[z]] = ')';
					resp -= cnt;
				} else {
					ans[inds[z]] = '(';
				}
			}
			assert(resp == 0);
		}

		cout << '!' << ' ' << ans << '\n' << flush;
	}

	return 0;
}
