#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, Q; cin >> N >> K >> Q;
		std::vector<std::array<int, 2>> locs(N, {N, -1});
		for (int k = 0; k < K; k++) {
			for (int i = 0; i < N; i++) {
				int a; cin >> a; a--;
				locs[a][0] = std::min(locs[a][0], i);
				locs[a][1] = std::max(locs[a][1], i);
			}
		}
		std::vector<int> pref(N);
		for (int a = 0; a < N; a++) {
			cerr << a << ' ' << locs[a][0] << ' ' << locs[a][1] << '\n';
			pref[locs[a][0]]++;
			pref[locs[a][1]]--;
		}
		for (int i = 1; i < N; i++) {
			pref[i] += pref[i-1];
		}
		std::vector<int64_t> pref_pairs(N+1);
		std::vector<int> pref_cnt(N);
		std::vector<int> val(N);
		std::vector<int> tot_cnt(N);

		int cnt = 0;
		int v = -1;
		for (int i = 0; i < N; i++) {
			if (i == 0 || pref[i-1] == 0) {
				// then i is different from the previous
				cnt = 0;
				v++;
			}
			pref_cnt[i] = cnt;
			val[i] = v;
			pref_pairs[i+1] = pref_pairs[i] + cnt;
			cerr << i << ' ' << cnt << '\n';
			cnt++;
			tot_cnt[v]++;
		}

		auto query = [&](int l, int r) -> int64_t {
			// subtract off l <-> r pairs
			assert(l < r);
			int a = pref_cnt[l]; int b = tot_cnt[val[l]] - a;
			int64_t ans = pref_pairs[r] - pref_pairs[l] - int64_t(a) * int64_t(b);
			return ans;
		};

		int64_t prv_ans = 0;
		for (int q = 0; q < Q; q++) {
			int id, l, r; cin >> id >> l >> r;
			id = int((id + prv_ans) % K + 1);
			l = int((l + prv_ans) % N + 1);
			r = int((r + prv_ans) % N + 1);
			id--; l--; r--;
			r++;

			prv_ans = query(l, r);

			cout << prv_ans << '\n';
		}
	}

	return 0;
}
