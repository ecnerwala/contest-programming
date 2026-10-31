#include <bits/stdc++.h>

void wssert(bool b) {
	if (!b) {
		std::cout << "GARBAGE" << '\n';
		std::cout << "GARBAGE" << '\n';
		std::cout << "GARBAGE" << '\n';
		std::cout << "GARBAGE" << '\n';
		std::cout << "GARBAGE" << '\n';
		exit(0);
	}
}

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

	int Q; cin >> Q;

	std::string all_inputs; all_inputs.reserve(int(2e6));

	struct node_t {
		std::array<int, 2> c;
		int st, own_len;
		//int64_t sz;
		int64_t len;
	};
	std::vector<node_t> nodes; nodes.reserve(int(4e6));
	// 0 is null
	nodes.push_back(node_t{
		{0, 0},
		0, 0,
		//0,
		0,
	});

	std::mt19937_64 mt(48);

	auto update_node = [&](int r) {
		//nodes[r].sz = 1 + nodes[nodes[r].c[0]].sz + nodes[nodes[r].c[1]].sz;
		nodes[r].len = nodes[r].own_len + nodes[nodes[r].c[0]].len + nodes[nodes[r].c[1]].len;
	};

	auto do_split_3 = std::y_combinator([&](auto self, int state, int64_t l, int64_t r) -> std::tuple<int, int, int> {
		assert(0 <= l && l <= r && r <= nodes[state].len);
		if (r == 0) {
			return {0, 0, state};
		}
		if (l == nodes[state].len) {
			return {state, 0, 0};
		}
		if (l == 0 && r == nodes[state].len) {
			return {0, state, 0};
		}
		assert(state);

		auto [c0, c1] = nodes[state].c;

		int64_t own_l = nodes[c0].len;
		int64_t own_r = nodes[c0].len + nodes[state].own_len;

		auto [c0_a, c0_b, c0_c] = self(c0, std::min(l, own_l), std::min(r, own_l));
		auto [c1_a, c1_b, c1_c] = self(c1, std::max(l, own_r) - own_r, std::max(r, own_r) - own_r);

		// Now, figure out what we actually do
		int a = 0, b = 0, c = 0;
		// first split ourselves
		if (own_l < l) {
			int64_t st = own_l;
			int64_t en = std::min(own_r, l);
			a = int(nodes.size());
			nodes.push_back({
				{c0_a, c1_a},
				int(nodes[state].st + st - own_l), int(en - st),
				//0,
				0,
			});
			update_node(a);
		} else if (c0_a || c1_a) {
			assert(!(c0_a && c1_a));
			assert(!c1_a);
			a = c0_a ^ c1_a;
		}

		if (std::max(l, own_l) < std::min(r, own_r)) {
			int64_t st = std::max(own_l, l);
			int64_t en = std::min(own_r, r);
			b = int(nodes.size());
			nodes.push_back({
				{c0_b, c1_b},
				int(nodes[state].st + st - own_l), int(en - st),
				//0,
				0,
			});
			update_node(b);
		} else if (c0_b || c1_b) {
			assert(!(c0_b && c1_b));
			b = c0_b ^ c1_b;
		}

		if (r < own_r) {
			int64_t st = std::max(own_l, r);
			int64_t en = own_r;
			c = int(nodes.size());
			nodes.push_back({
				{c0_c, c1_c},
				int(nodes[state].st + st - own_l), int(en - st),
				//0,
				0,
			});
			update_node(c);
		} else if (c0_c || c1_c) {
			assert(!(c0_c && c1_c));
			assert(!c0_c);
			c = c0_c ^ c1_c;
		}

		return {a, b, c};
	});

	auto do_merge_3 = std::y_combinator([&](auto self, int a, int b, int c) -> int {
		if (a == 0 && b == 0) return c;
		if (a == 0 && c == 0) return b;
		if (b == 0 && c == 0) return a;

		int64_t my_root = std::uniform_int_distribution<int64_t>(0, nodes[a].len + nodes[b].len + nodes[c].len - 1)(mt);

		int r;
		if (my_root < nodes[a].len) {
			int v1 = self(nodes[a].c[1], b, c);

			r = int(nodes.size());
			nodes.push_back(nodes[a]);
			nodes[r].c[1] = v1;
		} else if (my_root < nodes[a].len + nodes[b].len) {
			int v0 = self(a, nodes[b].c[0], 0);
			int v1 = self(0, nodes[b].c[1], c);

			r = int(nodes.size());
			nodes.push_back(nodes[b]);
			nodes[r].c[0] = v0;
			nodes[r].c[1] = v1;
		} else {
			int v0 = self(a, b, nodes[c].c[0]);

			r = int(nodes.size());
			nodes.push_back(nodes[c]);
			nodes[r].c[0] = v0;
		}
		update_node(r);
		return r;
	});

	auto do_copy = std::y_combinator([&](auto self, int state, int64_t l, int64_t r) -> int {
		return std::get<1>(do_split_3(state, l, r));
	});
	auto do_insert = std::y_combinator([&](auto self, int state, int64_t p, int node) -> int {
		auto [l, m, r] = do_split_3(state, p, p);
		assert(m == 0);
		return do_merge_3(l, node, r);
	});
	auto do_erase = std::y_combinator([&](auto self, int state, int64_t l, int64_t r) -> int {
		auto [a, b, c] = do_split_3(state, l, r);
		return do_merge_3(a, 0, c);
	});
	// state, clipboard
	auto do_cut = std::y_combinator([&](auto self, int state, int64_t l, int64_t r) -> std::pair<int, int> {
		auto [a, b, c] = do_split_3(state, l, r);
		return {do_merge_3(a, 0, c), b};
	});
	auto do_print = std::y_combinator([&](auto self, int state, int64_t l, int64_t r) -> void {
		//cerr << "printing " << state << ' ' << l << ' ' << r << ' ' << nodes[state].len << '\n';
		assert(0 <= l);
		assert(l < r);
		assert(r <= nodes[state].len);
		auto [c0, c1] = nodes[state].c;
		if (l < nodes[c0].len) {
			self(c0, l, std::min(nodes[c0].len, r));
		}
		l -= nodes[c0].len;
		if (l < 0) l = 0;
		r -= nodes[c0].len;
		if (r <= 0) return;
		if (l < nodes[state].own_len) {
			for (int z = int(l); z < std::min<int64_t>(r, nodes[state].own_len); z++) {
				cout << all_inputs[nodes[state].st + z];
			}
		}
		l -= nodes[state].own_len;
		if (l < 0) l = 0;
		r -= nodes[state].own_len;
		if (r <= 0) return;
		self(c1, l, r);
	});

	std::vector<int> states; states.reserve(Q+1);
	states.push_back(0);
	int cur_state_idx = 0;
	int clipboard = 0;

	for (int q = 0; q < Q; q++) {
		string op; cin >> op;
		if (op == "insert") {
			int64_t p; std::string s; cin >> p >> s;
			int st = int(all_inputs.size());
			all_inputs += s;
			int en = int(all_inputs.size());

			int v = int(nodes.size());
			nodes.push_back(node_t{
				{0, 0},
				st, en - st,
				//1,
				en-st,
			});

			int new_state = do_insert(states[cur_state_idx], p, v);
			states.resize(cur_state_idx+1);
			states.push_back(new_state);
			cur_state_idx++;
		} else if (op == "erase") {
			int64_t l, r; cin >> l >> r;

			int new_state = do_erase(states[cur_state_idx], l, r);
			states.resize(cur_state_idx+1);
			states.push_back(new_state);
			cur_state_idx++;
		} else if (op == "print") {
			int64_t l, r; cin >> l >> r;

			do_print(states[cur_state_idx], l, r);
			cout << '\n';
		} else if (op == "copy") {
			int64_t l, r; cin >> l >> r;

			clipboard = do_copy(states[cur_state_idx], l, r);
		} else if (op == "cut") {
			int64_t l, r; cin >> l >> r;

			auto [new_state, new_clipboard] = do_cut(states[cur_state_idx], l, r);
			clipboard = new_clipboard;
			states.resize(cur_state_idx+1);
			states.push_back(new_state);
			cur_state_idx++;
		} else if (op == "paste") {
			int64_t p; cin >> p;

			int new_state = do_insert(states[cur_state_idx], p, clipboard);
			states.resize(cur_state_idx+1);
			states.push_back(new_state);
			cur_state_idx++;
		} else if (op == "serialize") {
			assert(q+1 == Q);

			std::vector<bool> touched(int(nodes.size()));
			touched[states[cur_state_idx]] = true;
			int num_touched = 0;
			for (int i = int(nodes.size()) - 1; i >= 0; i--) {
				if (touched[i]) {
					num_touched++;
					touched[nodes[i].c[0]] = true;
					touched[nodes[i].c[1]] = true;
				}
			}

			assert(touched[0]);
			std::vector<int> new_idx(int(nodes.size()), -1);

			std::vector<std::pair<int, int>> intervals;
			int nxt_idx = 0;
			for (int i = 0; i < int(nodes.size()); i++) {
				if (touched[i]) {
					new_idx[i] = nxt_idx++;
					intervals.push_back({nodes[i].st, nodes[i].own_len});
				}
			}
			std::sort(intervals.begin(), intervals.end());
			intervals.resize(std::unique(intervals.begin(), intervals.end()) - intervals.begin());

			std::string s; s.reserve(1e7);
			auto write_int = [&](int a) {
				assert(0 <= a && a < (1 << 24));
				s += char(33 + ((a >> 0) & 63));
				s += char(33 + ((a >> 6) & 63));
				s += char(33 + ((a >> 12) & 63));
				s += char(33 + ((a >> 18) & 63));
			};

			write_int(int(all_inputs.size()));
			s += all_inputs;
			write_int(int(intervals.size()));
			for (auto [a, b] : intervals) {
				write_int(a);
				write_int(b);
			}
			write_int(num_touched);
			for (int i = 0; i < int(nodes.size()); i++) {
				if (touched[i]) {
					write_int(new_idx[nodes[i].c[0]]);
					write_int(new_idx[nodes[i].c[1]]);
					int interval_idx = int(std::lower_bound(intervals.begin(), intervals.end(), std::pair<int, int>{nodes[i].st, nodes[i].own_len}) - intervals.begin());
					write_int(interval_idx);
				}
			}
			assert(int(s.size()) <= int(1e7));

			cout << s << '\n';
		} else if (op == "deserialize") {
			assert(q == 0);

			std::string s; cin >> s;
			auto it = s.begin();
			auto read_int = [&]() -> int {
				int a = *it - 33;
				it++;
				a += (*it - 33) << 6;
				it++;
				a += (*it - 33) << 12;
				it++;
				a += (*it - 33) << 18;
				it++;
				return a;
			};

			int all_inputs_sz = read_int();
			assert(all_inputs.empty());
			all_inputs.insert(all_inputs.end(), it, it + all_inputs_sz);
			it += all_inputs_sz;

			int interval_sz = read_int();
			std::vector<std::pair<int, int>> intervals(interval_sz);
			for (int i = 0; i < interval_sz; i++) {
				intervals[i].first = read_int();
				intervals[i].second = read_int();
			}

			int nodes_sz = read_int();
			assert(nodes.size() == 1);
			nodes.resize(nodes_sz);
			for (int i = 0; i < int(nodes_sz); i++) {
				int c0 = read_int();
				int c1 = read_int();
				int interval_idx = read_int();
				nodes[i].c[0] = c0;
				nodes[i].c[1] = c1;
				nodes[i].st = intervals[interval_idx].first;
				nodes[i].own_len = intervals[interval_idx].second;
				if (i == 0) {
					assert(c0 == 0 && c1 == 0);
				} else {
					assert(c0 < i && c1 < i);
					update_node(i);
				}
			}

			assert(cur_state_idx == 0 && states[cur_state_idx] == 0);

			int new_state = int(nodes.size()) - 1;
			states.resize(cur_state_idx+1);
			states.push_back(new_state);
			cur_state_idx++;
		} else if (op == "undo") {
			if (cur_state_idx - 1 >= 0) {
				cur_state_idx--;
			}
		} else if (op == "redo") {
			if (cur_state_idx + 1 < int(states.size())) {
				cur_state_idx++;
			}
		} else assert(false);
	}

	return 0;
}
