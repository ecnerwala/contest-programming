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

	int N; cin >> N;
	vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i], P[i]--; }

	vector<int> sz(N, 1);
	vector<vector<int>> ch(N);
	vector<pair<int, int>> best_ch(N, {-1, -1});
	for (int i = N-1; i > 0; i--) {
		ch[P[i]].push_back(i);
		sz[P[i]] += sz[i];
		if (sz[i] > best_ch[P[i]].first) best_ch[P[i]] = { sz[i], i };
	}
	for (int i = 0; i < N; i++) {
	}

	auto send_node = [&](int cur) -> void {
		cout << '+' << cur+1;
	};
	auto return_node = [&](int cur) -> void {
		assert(0 <= cur && cur < N);
		cout << '-';
	};

	auto send_subtree = std::y_combinator([&](auto self, int cur) -> void {
		cout << '+' << cur+1;
		for (int nxt : ch[cur]) self(nxt);
	});
	auto return_subtree = [&](int cur) -> void {
		cout << string(sz[cur], '-');
	};

	auto solve = std::y_combinator([&](auto solve_root, int root) -> void {
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> others;
		// First, do the heavy path
		int cur = root;
		while (true) {
			cout << '=' << cur+1;
			if (ch[cur].empty()) break;
			send_node(cur);
			for (int nxt : ch[cur]) {
				if (nxt == best_ch[cur].second) continue;
				others.push({sz[nxt], ~nxt});
				send_subtree(nxt);
			}
			cur = best_ch[cur].second;
		}
		while (cur != root) {
			cur = P[cur];
			for (int nxt : ch[cur]) {
				if (nxt == best_ch[cur].second) continue;
				return_subtree(nxt);
			}
			return_node(cur);
		}

		if (others.empty()) return;

		vector<pair<int, pair<int, int>>> ops; ops.reserve(int(others.size())-1);

		// Now, do the Huffman coding segtree thing
		while (others.size() > 1) {
			auto [s1, t1] = others.top(); others.pop();
			auto [s2, t2] = others.top(); others.pop();
			int idx = int(ops.size());
			ops.push_back({s1+s2, {t1, t2}});
			others.push({s1+s2, idx});
		}

		auto send_op = std::y_combinator([&](auto self, int a) -> void {
			if (a < 0) {
				send_subtree(~a);
			} else {
				self(ops[a].second.first);
				self(ops[a].second.second);
			}
		});
		auto return_op = [&](int a) -> void {
			if (a < 0) {
				return_subtree(~a);
			} else {
				cout << string(ops[a].first, '-');
			}
		};

		while (true) {
			send_node(cur);
			if (ch[cur].empty()) break;
			cur = best_ch[cur].second;
		}

		std::y_combinator([&](auto self, int op) -> void {
			if (op < 0) {
				solve_root(~op);
				return;
			}

			auto [x, y] = ops[op].second;
			send_op(x);
			self(y);
			return_op(x);

			send_op(y);
			self(x);
			return_op(y);
		})(others.top().second);

		while (true) {
			return_node(cur);
			if (cur == root) break;
			cur = P[cur];
		}
	});
	solve(0);
	cout << '!' << '\n';

	return 0;
}
