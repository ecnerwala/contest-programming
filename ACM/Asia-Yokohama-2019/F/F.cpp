#include<bits/stdc++.h>
using namespace std;

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

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};
using num = modnum<int(1e9)+7>;
using ll = long long;

struct DSU {
	vector<int> par, rank;
	DSU() {}
	explicit DSU(int N) : par(N, -1), rank(N, 0) {}
	int getPar(int a) {
		return par[a] == -1 ? a : (par[a] = getPar(par[a]));
	}
	bool merge(int a, int b) {
		a = getPar(a), b = getPar(b);
		if (a == b) return false;
		if (rank[a] < rank[b]) swap(a,b);
		par[b] = a;
		if (rank[a] == rank[b]) rank[a] ++;
		return true;
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N; ll X0;
	cin >> N >> X0;

	struct node_t {
		ll A;
		ll B; int C;
		ll D; int E;

		bool isRoot;
		int root;
		int depth;
		int ind;

		vector<int> ch;

		int cycLen;
		ll cycOffset;
		unordered_map<ll, map<ll, int>> cycEnds;
	};

	vector<node_t> nodes(N+1);
	for (int i = 0; i < N; i++) {
		cin >> nodes[i].A >> nodes[i].B >> nodes[i].C >> nodes[i].D >> nodes[i].E;
		nodes[i].C --, nodes[i].E --;
		nodes[i].B += nodes[i].A;
	}
	nodes[N].A = nodes[N].B = nodes[N].D = 0;
	nodes[N].C = nodes[N].E = N;

	vector<int> roots;

	{ // Find roots
		DSU dsu(N+1);
		for (int i = 0; i < N; i++) {
			if (dsu.merge(i, nodes[i].E)) {
				nodes[nodes[i].E].ch.push_back(i);
				nodes[i].isRoot = false;
			} else {
				//cerr << "Found root " << i << '\n';
				roots.push_back(i);
				nodes[i].isRoot = true;
			}
		}

		roots.push_back(N);
	}

	unordered_map<ll, map<ll, int>> treeEnds;
	{
		int curInd = 0;
		for (int root : roots) {
			//cerr << "Starting root " << root << "\n";
			ll cycOffset = nodes[root].D;
			nodes[root].D = 0;
			nodes[root].depth = 0;
			y_combinator([&](auto self, int cur) -> void {
				nodes[cur].root = root;
				nodes[cur].ind = curInd;
				nodes[cur].A += nodes[cur].D;
				if (!treeEnds.count(nodes[cur].A)) {
					treeEnds[nodes[cur].A][0] = -1;
				}
				int oldVal = treeEnds[nodes[cur].A].rbegin()->second;
				assert(curInd >= treeEnds[nodes[cur].A].rbegin()->first);
				treeEnds[nodes[cur].A][curInd] = cur;

				curInd++;
				for (int nxt : nodes[cur].ch) {
					nodes[nxt].D += nodes[cur].D;
					nodes[nxt].depth = nodes[cur].depth + 1;
					self(nxt);
				}
				assert(curInd >= treeEnds[nodes[cur].A].rbegin()->first);
				treeEnds[nodes[cur].A][curInd] = oldVal;
			})(root);

			cycOffset += nodes[nodes[root].E].D;
			//cerr << "Root " << root << ' ' << cycOffset << '\n';
			unordered_map<ll, map<ll, int>> cycEnds;
			for (int cur = nodes[root].E; true; cur = nodes[cur].E) {
				ll v = nodes[cur].A;
				ll modV;
				if (cycOffset == 0) modV = v;
				else if (cycOffset > 0) {
					modV = (v % cycOffset + cycOffset) % cycOffset;
				} else {
					v = -v;
					modV = (v % -cycOffset + -cycOffset) % -cycOffset;
				}
				assert(cycOffset == 0 || (0 <= modV && modV < abs(cycOffset)));
				if (!cycEnds[modV].count(v)) {
					cycEnds[modV][v] = cur;
				}
				if (cur == root) break;
			}

			nodes[root].cycLen = nodes[nodes[root].E].depth + 1;
			nodes[root].cycOffset = cycOffset;
			nodes[root].cycEnds = cycEnds;
		}
		assert(curInd == N+1);
	}

	//cerr << "Finished preprocessing" << '\n';

	int curNode = 0;
	ll curX = X0 + nodes[curNode].D;

	num ans = 0;

	vector<bool> visA(N);
	while (curNode != N) {
		//cerr << ans << ' ' << curNode << ' ' << curX << '\n';
		if (curX == nodes[curNode].A) {
			if (visA[curNode]) {
				cout << -1 << '\n';
				exit(0);
			}
			visA[curNode] = true;
			curX = nodes[curNode].B;
			curNode = nodes[curNode].C;
			curX += nodes[curNode].D;
			ans++;
		} else if (nodes[curNode].isRoot) {
			int cycLen = nodes[curNode].cycLen;
			ll cycOffset = nodes[curNode].cycOffset;
			auto& cycEnds = nodes[curNode].cycEnds;

			// look up it's data
			if (cycOffset == 0) {
				if (!cycEnds.count(curX)) {
					cout << -1 << '\n';
					exit(0);
				}
				int nxt = cycEnds[curX][curX];
				ans += cycLen - nodes[nxt].depth;
				curNode = nxt;
			} else {
				ll mod = cycOffset;
				if (mod < 0) {
					curX = -curX;
					mod = -mod;
				}
				ll curMod = (curX % mod + mod) % mod;
				if (!cycEnds.count(curMod)) {
					cout << -1 << '\n';
					exit(0);
				}
				auto& modVals = cycEnds[curMod];
				auto it = modVals.upper_bound(curX);
				if (it == modVals.end()) {
					cout << -1 << '\n';
					exit(0);
				}
				int nxt = it->second;
				ans += num(cycLen) * ((it->first - curX) / mod) - nodes[nxt].depth;
				curNode = nxt;
			}
			curX = nodes[curNode].A;
		} else {
			//cerr << "Moving up to root" << '\n';
			int nxt = nodes[curNode].root;
			if (treeEnds.count(curX)) {
				auto it = treeEnds[curX].upper_bound(nodes[curNode].ind);
				--it;
				if (it->second != -1) {
					nxt = it->second;
					assert(curX == nodes[nxt].A);
					assert(nodes[nxt].root == nodes[curNode].root);
				}
			}
			ans += nodes[curNode].depth - nodes[nxt].depth;
			curNode = nxt;
		}
	}

	cout << ans << '\n';
	return 0;
}
