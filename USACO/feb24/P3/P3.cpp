#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L, Q; cin >> L >> Q;
	std::vector<int> T(L);
	for (auto& t : T) cin >> t;

	std::vector<int> pref(L+1);
	for (int i = 0; i < L; i++) pref[i+1] = pref[i] + T[i];

	const int V = pref[L];

	std::vector<int> edges(V);
	for (auto& j : edges) { cin >> j; j--; }

	std::vector<std::pair<int, int>> by_time(L);
	for (int i = 0; i < L; i++) {
		by_time[i] = {T[i], i};
	}
	std::sort(by_time.begin(), by_time.end());

	struct jump_t {
		int dest_loc;
		int64_t time;
	};
	std::vector<std::array<jump_t, 20>> same_jump(V);
	std::vector<jump_t> bigger_jump(V, {-1, -1});

	for (int T_cur = 1, z_lo = 0, z_hi = 0; z_lo < L; z_lo = z_hi, T_cur *= 2) {
		while (z_hi < L && by_time[z_hi].first == T_cur) {
			z_hi++;
		}
		cerr << T_cur << ' ' << z_lo << '-' << z_hi << '\n';

		for (int z = z_lo; z < z_hi; z++) {
			int loc = by_time[z].second;
			for (int v = pref[loc]; v < pref[loc+1]; v++) {
				int cur_loc = edges[v];
				int64_t cur_time = 1;
				while (true) {
					if (T[cur_loc] < T_cur) {
						auto [nxt_loc, jump_time] = bigger_jump[pref[cur_loc] + int(((v - pref[loc] + cur_time) & int64_t(T[cur_loc] - 1)))];
						if (nxt_loc == -1) {
							same_jump[v][0] = {-1, -1};
							bigger_jump[v] = {-1, -1};
							break;
						}
						assert(T[nxt_loc] > T[cur_loc]);
						cur_loc = nxt_loc;
						cur_time += jump_time;
					} else if (T[cur_loc] == T_cur) {
						same_jump[v][0] = {cur_loc, cur_time};
						bigger_jump[v] = {-1, -1};
						break;
					} else if (T[cur_loc] > T_cur) {
						same_jump[v][0] = {-1, -1};
						bigger_jump[v] = {cur_loc, cur_time};
						break;
					} else assert(false);
				}
			}
		}

		for (int b = 1; b < 20; b++) {
			for (int z = z_lo; z < z_hi; z++) {
				int loc = by_time[z].second;
				for (int v = pref[loc]; v < pref[loc+1]; v++) {
					auto [cur_loc, cur_time] = same_jump[v][b-1];
					if (cur_loc == -1) {
						same_jump[v][b] = {-1, -1};
						continue;
					}
					assert(T[cur_loc] == T_cur);
					int nxt_v = pref[cur_loc] + int(((v - pref[loc] + cur_time) & int64_t(T[cur_loc] - 1)));
					{
						auto [nxt_loc, jump_time] = same_jump[nxt_v][b-1];
						if (nxt_loc != -1) {
							same_jump[v][b] = {nxt_loc, jump_time + cur_time};
						} else {
							same_jump[v][b] = {-1, -1};
						}
					}
					{
						auto [nxt_loc, jump_time] = bigger_jump[nxt_v];
						if (nxt_loc != -1) {
							bigger_jump[v] = {nxt_loc, jump_time + cur_time};
						}
					}
				}
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		int loc; int64_t cur_time; int64_t rem_time;
		cin >> loc >> cur_time >> rem_time; loc--;
		while (rem_time > 0) {
			int T_cur = T[loc];
			int v = pref[loc] + int(cur_time & int64_t(T_cur - 1));
			{
				auto [nxt_loc, jump_time] = bigger_jump[v];
				if (nxt_loc != -1 && jump_time <= rem_time) {
					loc = nxt_loc;
					cur_time += jump_time;
					rem_time -= jump_time;
					continue;
				}
			}
			for (int b = 19; b >= 0; b--) {
				auto [nxt_loc, jump_time] = same_jump[v][b];
				if (nxt_loc != -1 && jump_time <= rem_time) {
					assert(T[nxt_loc] == T_cur);
					loc = nxt_loc;
					cur_time += jump_time;
					rem_time -= jump_time;
					v = pref[loc] + int(cur_time & int64_t(T_cur - 1));
					continue;
				}
			}
			if (rem_time > 0) {
				loc = edges[v];
				cur_time++;
				rem_time--;
			}
		}
		assert(rem_time == 0);
		cout << loc << '\n';
	}

	return 0;
}
