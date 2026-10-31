#include<bits/stdc++.h>
using namespace std;

using state = vector<pair<int, int>>;

ostream& operator << (ostream& o, const state& s) {
	o << "state[";
	for (auto e : s) o << "(" << e.first << ", " << e.second << ")" << ' ';
	o << "]";
	return o;
}

bool isGood(const state& s) {
	//cerr << "isGood " << s << '\n';
	unordered_map<int, int> deg;
	unordered_map<int, int> par;
	auto getPar = [&](int a) {
		while (par.count(a)) a = par[a];
		return a;
	};
	for (auto e : s) {
		if (++deg[e.first] > 2) {
			return false;
		}
		if (++deg[e.second] > 2) {
			return false;
		}
		int u = getPar(e.first), v = getPar(e.second);
		if (u > v) swap(u, v);
		assert(u <= v);
		if (u == v && u != -1) {
			return false;
		}
		if (u != v) {
			par[v] = u;
		}
	}

	//cerr << "Good!\n";
	return true;
}

state merge(const state& a, const state& b) {
	state res = a;
	res.insert(res.end(), b.begin(), b.end());
	sort(res.begin(), res.end());
	return res;
}

state edgeState(int a, int b) {
	if (a > b) swap(a,b);
	assert(a <= b);
	return state({{a,b}});
}

state removeVert(const state& s, int v) {
	//cerr << "removeVert " << s << ' ' << v << '\n';
	assert(v != -1);
	//assert(isGood(s));
	state ns;
	vector<int> adj;
	for (auto e : s) {
		if (e.first == v) {
			adj.emplace_back(e.second);
		} else if (e.second == v) {
			adj.emplace_back(e.first);
		} else {
			ns.push_back(e);
		}
	}
	assert(adj.size() <= 2);
	if (adj.empty()) {
		return ns;
	} else if (adj.size() == 1) {
		return merge(edgeState(-1, adj[0]), ns);
	} else if (adj.size() == 2) {
		return merge(edgeState(adj[0], adj[1]), ns);
	} else assert(false);
}

using states = map<state, int>;
ostream& operator << (ostream& o, const states& s) {
	o << "states{" << '\n';
	for (const auto& it : s) {
		o << "  " << it.first << ": " << it.second << '\n';
	}
	o << "}";
	return o;
}

states merge(states a, states b) {
	//cerr << "merge" << '\n'; cerr << a << '\n'; cerr << b << '\n';
	states res = a; // just a

	// just b
	for (const auto& s2 : b) {
		if (!res.count(s2.first) || s2.second > res[s2.first]) res[s2.first] = s2.second;
	}

	for (const auto& s1 : a) {
		for (const auto& s2 : b) {
			auto s = merge(s1.first, s2.first);
			if (isGood(s)) {
				int v = s1.second + s2.second;
				if (!res.count(s) || v > res[s]) res[s] = v;
			}
		}
	}
	//cerr << res << '\n';
	return res;
}

states removeVert(states a, int vert) {
	//cerr << "removeVert " << vert << '\n'; cerr << a << '\n';
	states res;
	for (const auto& s1 : a) {
		auto s = removeVert(s1.first, vert);
		if (isGood(s)) {
			if (!res.count(s) || s1.second > res[s]) res[s] = s1.second;
		}
	}
	//cerr << res << '\n';
	return res;
}

using tri = pair<pair<int, int>, int>;
tri canon(int a, int b, int c) {
	if (a > b) swap(a,b);
	if (b > c) swap(b,c);
	if (a > b) swap(a,b);
	return tri{{a,b},c};
}

map<tri, states> dp;

const int MAXN = 600;
int N;
vector<pair<int, int>> adj[MAXN];
int deg[MAXN];
bool alive[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;

	for (int e = 0; e < 3 * (N - 2); e++) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		adj[a].emplace_back(b, c);
		adj[b].emplace_back(a, c);
		deg[a] ++, deg[b] ++;
	}

	for (int i = 0; i < N; i++) {
		alive[i] = true;
	}

	stack<int> q;
	for (int i = 0; i < N; i++) {
		if (deg[i] <= 3) q.push(i);
	}

	for (int cyc = 0; cyc < N-3; cyc++) {
		int cur = q.top(); q.pop();
		{
			vector<pair<int, int>> nadj;
			for (auto e : adj[cur]) {
				if (!alive[e.first]) continue;
				nadj.push_back(e);
			}
			assert(nadj.size() == 3);
			adj[cur] = nadj;
		}
		//cerr << "triangle " << cur << ' ' << adj[cur][0].first << ' ' << adj[cur][1].first << ' ' << adj[cur][2].first << '\n';

		tri x = canon(cur, adj[cur][0].first, adj[cur][1].first);
		tri y = canon(cur, adj[cur][1].first, adj[cur][2].first);
		tri z = canon(cur, adj[cur][2].first, adj[cur][0].first);

		tri t = canon(adj[cur][0].first, adj[cur][1].first, adj[cur][2].first);

		states ns;
		ns = merge(ns, dp[x]);
		ns = merge(ns, dp[y]);
		ns = merge(ns, dp[z]);
		ns = merge(ns, states({{edgeState(cur, adj[cur][0].first), adj[cur][0].second}}));
		ns = merge(ns, states({{edgeState(cur, adj[cur][1].first), adj[cur][1].second}}));
		ns = merge(ns, states({{edgeState(cur, adj[cur][2].first), adj[cur][2].second}}));

		ns = removeVert(ns, cur);
		dp[t] = merge(ns, dp[t]);

		alive[cur] = false;
		for (int i = 0; i < 3; i++) {
			if (--deg[adj[cur][i].first] == 3) {
				q.push(adj[cur][i].first);
			}
		}
	}

	assert(q.size() == 3);
	int a = q.top(); q.pop();
	int b = q.top(); q.pop();
	int c = q.top(); q.pop();
	//cerr << "last triangle " << a << ' ' << b << ' ' << c << '\n';

	states ns = dp[canon(a,b,c)];

	for (auto e : adj[a]) {
		if (e.first == b) {
			ns = merge(ns, states({{edgeState(a, b), e.second}}));
		}
		if (e.first == c) {
			ns = merge(ns, states({{edgeState(a, c), e.second}}));
		}
	}
	for (auto e : adj[b]) {
		if (e.first == c) {
			ns = merge(ns, states({{edgeState(b, c), e.second}}));
		}
	}

	ns = removeVert(ns, a);
	ns = removeVert(ns, b);
	ns = removeVert(ns, c);

	cout << ns[state({{-1, -1}})] << '\n';

	return 0;
}
