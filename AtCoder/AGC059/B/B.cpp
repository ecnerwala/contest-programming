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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& c : C) { cin >> c; c--; }

		std::sort(C.begin(), C.end());

		int num_distinct = 0;
		for (int i = 0; i < N; i++) {
			if (i == 0 || C[i] != C[i-1]) num_distinct++;
		}
		assert(num_distinct >= 1);
		if (num_distinct >= 2 && N >= 2 * (num_distinct - 1)) {
			// Build the tree
			std::vector<int> freq(N);
			for (auto& c : C) freq[c]++;

			int extra_deg = num_distinct - 2;
			std::vector<int> deg(N);
			for (int i = 0; i < N; i++) {
				if (!freq[i]) continue;
				deg[i] = 1 + std::min(freq[i] - 1, extra_deg);
				extra_deg -= deg[i] - 1;
				freq[i] -= deg[i];
			}

			std::vector<int> leaves; leaves.reserve(N);
			for (int i = 0; i < N; i++) {
				if (deg[i] == 1) leaves.push_back(i);
			}
			std::vector<std::vector<int>> adj(N);
			for (int i = 0; i < N; i++) {
				if (deg[i] >= 2) {
					// try to combine as many leaves as possible
					for (int z = 0; z < deg[i]-1; z++) {
						assert(!leaves.empty());
						int j = leaves.back(); leaves.pop_back();
						adj[i].push_back(j);
						adj[j].push_back(i);
					}
					leaves.push_back(i);
				}
			}
			assert(leaves.size() == 2);
			adj[leaves[0]].push_back(leaves[1]);
			adj[leaves[1]].push_back(leaves[0]);

			std::vector<int> ans; ans.reserve(N);
			auto push_val = [&](int v) -> void {
				for (int z = 0; z < freq[v]+1; z++) {
					ans.push_back(v);
				}
				freq[v] = 0;
			};
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					push_val(cur);
					self(nxt, cur);
					push_val(nxt);
				}
			})(leaves[0], -1);

			for (int i = 0; i < N; i++) {
				cout << ans[i]+1 << " \n"[i+1==N];
			}
		} else {
			// just output the sorted one
			for (int i = 0; i < N; i++) {
				cout << C[i]+1 << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
