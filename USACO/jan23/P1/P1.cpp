#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	string dir_string; cin >> dir_string;
	string is_special; cin >> is_special;
	std::vector<int> pref_special(N+1);
	for (int i = 0; i < N; i++) {
		pref_special[i+1] = pref_special[i] + (is_special[i] == '1');
	}


	struct jump_t {
		int dest;
		int len;
		int num_special;
		int64_t special_dist;
	};
	auto merge = [&](jump_t a, jump_t b) -> jump_t {
		return {b.dest, a.len + b.len, a.num_special + b.num_special, a.special_dist + int64_t(b.num_special) * a.len + b.special_dist};
	};
	std::vector<jump_t> prv(N);
	std::vector<jump_t> nxt(N);

	{
		int cntL = 0, cntR = 0;
		for (int i = 0; i < 2*N; i++) {
			if (dir_string[i] == 'L') {
				int loc;
				if (cntR == cntL) {
					assert(cntL == 0);
					loc = -1;
				} else {
					loc = cntR;
				}
				int num_special = pref_special[cntL] - pref_special[std::max(loc, 0)];
				prv[cntL] = {loc, 1, num_special, num_special};
				cntL++;
			} else if (dir_string[i] == 'R') {
				int loc;
				if (cntR == cntL-1) {
					assert(cntR == N-1);
					loc = N;
				} else {
					loc = cntL - 1;
				}
				int num_special = pref_special[std::min(N, loc+1)] - pref_special[cntR+1];
				nxt[cntR] = {loc, 1, num_special, num_special};
				cntR++;
			} else assert(false);
		}
	}

	std::vector<jump_t> prv_long(N);
	std::vector<jump_t> nxt_long(N);
	for (int i = 0; i < N; i++) {
		prv_long[i] = prv[i];
		int p = prv[i].dest;
		if (p == -1) continue;
		int gp = prv_long[p].dest;
		if (gp == -1) continue;
		if (prv_long[gp].len == prv_long[p].len) {
			prv_long[i] = merge(prv[i], merge(prv_long[p], prv_long[gp]));
		}
	}
	for (int i = N-1; i >= 0; i--) {
		nxt_long[i] = nxt[i];
		int p = nxt[i].dest;
		if (p == N) continue;
		int gp = nxt_long[p].dest;
		if (gp == N) continue;
		if (nxt_long[gp].len == nxt_long[p].len) {
			nxt_long[i] = merge(nxt[i], merge(nxt_long[p], nxt_long[gp]));
		}
	}

	for (int q = 0; q < Q; q++) {
		int a, b; cin >> a >> b; a--, b--;

		int tot_special = pref_special[b+1] - pref_special[a];

		jump_t cur_a{a, 0, is_special[a] == '1', 0};
		while (nxt[cur_a.dest].dest < b) {
			if (nxt_long[cur_a.dest].dest < b) {
				cur_a = merge(cur_a, nxt_long[cur_a.dest]);
			} else {
				cur_a = merge(cur_a, nxt[cur_a.dest]);
			}
		}

		jump_t cur_b{b, 0, is_special[b] == '1', 0};
		while (prv[cur_b.dest].dest > a) {
			if (prv_long[cur_b.dest].dest > a) {
				cur_b = merge(cur_b, prv_long[cur_b.dest]);
			} else {
				cur_b = merge(cur_b, prv[cur_b.dest]);
			}
		}

		assert(cur_a.len + 1 == cur_b.len + 1);
		int d = cur_a.len + 1;

		int64_t special_dist = cur_a.special_dist + cur_b.special_dist;
		// handle stragglers from cur_a.
		special_dist += d * (tot_special - cur_a.num_special);
		special_dist += d * (tot_special - cur_b.num_special);

		int64_t num_good = int64_t(d + 1) * tot_special - special_dist;
		cout << d << ' ' << num_good << '\n';
	}

	return 0;
}
