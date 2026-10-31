#include<bits/stdc++.h>

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

int64_t solve() {
	using namespace std;

	int N, M, S, T; cin >> N >> M >> S >> T; S--, T--;
	vector<int> P(N);
	vector<int64_t> C(N);
	for (int i = 0; i < N; i++) {
		cin >> P[i] >> C[i]; P[i]--;
	}
	vector<int> path;
	{
		vector<int> xpath; xpath.reserve(N);
		for (int x = S; x != -1; x = P[x]) xpath.push_back(x);
		vector<int> ypath; ypath.reserve(N);
		for (int y = T; y != -1; y = P[y]) ypath.push_back(y);
		assert(xpath.back() == 0);
		assert(ypath.back() == 0);
		int lca;
		while (!xpath.empty() && !ypath.empty() && xpath.back() == ypath.back()) {
			lca = xpath.back();
			xpath.pop_back();
			ypath.pop_back();
		}
		path.reserve(xpath.size() + 1 + ypath.size());
		path.insert(path.end(), xpath.begin(), xpath.end());
		path.push_back(lca);
		path.insert(path.end(), ypath.rbegin(), ypath.rend());
		assert(path.front() == S);
		assert(path.back() == T);
	}

	map<int, int64_t> mp;
	mp[M] = 0;
	auto query_cost = [&mp](int x) -> optional<int64_t> {
		auto it = mp.lower_bound(x);
		if (it == mp.end()) return std::nullopt;
		else return it->second;
	};
	auto insert_pt = [&mp](int dest, int64_t cost) -> void {
		auto it = mp.lower_bound(dest);
		if (it != mp.end() && it->second <= cost) {
			// we're worthless
			return;
		}
		it = mp.upper_bound(dest);
		while (it != mp.begin()) {
			--it;
			assert(it->first <= dest);
			if (cost <= it->second) {
				it = mp.erase(it);
			} else {
				break;
			}
		}
		mp.insert(it, {dest, cost});
	};

	vector<vector<int>> adj(N);
	for (int i = 1; i < N; i++) {
		adj[P[i]].push_back(i);
		adj[i].push_back(P[i]);
	}

	for (int z = 1; z+1 < int(path.size()); z++) {
		auto process_pt = [&](int depth, int i) {
			if (!C[i]) return;
			auto path_to = query_cost(z + depth);
			if (!path_to) return;
			int64_t cost = C[i] + *path_to;
			insert_pt(z + M - depth, cost);
		};

		int rt = path[z];
		process_pt(0, rt);
		for (int ch : adj[rt]) {
			if (ch == path[z-1] || ch == path[z+1]) continue;
			std::y_combinator([&](auto self, int cur, int prv, int depth) -> void {
				process_pt(depth, cur);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur, depth+1);
				}
			})(ch, rt, 1);
		}
	}

	auto ans = query_cost(int(path.size())-1);
	if (!ans) return -1;
	return *ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ": " << solve() << '\n';
	}

	return 0;
}
