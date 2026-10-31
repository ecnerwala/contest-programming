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


namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, e, y, top = me;
	for (auto pa : ed[at]) if (pa.second != par) {
		tie(y, e) = pa;
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				f(vi(st.begin() + si, st.end()), at);
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else {
				/* e is a bridge */
				f(vi({e}), at);
			}
		}
	}
	return top;
}

template<class F, class NewCC, class FinishCC>
void bicomps(NewCC newcc, F f, FinishCC finishcc) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) {
		newcc(i);
		dfs(i, -1, f);
		finishcc(i);
	}
}

#undef rep
#undef all
#undef sz
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int E, V; cin >> E >> V;
	std::vector<std::array<int, 2>> edges(E);
	for (auto& [x, y] : edges) { cin >> x >> y; x--, y--; }

	int alpha = 0, beta = 0;

	kactl::ed.resize(V);
	for (int e = 0; e < E; ++e) {
		auto [x, y] = edges[e];
		kactl::ed[x].push_back({y, e});
		kactl::ed[y].push_back({x, e});
	}
	std::vector<int> cur_cnt(V);
	std::vector<int> subtree_sz(V);
	std::vector<bool> allowed(E, true);

	std::vector<int> deg(V);
	std::vector<int> pos(V);
	std::vector<int> cur_cc; cur_cc.reserve(V);
	std::vector<int> cur_bcc; cur_bcc.reserve(V);

	std::vector<std::vector<int>> adj_bigger(V);
	std::vector<char> mark(V);

	std::vector<int> num_remove(V);

	kactl::bicomps([&](int cc_root) -> void {
		cur_cc.clear();
	}, [&](const std::vector<int>& edgelist, int root) {
		int cur_sz = int(edgelist.size());
		cur_bcc.clear();
		for (int e : edgelist) {
			for (int v : edges[e]) {
				if ((deg[v]++) == 0) {
					cur_cc.push_back(v);
				}
				if ((cur_cnt[v]++) == 0) {
					cur_bcc.push_back(v);
					if (v != root) cur_sz += subtree_sz[v];
				}
			}
		}

		for (int v : cur_bcc) {
			if (v == root ? ((cur_sz + cur_cnt[v]) % 2 == 1) : ((subtree_sz[v] + cur_cnt[v]) % 2 == 0)) {
				num_remove[v] += cur_cnt[v] - 1;
			} else {
				num_remove[v] += cur_cnt[v];
			}
		}

		for (int e : edgelist) {
			for (int v : edges[e]) {
				if (cur_cnt[v] == 1) assert(edgelist.size() == 1);
				if (cur_cnt[v] == 2 && (v == root ? (cur_sz % 2 == 0) : (subtree_sz[v] % 2 == 1))) {
					// this guy is bad
					allowed[e] = false;
				}
			}
		}

		subtree_sz[root] += cur_sz;

		for (int e : edgelist) {
			for (int v : edges[e]) {
				cur_cnt[v]--;
			}
		}
	}, [&](int cc_root) -> void {
		//cerr << "cc_root: " << cc_root << endl;
		//cerr << "subtree_sz: " << subtree_sz[cc_root] << endl;
		if (subtree_sz[cc_root] == 0) {
			return;
		}
		if (subtree_sz[cc_root] % 2 == 0) {
			return;
		}
		auto has_triangle = [&]() -> bool {
			// now, beta++ unless we can find a triangle
			std::sort(cur_cc.begin(), cur_cc.end(), [&](auto a, auto b) { return deg[a] < deg[b]; });
			for (int i = 0; i < int(cur_cc.size()); i++) {
				pos[cur_cc[i]] = i;
			}

			for (int i = 0; i < int(cur_cc.size()); i++) {
				adj_bigger[i].clear();
				for (auto [y, e] : kactl::ed[cur_cc[i]]) {
					if (allowed[e]) {
						int j = pos[y];
						if (j > i) adj_bigger[i].push_back(j);
					}
				}
			}
			for (int i = 0; i < int(cur_cc.size()); i++) {
				for (int j : adj_bigger[i]) mark[j] = true;
				bool found = false;
				for (int j : adj_bigger[i]) {
					for (int k : adj_bigger[j]) {
						if (mark[k]) {
							found = true;
							goto done;
						}
					}
				}
done:
				for (int j : adj_bigger[i]) mark[j] = false;
				if (found) return true;
			}
			return false;
		};
		if (has_triangle()) {
			return;
		}
		auto has_claw = [&]() -> bool {
			for (int v : cur_cc) {
				if (num_remove[v] >= 3) {
					return true;
				}
			}
			return false;
		};
		if (has_claw()) {
			beta++;
		} else {
			alpha++;
		}
	});

	cout << alpha << ' ' << beta << '\n';

	return 0;
}
