#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string T; cin >> T;
	int N = int(T.size());
	std::vector<int> node_sz(2*N);
	for (int i = 2*N-1; i >= N; i--) {
		node_sz[i] = 1;
	}
	for (int i = N-1; i > 0; i--) {
		node_sz[i] = node_sz[2*i] + node_sz[2*i+1];
	}
	std::vector<std::array<int, 2>> bounds(2*N);
	bounds[1] = {0, N};
	for (int i = 1; i < N; i++) {
		bounds[2*i] = {bounds[i][0], bounds[i][0] + node_sz[2*i]};
		bounds[2*i+1] = {bounds[i][1] - node_sz[2*i+1], bounds[i][1]};
	}
	std::vector<int> point_node(N);
	for (int i = N; i < 2*N; i++) {
		point_node[bounds[i][0]] = i;
	}

	struct seg_t {
		// 0 -> 6
		std::array<int, 6> start_states;
		// 0 -> 7
		int64_t cur_cyc = 0;
		// 0 -> 8
		int64_t cur_ans = 0;
		// 6 -> 6, 6 -> 7, 6 -> 8
		std::array<std::tuple<int, int, int64_t>, 6> nxt_state;
		// 7 -> 8
		int num_ends = 0;
	};

	std::vector<seg_t> seg(2*N);
	auto update_node = [&](int a) -> void {
		const seg_t& x = seg[2*a];
		const seg_t& y = seg[2*a+1];
		seg_t& r = seg[a];

		// 0 -> 0 -> 6
		r.start_states = y.start_states;
		// 0 -> 0/7 -> 7
		r.cur_cyc = y.cur_cyc + x.cur_cyc;
		// 0 -> 0/7/8 -> 8
		r.cur_ans = y.cur_ans + int64_t(x.cur_cyc) * int64_t(y.num_ends) + x.cur_ans;
		// 7 -> 7/8 -> 8
		r.num_ends = x.num_ends + y.num_ends;

		// 0 -> 6 -> 6/7/8
		for (int i = 0; i < 6; i++) {
			auto [j, y_cyc, y_ans] = y.nxt_state[i];
			int x_ways = x.start_states[i];
			r.start_states[j] += x_ways;
			r.cur_cyc += int64_t(x_ways) * int64_t(y_cyc);
			r.cur_ans += int64_t(x_ways) * int64_t(y_ans);
		}

		// 6 -> 6/7/8 -> 6/7/8
		for (int i = 0; i < 6; i++) {
			auto [j, x_cyc, x_ans] = x.nxt_state[i];
			auto [k, y_cyc, y_ans] = y.nxt_state[j];
			r.nxt_state[i] = {k, x_cyc + y_cyc, x_ans + int64_t(x_cyc) * int64_t(y.num_ends) + y_ans};
		}
	};

	auto set_node = [&](int a, char c) -> void {
		seg[a].start_states.fill(0);
		seg[a].start_states[(c == 'b') ? 1 : 0] = 1;
		seg[a].cur_cyc = 0;
		seg[a].cur_ans = 0;
		seg[a].num_ends = 1;
		seg[a].nxt_state[0] = {(c == 'b') ? 1 : 0, 0, 0};
		seg[a].nxt_state[1] = {(c == 'e') ? 2 : 1, 0, 0};
		seg[a].nxt_state[2] = {(c == 's') ? 3 : 2, 0, 0};
		seg[a].nxt_state[3] = {(c == 's') ? 4 : 3, 0, 0};
		seg[a].nxt_state[4] = {(c == 'i') ? 5 : 4, 0, 0};
		seg[a].nxt_state[5] = {(c == 'e') ? 0 : 5, (c == 'e'), (c == 'e')};
	};
	for (int i = 0; i < N; i++) {
		int a = point_node[i];
		set_node(a, T[i]);
	}
	for (int a = N-1; a > 0; a--) update_node(a);

	cout << seg[1].cur_ans << '\n';
	int U; cin >> U;
	for (int u = 0; u < U; u++) {
		int i; cin >> i; i--;
		char c; cin >> c;
		T[i] = c;
		set_node(point_node[i], c);
		for (int a = point_node[i] / 2; a; a /= 2) update_node(a);
		cout << seg[1].cur_ans << '\n';
	}

	return 0;
}
