#include <bits/stdc++.h>

const int64_t INF = 4e18;
// our states are 0, each half of 1->2, each half of 2->1
using mat_t = std::array<std::array<int64_t, 5>, 5>;
const mat_t ID {
	std::array<int64_t, 5>{0, INF, INF, INF, INF},
	{INF, 0, INF, INF, INF},
	{INF, INF, 0, INF, INF},
	{INF, INF, INF, 0, INF},
	{INF, INF, INF, INF, 0}
};
// you can do anything, it's all legal, except 1not -> 1has
const mat_t M_COW {
	std::array<int64_t, 5>{0, 0, 0, 0, 0},
	{0, 0, INF, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0}
};
// you're required to have something, you can stop if you're done, otherwise not allowed to change
const mat_t M_PACKAGE {
	std::array<int64_t, 5>{INF, 0, INF, 0, INF},
	{INF, 0, INF, INF, INF},
	{0, INF, 0, INF, INF},
	{INF, INF, INF, 0, INF},
	{0, INF, INF, INF, 0}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t M; int N_cows, N_packages; cin >> M >> N_cows >> N_packages;
	int S = 1;
	while (S < N_cows + N_packages + 1) S *= 2;
	std::vector<std::pair<int64_t, int>> vals; vals.reserve(S+1);
	vals.push_back({0, -1});

	std::vector<std::pair<int64_t, int>> evts; evts.reserve(2 * (N_cows + N_packages) + 1);
	for (int i = 0; i < N_cows + N_packages; i++) {
		int64_t l, r; cin >> l >> r;
		assert(l % M == r % M);
		std::pair<int64_t, int> v(l % M, i);
		vals.push_back(v);
		evts.push_back({l / M, i});
		evts.push_back({r / M + 1, ~i});
	}
	std::sort(vals.begin() + 1, vals.end());
	std::vector<int> v_idx(N_cows + N_packages);
	for (int i = 1; i < int(vals.size()); i++) {
		v_idx[vals[i].second] = i;
	}
	assert(int(vals.size()) <= S);
	while (int(vals.size()) <= S) vals.push_back({M, -1});
	std::sort(evts.begin(), evts.end());

	auto mul = [](mat_t a, mat_t b) -> mat_t {
		mat_t r;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				r[i][j] = std::min({a[i][0] + b[0][j], a[i][1] + b[1][j], a[i][2] + b[2][j], a[i][3] + b[3][j], a[i][4] + b[4][j]});
			}
		}
		return r;
	};
	auto pow = [mul](mat_t a, int64_t e) -> mat_t {
		mat_t r = ID;
		while (e) {
			if (e & 1) r = mul(r, a);
			e >>= 1;
			if (!e) break;
			a = mul(a, a);
		}
		return r;
	};

	std::vector<mat_t> seg(2*S, ID);
	auto update_node = [&](int a) -> void {
		assert(a < S);
		seg[a] = mul(seg[2*a+0], seg[2*a+1]);
	};
	auto update_leaf = [&](int v) -> void {
		int64_t l = vals[v+1].first - vals[v].first;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (seg[S+v][i][j] < INF) seg[S+v][i][j] += ((j >= 1) + (j >= 3)) * l;
			}
		}
	};
	for (int v = 0; v < S; v++) {
		seg[S+v] = ID;
		update_leaf(v);
	}
	for (int a = S-1; a > 0; a--) update_node(a);

	mat_t r = ID;

	int64_t cur_t = 0;
	for (auto [t, evt_idx] : evts) {
		assert(t >= cur_t);
		if (t > cur_t) {
			r = mul(r, pow(seg[1], t - cur_t));
			cur_t = t;
		}
		int v;
		if (evt_idx >= 0) {
			v = v_idx[evt_idx];
			if (evt_idx < N_cows) {
				// we're a cow, we can switch from anything to anything except 1 to 1
				seg[S+v] = M_COW;
			} else {
				// we can switch from anything to anything except 0 to 0
				seg[S+v] = M_PACKAGE;
			}
		} else {
			v = v_idx[~evt_idx];
			seg[S+v] = ID;
		}
		update_leaf(v);
		for (int a = (S+v)/2; a > 0; a /= 2) update_node(a);
	}
	cout << r[0][0] << '\n';

	return 0;
}
