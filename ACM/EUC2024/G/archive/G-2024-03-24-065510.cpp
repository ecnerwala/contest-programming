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

	std::string A; cin >> A;
	std::string B; cin >> B;

	assert(int(A.size()) == N);
	assert(int(B.size()) == N);

	std::array<std::vector<std::pair<int, int>>, 3> adj;
	adj[0].reserve(N);
	adj[1].reserve(N);
	adj[2].reserve(N);

	std::array<int, 3> in_deg{0,0,0};
	std::array<int, 3> out_deg{0,0,0};

	auto char_to_int = [](char c) {
		if (c == '-') return 0;
		else if (c == 'C') return 1;
		else if (c == 'M') return 2;
		else assert(false);
	};

	for (int i = 0; i < N; i++) {
		// a is the need, b is the have
		int a = char_to_int(A[i]);
		int b = char_to_int(B[i]);
		if (a == b) continue;
		adj[b].push_back({a, i});
		in_deg[a]++;
		out_deg[b]++;
	}
	assert(out_deg[1] >= in_deg[1]);
	assert(out_deg[2] >= in_deg[2]);
	assert(out_deg[0] <= in_deg[0]);
	for (int z : {1, 2}) {
		while (in_deg[z] < out_deg[z]) {
			adj[0].push_back({z, -1});
			out_deg[0]++;
			in_deg[z]++;
		}
	}
	assert(out_deg[0] == in_deg[0]);
	assert(out_deg[1] == in_deg[1]);
	assert(out_deg[2] == in_deg[2]);

	std::vector<std::pair<int, int>> ans; ans.reserve(3*N);
	std::y_combinator([&](auto self, int cur) -> void {
		while (!adj[cur].empty()) {
			auto [nxt, e] = adj[cur].back();
			adj[cur].pop_back();
			self(nxt);
			if (e == -1) {
				assert(cur == 0 && nxt != 0);
				assert((!ans.empty() && ans.back() == std::pair<int, int>(1, nxt)));
				ans.pop_back();
			} else {
				ans.push_back({0, e});
				if (nxt != 0) {
					// drop off
					ans.push_back({2, nxt});
				}
				if (cur != 0) {
					ans.push_back({1, cur});
				}
			}
		}
	})(0);

	/*
	cerr << ans.size() << '\n';
	for (auto [op, v] : ans) {
		if (op == 0) {
			cerr << "DRIVE" << ' ' << v+1 << '\n';
		} else if (op == 1) {
			cerr << "PICKUP" << ' ' << "-CM"[v] << '\n';
		} else if (op == 2) {
			cerr << "DROPOFF" << ' ' << "-CM"[v] << '\n';
		} else assert(false);
	}
	*/

	cout << ans.size() << '\n';
	for (auto [op, v] : ans) {
		if (op == 0) {
			cout << "DRIVE" << ' ' << v+1 << '\n';
		} else if (op == 1) {
			cout << "PICKUP" << '\n';
		} else if (op == 2) {
			cout << "DROPOFF" << '\n';
		} else assert(false);
	}

	return 0;
}
