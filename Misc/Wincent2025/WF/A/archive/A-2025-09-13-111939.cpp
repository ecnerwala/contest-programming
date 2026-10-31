#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct RollbackUF {
	vi e; vector<pii> st;
	RollbackUF(int n) : e(n, -1) {}
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : find(e[x]); }
	int time() { return sz(st); }
	void rollback(int t) {
		for (int i = time(); i --> t;)
			e[st[i].first] = st[i].second;
		st.resize(t);
	}
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.push_back({a, e[a]});
		st.push_back({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};

struct Edge { int a, b; ll w = 0; };
struct Node { /// lazy skew heap node
	Edge key;
	Node *l = nullptr, *r = nullptr;
	ll delta = 0;
	void prop() {
		key.w += delta;
		if (l) l->delta += delta;
		if (r) r->delta += delta;
		delta = 0;
	}
	Edge top() { prop(); return key; }
};
Node *merge(Node *a, Node *b) {
	if (!a || !b) return a ? a : b;
	a->prop(), b->prop();
	if (a->key.w > b->key.w) swap(a, b);
	swap(a->l, (a->r = merge(b, a->r)));
	return a;
}
void pop(Node*& a) { a->prop(); a = merge(a->l, a->r); }

pair<ll, vi> dmst(int n, int r, vector<Edge>& g) {
	RollbackUF uf(n);
	vector<Node*> heap(n);
	for (Edge e : g) heap[e.b] = merge(heap[e.b], new Node{e});
	ll res = 0;
	vi seen(n, -1), path(n), par(n);
	seen[r] = r;
	vector<Edge> Q(n), in(n, {-1,-1}), comp;
	deque<tuple<int, int, vector<Edge>>> cycs;
	rep(s,0,n) {
		int u = s, qi = 0, w;
		while (seen[u] < 0) {
			if (!heap[u]) return {-1,{}};
			Edge e = heap[u]->top();
			heap[u]->delta -= e.w, pop(heap[u]);
			Q[qi] = e, path[qi++] = u, seen[u] = s;
			res += e.w, u = uf.find(e.a);
			if (seen[u] == s) { /// found cycle, contract
				Node* cyc = 0;
				int end = qi, time = uf.time();
				do cyc = merge(cyc, heap[w = path[--qi]]);
				while (uf.join(u, w));
				u = uf.find(u), heap[u] = cyc, seen[u] = -1;
				cycs.push_front({u, time, {&Q[qi], &Q[end]}});
			}
		}
		rep(i,0,qi) in[uf.find(Q[i].b)] = Q[i];
	}

	for (auto& [u,t,comp_edges] : cycs) { // restore sol (optional)
		uf.rollback(t);
		Edge inEdge = in[u];
		for (auto& e : comp_edges) in[uf.find(e.b)] = e;
		in[uf.find(inEdge.b)] = inEdge;
	}
	rep(i,0,n) par[i] = in[i].a;
	return {res, par};
}
}

const std::array<char, 4> dc = {'^', '<', '>', 'v'};
const std::array<int, 4> dx = {-1, 0, 0, +1};
const std::array<int, 4> dy = {0, -1, +1, 0};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::string> S(N);
		for (int i = 0; i < N; i++) {
			cin >> S[i];
			assert(int(S[i].size()) == M);
		}
		std::vector<std::vector<bool>> can_move(N, std::vector<bool>(M));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				char v; cin >> v;
				assert(v == '0' || v == '1');
				can_move[i][j] = v == '1';
			}
		}

		int V = N*M + 1;
		std::vector<kactl::Edge> g; g.reserve(N*M*4);
		std::vector<std::pair<std::array<int, 2>, char>> e_vals; e_vals.reserve(N*M*4);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				for (int d = 0; d < 4; d++) {
					int ni = i + dx[d], nj = j + dy[d];
					int st = i * M + j;
					int en;
					if (0 <= ni && ni < N && 0 <= nj && nj < M) {
						en = ni * M + nj;
					} else {
						en = N * M;
					}

					if (S[i][j] == dc[d]) {
						cerr << "add " << st << ' ' << en << ' ' << 0 << '\n';
						g.push_back({en, st, 0});
						e_vals.push_back({{i, j}, dc[d]});
					} else if (can_move[i][j]) {
						cerr << "add " << st << ' ' << en << ' ' << 1 << '\n';
						g.push_back({en, st, 1});
						e_vals.push_back({{i, j}, dc[d]});
					}
				}
			}
		}
		auto [cost, par] = kactl::dmst(V, N*M, g);
		assert(cost != -1);
		assert(int(par.size()) == V);
		std::vector<std::string> O(N, std::string(M, '?'));
		int tot_cost = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::pair<int, char> val{2, '?'};
				for (int d = 0; d < 4; d++) {
					int ni = i + dx[d], nj = j + dy[d];
					int st = i * M + j;
					int en;
					if (0 <= ni && ni < N && 0 <= nj && nj < M) {
						en = ni * M + nj;
					} else {
						en = N * M;
					}

					if (en != par[st]) continue;

					if (S[i][j] == dc[d]) {
						val = std::min(val, {0, dc[d]});
					} else if (can_move[i][j]) {
						val = std::min(val, {1, dc[d]});
					}
				}
				assert(val.first <= 1);
				tot_cost += val.first;
				O[i][j] = val.second;
			}
		}
		assert(tot_cost == cost);
		cout << cost << '\n';
		for (auto r : O) cout << r << '\n';
	}

	return 0;
}
