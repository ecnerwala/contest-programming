#include "plants.h"

#include <bits/stdc++.h>

namespace {

class plants_solver {
	struct index_data {
		int l_to_r;
		int l_to_r_2;
		int r_to_l;
		int r_to_l_2;
	};
	std::vector<index_data> v;
public:
	plants_solver() { }
	plants_solver(int N, int K, std::vector<int> R) : v(N) {
		using std::min;
		using std::max;

		assert(N == int(R.size()));
		assert(1 <= K && K <= N);
		for (int& r : R) assert(0 <= r && r < K);

		struct seg_node {
			int val;
			int ind;
			int lazy;
			int last_taken;
		};
		std::vector<seg_node> seg(2*N);
		auto update_node = [&](int a) {
			assert(a < N);
			if (seg[2*a].val < seg[2*a+1].val) {
				seg[a].val = seg[2*a].val;
				seg[a].ind = seg[2*a].ind;
			} else {
				seg[a].val = seg[2*a+1].val;
				seg[a].ind = seg[2*a+1].ind;
			}
			seg[a].val += seg[a].lazy;
		};

		for (int i = 0; i < N; i++) {
			seg[N+i] = seg_node{R[i], i, 0, -1};
		}
		for (int a = N-1; a; a--) {
			update_node(a);
			seg[a].last_taken = -1;
		}

		auto downdate_all = [&](int n) {
			n >>= __builtin_ctz(n); n >>= 1;
			if (!n) return;
			for (int l = 31 - __builtin_clz(n); l >= 0; l--) {
				int a = n >> l;
				assert(a < N);
				seg[2*a].val += seg[a].lazy;
				seg[2*a].lazy += seg[a].lazy;
				seg[2*a+1].val += seg[a].lazy;
				seg[2*a+1].lazy += seg[a].lazy;
				seg[a].lazy = 0;
			}
		};

		std::vector<int> toposort(N); // We'll use the tail of toposort as a stack.
		std::vector<int> prev_bigger(N);
		std::vector<int> next_bigger(N);
		for (int t = 0, stk = N; t < N; t++) {
			if (stk == N) {
				assert(seg[1].val == 0);
				toposort[--stk] = seg[1].ind;
			}

			int cur = toposort[stk];
			//std::cerr << "trying " << cur << '\n';

			int prev_taken = -1;
			int lo = cur-(K-1), hi = cur;
			if (lo < 0) {
				lo += 2 * N, hi = 2 * (N + hi);
			} else {
				lo += N, hi += N;
			}

			downdate_all(lo);
			downdate_all(hi);

			int zero_ind = -1;
			for (int a = lo, b = hi; a < b; a >>= 1, b >>= 1) {
				if (a & 1) {
					if (seg[a].val == 0) {
						zero_ind = seg[a].ind;
						goto found_zero;
					}
					prev_taken = max(prev_taken, seg[a].last_taken);
					a++;
				}
				if (b & 1) {
					--b;
					if (seg[b].val == 0) {
						zero_ind = seg[b].ind;
						goto found_zero;
					}
					prev_taken = max(prev_taken, seg[b].last_taken);
				}
			}
			goto not_found_zero;
found_zero:
			{
				//std::cerr << "bad: " << zero_ind << '\n';
				toposort[--stk] = zero_ind;
				t--;
				continue;
			}
not_found_zero:;
			int next_taken = -1;
			{
				int lo2 = cur+1, hi2 = cur+K;
				assert(lo2 <= N);
				lo2 += N;
				if (hi2 > N) {
					hi2 = 2 * hi2;
				} else {
					hi2 += N;
				}
				for (int a = lo2, b = hi2; a < b; a >>= 1, b >>= 1) {
					if (a & 1) {
						next_taken = max(next_taken, seg[a].last_taken);
						a++;
					}
					if (b & 1) {
						--b;
						next_taken = max(next_taken, seg[b].last_taken);
					}
				}
			}

			assert(toposort[stk] == cur);
			++stk;
			toposort[t] = cur;
			prev_bigger[cur] = (prev_taken == -1) ? -1 : toposort[prev_taken];
			next_bigger[cur] = (next_taken == -1) ? -1 : toposort[next_taken];

			//std::cerr << "take " << cur << ' ' << prev_bigger[cur] << ' ' << next_bigger[cur] << '\n';

			seg[N+cur].last_taken = t;
			seg[N+cur].val += K; // should never be bad
			for (int a = lo, b = hi; a < b; a >>= 1, b >>= 1) {
				if (a & 1) {
					seg[a].lazy--;
					seg[a].val--;
					a++;
				}
				if (b & 1) {
					--b;
					seg[b].lazy--;
					seg[b].val--;
				}
			}
			for (int a = (N+cur) >> 1; a; a >>= 1) { update_node(a); seg[a].last_taken = t; }
			for (int a = lo >> __builtin_ctz(lo) >> 1; a; a >>= 1) update_node(a);
		}

		{
			std::vector<int> pred(2*N+1, -1);
			{
				int cur = 2*N;
				for (int a : toposort) {
					if (a > N-K) {
						pred[cur] = N+a;
						cur = N+a;
					}
				}
			}
			for (int i = 2*N-K; i >= 0; i--) {
				assert(i + K <= 2 * N);
				int j = next_bigger[i >= N ? i-N : i];
				if (j == -1) {
					j = 2 * N;
				} else {
					if (j < i) j += N;
					assert(i < j && j < i + K);
				}
				pred[i] = pred[j];
				pred[j] = i;
			}
			for (int cur = pred[2*N], idx = 2*N-1; idx >= 0; cur = pred[cur], idx--) {
				if (cur >= N) {
					v[cur - N].l_to_r_2 = idx;
				} else {
					v[cur].l_to_r = idx;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			//std::cerr << v[i].l_to_r << ' ' << v[i].l_to_r_2 << '\n';
			assert(v[i].l_to_r > v[i].l_to_r_2);
		}

		{
			std::vector<int> pred(2*N+1, -1);
			{
				int cur = 2 * N;
				for (int a : toposort) {
					if (a < K) {
						pred[cur] = a;
						cur = a;
					}
				}
			}
			for (int i = K; i < 2*N; i++) {
				int j = prev_bigger[i >= N ? i-N : i];
				if (j == -1) {
					j = 2 * N;
				} else {
					if (j + N < i) {
						j += N;
					}
					assert(i - K < j && j < i);
				}
				pred[i] = pred[j];
				pred[j] = i;
			}
			for (int cur = pred[2*N], idx = 2*N-1; idx >= 0; cur = pred[cur], idx--) {
				if (cur >= N) {
					v[cur - N].r_to_l_2 = idx;
				} else {
					v[cur].r_to_l = idx;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			//std::cerr << v[i].r_to_l << ' ' << v[i].r_to_l_2 << '\n';
			assert(v[i].r_to_l_2 > v[i].r_to_l);
		}
	}

public:
	int compare_plants(int x, int y) const {
		if (x == y) return 0;
		if (x > y) {
			return -compare_plants(y, x);
		}
		assert(x < y);
		const auto& a = v[x];
		const auto& b = v[y];
		if (a.l_to_r < b.l_to_r) {
			return -1;
		}
		if (b.l_to_r < a.l_to_r_2) {
			return 1;
		}
		if (a.r_to_l_2 < b.r_to_l) {
			return -1;
		}
		if (b.r_to_l < a.r_to_l) {
			return 1;
		}

		return 0;
	}
} SOLVER;

}

void init(int K, std::vector<int> R) {
	int N = int(R.size());
	SOLVER = plants_solver(N, K, R);
}

int compare_plants(int x, int y) {
	return SOLVER.compare_plants(x, y);
}
