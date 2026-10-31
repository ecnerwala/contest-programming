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

	int N, M; cin >> N >> M;
	vector<vector<int>> outEdges(N);
	vector<vector<int>> inEdges(N);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		outEdges[a].push_back(b);
		inEdges[b].push_back(a);
	}

	vector<int> cc_id(N); // cc_ids are sorted topologically
	int num_ccs = 0;

	{ // Compute the sccs
		vector<bool> vis(N);
		vector<bool> inStack(N);
		int nextIndex = 0;
		vector<int> index(N);
		vector<int> lowval(N);
		vector<int> stk; stk.reserve(N);
		vector<int> toposort; toposort.reserve(N);
		for (int st = 0; st < N; st++) {
			if (vis[st]) continue;
			std::y_combinator([&](auto self, int cur) -> void {
				assert(!vis[cur]);
				index[cur] = nextIndex++;
				vis[cur] = true;
				inStack[cur] = true;
				stk.push_back(cur);
				lowval[cur] = index[cur];

				for (int nxt : inEdges[cur]) {
					if (!vis[nxt]) {
						self(nxt);
						lowval[cur] = min(lowval[cur], lowval[nxt]);
					} else if (inStack[nxt]) {
						lowval[cur] = min(lowval[cur], index[nxt]);
					}
				}

				if (lowval[cur] == index[cur]) {
					// Pop off this scc
					while (true) {
						int v = stk.back(); stk.pop_back();
						inStack[v] = false;
						cc_id[v] = num_ccs;
						if (v == cur) break;
					}
					num_ccs++;
				}
			})(st);
			assert(stk.empty());
		}
	}

	vector<int> succ(num_ccs, num_ccs);
	vector<int> pred(num_ccs, -1);

	for (int i = 0; i < N; i++) {
		for (int j : outEdges[i]) {
			int a = cc_id[i], b = cc_id[j];
			assert(a <= b);
			if (a == b) continue;
			succ[a] = min(succ[a], b);
			pred[b] = max(pred[b], a);
		}
	}

	vector<int> num_bad(num_ccs+1);
	for (int i = 0; i < num_ccs; i++) {
		num_bad[pred[i]+1]++;
		num_bad[i]--;
		num_bad[i+1]++;
		num_bad[succ[i]]--;
	}
	for (int i = 0; i < num_ccs; i++) {
		num_bad[i+1] += num_bad[i];
	}

	vector<int> ans; ans.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!num_bad[cc_id[i]]) {
			ans.push_back(i);
		}
	}
	cout << ans.size() << '\n';
	for (int k = 0; k < int(ans.size()); k++) {
		cout << ans[k]+1 << " \n"[k+1==int(ans.size())];
	}

	return 0;
}

// First, build the DAG of SCC's; each SCC behaves as a single vertex
// Then, consider a toposort; you must be reachable from *all* predecessors and
// be able to reach *all* successors, by toposort.
// You're bad iff there's some predecessor whose first successor is after you,
// or if there's some successor whose first predecessor is before you; these
// are just intervals.
//
// 1 2 x y z 3 4 5 6
//   ---------->
// ------------>
// ---------->
