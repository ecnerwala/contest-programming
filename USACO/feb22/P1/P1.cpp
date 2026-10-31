#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// Let's count the total winding number, tracing each region CW.
	// Each intersection adds 2 to the white and black winding numbers.
	// Each black (interior) corner adds 1 to the black winding number and -1 to the white.

	// The total number of regions is the total winding number divided by 4,
	// except the white gets +1 + # cc's because the outer face has winding number -# cc's.

	int N, T; cin >> N >> T;
	std::vector<std::array<std::array<int, 2>, 2>> rects(N);
	for (auto& r : rects) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		xlo--, ylo--, xhi--, yhi--;
		r[0] = {xlo, xhi};
		r[1] = {ylo, yhi};
	}

	const int X = 2*N;

	std::vector<std::pair<int, std::array<int, 2>>> x_evts(X);
	for (int i = 0; i < N; i++) {
		x_evts[rects[i][0][0]] = {2*i+0, rects[i][1]};
		x_evts[rects[i][0][1]] = {2*i+1, rects[i][1]};
	}

	int64_t num_inter = 0;
	int vert_black_minus_white = 0;

	std::vector<int> rect_color(X);
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	// id if there's exactly 1, else -1 if empty else -2
	std::vector<int> seg_cc(2*X, -1);
	std::vector<int> seg_cnt(2*X, 0);
	auto update_node = [&](int a) {
		assert(a < X);
		int x = seg_cc[2*a+0];
		int y = seg_cc[2*a+1];
		if (x == -2 || y == -2) {
			seg_cc[a] = -2;
			return;
		}
		if (x == -1) {
			seg_cc[a] = y;
			return;
		}
		if (y == -1) {
			seg_cc[a] = x;
			return;
		}
		x = get_par(x);
		y = get_par(y);
		if (x == y) {
			seg_cc[a] = x;
			return;
		}

		seg_cc[a] = -2;
	};

	auto connect_node = std::y_combinator([&](auto self, int a, int conn) -> void {
		if (seg_cc[a] == -1) return;
		if (seg_cc[a] != -2) {
			merge(seg_cc[a], conn);
			return;
		}
		self(2*a+0, conn);
		self(2*a+1, conn);
		update_node(a);
		assert(seg_cc[a] == get_par(conn));
	});

	for (auto [t, ys] : x_evts) {
		if ((t&1) == 1) {
			// delete these lines
			for (int y : ys) {
				for (int a = X+y; a; a >>= 1) seg_cnt[a] --;

				seg_cc[X+y] = -1;
				for (int a = (X+y)>>1; a; a >>= 1) update_node(a);
			}
		}
		int cur_inter = 0;
		{
			for (int a = X+ys[0], b = X+ys[1]; a < b; a >>= 1, b >>= 1) {
				if (a & 1) {
					cur_inter += seg_cnt[a];
					connect_node(a, t/2);
					a++;
				}
				if (b & 1) {
					--b;
					cur_inter += seg_cnt[b];
					connect_node(b, t/2);
				}
			}
			// It's okay to the leave the parents as garbage; that only adds O(log(N)) amortized cost in the future.
			//rng.for_parents_up(update_node);
		}
		num_inter += cur_inter;
		int cur_color = 0;
		{
			for (int a = X, b = X+ys[0]; a < b; a >>= 1, b >>= 1) {
				if (a & 1) {
					cur_color += seg_cnt[a++];
				}
				if (b & 1) {
					cur_color += seg_cnt[--b];
				}
			}
		}
		int nxt_color = cur_color + cur_inter;
		if ((t&1) == 0) {
			rect_color[t/2] = (cur_color & 1) ? -1 : +1;
		}
		vert_black_minus_white += (cur_color & 1) ? -1 : +1;
		vert_black_minus_white += (nxt_color & 1) ? -1 : +1;

		if ((t&1) == 0) {
			// insert these lines
			for (int y : ys) {
				for (int a = X+y; a; a >>= 1) seg_cnt[a] ++;

				seg_cc[X+y] = t/2;
				for (int a = (X+y)>>1; a; a >>= 1) update_node(a);
			}
		}
	}

	int num_white_ccs = 0;
	int num_black_ccs = 0;

	std::vector<int> cc_color(N);
	for (auto [t, ys] : x_evts) {
		if ((t&1) == 0) {
			int i = t/2;
			int c = get_par(i);
			if (!cc_color[c]) {
				cc_color[c] = rect_color[i];
				if (rect_color[i] == 1) num_white_ccs++;
				else num_black_ccs++;
			}
		}
	}

	//assert(num_white_ccs + num_black_ccs == 1);
	int64_t num_white = (num_inter * 2 - vert_black_minus_white) / 4 + 1 + num_white_ccs;
	int64_t num_black = (num_inter * 2 + vert_black_minus_white) / 4 + num_black_ccs;

	if (T == 1) {
		cout << num_white + num_black << '\n';
	} else if (T == 2) {
		cout << num_white << ' ' << num_black << '\n';
	} else assert(false);

	return 0;
}
