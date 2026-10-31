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
	assert(N >= 0);
	if (N == 0) {
		cout << "" << '\n';
		exit(0);
	} else if (N == 1) {
		cout << "0" << '\n';
		exit(0);
	} else if (N == 2) {
		cout << "01" << '\n';
		exit(0);
	}
	assert(N >= 3);
	int K = 0;
	while ((1 << K) + (K-1) <= N) K++;
	K--;
	assert(K >= 1);

	std::vector<int> seq; seq.reserve(1<<K);

	{
		std::mt19937 mt(48);
		std::vector<std::vector<int>> adj(1<<(K-1));
		for (int i = 0; i < (1 << (K-1)); i++) {
			int b = mt() & 1;
			adj[i].push_back((2*i+b) & ((1 << (K-1)) - 1));
			adj[i].push_back((2*i+!b) & ((1 << (K-1)) - 1));
		}

		seq.reserve(1<<K);
		std::y_combinator([&](auto self, int cur) -> void {
			while (!adj[cur].empty()) {
				int nxt = adj[cur].back();
				adj[cur].pop_back();
				self(nxt);
				seq.push_back((cur * 2) | nxt);
			}
		})(0);

		std::reverse(seq.begin(), seq.end());

		if (K == 1) {
			assert(seq[1] == 0);
			seq[1] = 1;
		}
	}

	assert(int(seq.size()) == 1<<K);

	std::vector<int> succ(1<<K);
	for (int i = 0; i < (1<<K); i++) {
		succ[seq[i]] = seq[(i+1)&((1<<K)-1)] ^ 1;
	}

	std::vector<bool> vis(1<<K);
	int target_len = N-(K-1);
	assert(target_len >= int(seq.size()));

	int cur_len = int(seq.size());

	/*
	for (int v : seq) {
		for (int b = 0; b < K; b++) {
			cerr << ((v >> b) & 1);
		}
		cerr << '\n';
	}
	cerr << '\n';
	*/

	std::vector<int> ans; ans.reserve(target_len);
	if (cur_len == target_len) {
		ans = seq;
	} else {
		for (int z = 0; z < (1 << K); z++) {
			int st = seq[z];
			ans.push_back(st);
			int cur = st;
			while (!vis[cur]) {
				vis[cur] = true;
				cur = succ[cur];
				ans.push_back(cur);
				cur_len++;
				if (cur_len == target_len) {
					ans.insert(ans.begin(), seq.begin() + z + 1, seq.end());
					goto done;
				}
			}
		}
		assert(false);
done:;
	}
	assert(int(ans.size()) == target_len);
	/*
	for (int v : ans) {
		for (int b = 0; b < K; b++) {
			cerr << ((v >> b) & 1);
		}
		cerr << '\n';
	}
	cerr << '\n';
	*/
	std::vector<int> real_ans(N);
	for (int i = 0; i < int(ans.size()); i++) {
		for (int b = 0; b < K; b++) {
			real_ans[i+K-1 - b] = (ans[i] >> b) & 1;
		}
	}
	for (int i = 0; i < N; i++) cout << real_ans[i];
	cout << '\n';

	return 0;
}
