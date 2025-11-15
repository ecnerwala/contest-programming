#include <bits/stdc++.h>

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	struct tree_t {
		int N, S;
		std::vector<std::vector<int>> adj;
		std::vector<bool> alive;
	};
	std::vector<tree_t> trees(T);
	std::set<std::pair<double, std::pair<int, int>>> cnds;
	auto compute_vals = [&](int t_id) -> void {
		tree_t& t = trees[t_id];
		std::pair<double, std::pair<int, int>> best = {0, {t_id, -1}};
		{
			std::vector<pairnum<double, double>> dp_up(t.N, {0, 0});
			std::vector<double> vals(t.N);
			// probability lose/down, time to leave
			for (int rt : t.adj[0]) {
				[&](this auto&& self, int cur, int prv) -> void {
					if (!t.alive[cur]) {
						dp_up[cur] = {1, 0};
						return;
					}
					int deg = int(t.adj[cur].size());
					pairnum<double, double> tot{0, 0};
					for (int nxt : t.adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur);
						tot += dp_up[nxt];
					}
					auto [tot_p, tot_t] = tot;
					dp_up[cur] = {
						tot_p / (tot_p + 1),
						(tot_t + deg) / (tot_p + 1)
					};
				}(rt, 0);
				[&](this auto&& self, int cur, int prv, double p_prv, double v_prv) -> void {
					if (!t.alive[cur]) {
						return;
					}
					{
						auto [tot_p, tot_t] = dp_up[cur];
						p_prv *= 1 - tot_p;
						v_prv += tot_t / p_prv;
						vals[cur] = v_prv;
					}
					for (int nxt : t.adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur, p_prv, v_prv);
					}
				}(rt, 0, 1, 0);
			}
			for (int i = 0; i < t.N; i++) {
				if (t.alive[i]) {
					if (best.second.second == -1 || vals[i] > best.first) {
						best = {vals[i], {t_id, i}};
					}
				}
			}
		}
		if (best.second.second == -1) return;
		// now, run the dp again to get transition probabilities
		{
			int rt = best.second.second;
			assert(t.alive[rt]);
			std::vector<pairnum<double, double>> dp_up(t.N, {0, 0});
			std::vector<pairnum<double, double>> dp_down(t.N, {0, 0});
			std::vector<double> vals(t.N);
			// probability lose/down, time to leave
			[&](this auto&& self, int cur, int prv) -> void {
				if (!t.alive[cur]) {
					dp_up[cur] = {1, 0};
					return;
				}
				int deg = int(t.adj[cur].size());
				pairnum<double, double> tot{0, 0};
				for (int nxt : t.adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					tot += dp_up[nxt];
				}
				auto [tot_p, tot_t] = tot;
				dp_up[cur] = {
					tot_p / (tot_p + (prv != -1)),
					(tot_t + deg) / (tot_p + (prv != -1))
				};
			}(rt, -1);
			double t_rt = dp_up[rt].v;
			// just a dummy thing
			dp_down[rt] = {0, 0};
			[&](this auto&& self, int cur, int prv) -> void {
				if (!t.alive[cur]) {
					return;
				}
				{
					int deg = int(t.adj[cur].size());
					std::vector<pairnum<double, double>> suff; suff.reserve(deg);
					for (int nxt : t.adj[cur]) {
						if (nxt == prv) continue;
						suff.push_back(dp_up[nxt]);
					}
					suff.push_back(dp_down[cur]);
					for (int z = int(suff.size()) - 1; z > 0; z--) {
						suff[z-1] += suff[z];
					}
					pairnum<double, double> pref{0, 0};
					int z = 0;
					for (int nxt : t.adj[cur]) {
						if (nxt == prv) continue;
						z++;
						auto tot = pref + suff[z];
						auto [tot_p, tot_t] = tot;
						dp_down[nxt] = {
							tot_p / (tot_p + 1),
							(tot_t + deg) / (tot_p + 1)
						};
					}
					assert(z == int(suff.size()) - 1);
				}
				for (int nxt : t.adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}
			}(rt, -1);
		}
		cnds.insert(best);
	};
	int tot_N = 0;
	for (int t_id = 0; t_id < T; t_id++) {
		tree_t& t = trees[t_id];
		cin >> t.N >> t.S; t.S--;
		tot_N += t.N;
		t.adj.resize(t.N);
		assert(t.S != 0);
		for (int e = 0; e < t.N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			t.adj[u].push_back(v);
			t.adj[v].push_back(u);
		}
		t.alive.assign(t.N, true);
		t.alive[0] = false;
		compute_vals(t_id);
	}

	std::vector<std::pair<int, int>> q; q.reserve(tot_N);
	while (!cnds.empty()) {
		auto iter = std::prev(cnds.end());
		auto it = *iter;
		cnds.erase(iter);
		auto [t_id, v] = it.second;
		tree_t& t = trees[t_id];
		t.alive[v] = false;
		cerr << it.first << ' ' << t_id << ' ' << v << '\n';
		compute_vals(t_id);
	}

	return 0;
}
