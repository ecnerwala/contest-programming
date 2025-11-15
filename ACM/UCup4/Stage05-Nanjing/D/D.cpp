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
	using dbl = long double;

	int T; cin >> T;
	struct tree_t {
		int N;
		std::vector<std::vector<int>> adj;
		std::vector<bool> alive;
		std::vector<dbl> probs;
	};
	std::vector<tree_t> trees(T);
	std::set<std::pair<dbl, std::pair<int, int>>> cnds;
	auto compute_vals = [&](int t_id) -> void {
		tree_t& t = trees[t_id];
		std::pair<dbl, std::pair<int, int>> best = {0, {t_id, -1}};
		std::vector<pairnum<dbl, dbl>> dp_up(t.N, {0, 0});
		// probability lose/down, time to leave
		for (int rt : t.adj[0]) {
			[&](this auto&& self, int cur, int prv) -> void {
				if (!t.alive[cur]) {
					dp_up[cur] = {1, 0};
					return;
				}
				int deg = int(t.adj[cur].size());
				pairnum<dbl, dbl> tot{0, 0};
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
			[&](this auto&& self, int cur, int prv, dbl p_prv, dbl v_prv) -> void {
				if (!t.alive[cur]) {
					return;
				}
				{
					auto [tot_p, tot_t] = dp_up[cur];
					p_prv *= 1 - tot_p;
					v_prv += tot_t / p_prv;
					if (best.second.second == -1 || v_prv > best.first) {
						best = {v_prv, {t_id, cur}};
					}
				}
				for (int nxt : t.adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur, p_prv, v_prv);
				}
			}(rt, 0, 1, 0);
		}
		if (best.second.second == -1) return;
		cnds.insert(best);
	};
	int tot_N = 0;
	for (int t_id = 0; t_id < T; t_id++) {
		tree_t& t = trees[t_id];
		cin >> t.N;
		int S; cin >> S; S--;
		tot_N += t.N;
		t.adj.resize(t.N);
		for (int e = 0; e < t.N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			t.adj[u].push_back(v);
			t.adj[v].push_back(u);
		}
		t.alive.assign(t.N, true);
		t.alive[0] = false;
		assert(S != 0);
		t.probs.assign(t.N, 0);
		t.probs[S] = 1;
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
		q.push_back({t_id, v});
		compute_vals(t_id);
	}
	std::reverse(q.begin(), q.end());
	std::vector<dbl> t_alive(T, 1.);
	dbl ans = 0;
	for (auto [t_id, rt] : q) {
		cerr << "reverse " << t_id << ' ' << rt << '\n';
		tree_t& t = trees[t_id];
		// now, run the dp again to get transition probabilities
		assert(!t.alive[rt]);
		t.alive[rt] = true;
		std::vector<pairnum<dbl, dbl>> dp_up(t.N, {0, 0});
		std::vector<pairnum<dbl, dbl>> dp_down(t.N, {0, 0});
		// probability lose/down, time to leave
		[&](this auto&& self, int cur, int prv) -> void {
			if (!t.alive[cur]) {
				dp_up[cur] = {1, 0};
				return;
			}
			int deg = int(t.adj[cur].size());
			pairnum<dbl, dbl> tot{0, 0};
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
		dbl p_rt = std::exchange(t.probs[rt], 0);
		{
			dbl t_rt = dp_up[rt].v;
			cerr << "t_rt " << t_rt << '\n';
			cerr << "p_rt " << p_rt << '\n';
			dbl val = t_rt * p_rt;
			for (int t_oth = 0; t_oth < T; t_oth++) {
				if (t_oth == t_id) continue;
				val *= t_alive[t_oth];
			}
			cerr << "val " << val << '\n';
			ans += val;
		}
		[&](this auto&& self, int cur, int prv, dbl p_here) -> void {
			if (!t.alive[cur]) {
				cerr << " next " << cur << ' ' << p_here << '\n';
				if (cur != 0) t.probs[cur] += p_here;
				return;
			}
			{
				int deg = int(t.adj[cur].size());
				std::vector<pairnum<dbl, dbl>> suff; suff.reserve(deg);
				for (int nxt : t.adj[cur]) {
					if (nxt == prv) continue;
					suff.push_back(dp_up[nxt]);
				}
				suff.push_back(prv == -1 ? pairnum<dbl, dbl>{0, 0} : dp_down[cur]);
				for (int z = int(suff.size()) - 1; z > 0; z--) {
					suff[z-1] += suff[z];
				}
				pairnum<dbl, dbl> pref{0, 0};
				int z = 0;
				for (int nxt : t.adj[cur]) {
					if (nxt == prv) continue;
					z++;
					auto tot = pref + suff[z];
					pref += dp_up[nxt];
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
				self(nxt, cur, p_here * (1 - dp_down[nxt].u));
			}
		}(rt, -1, p_rt);
		t_alive[t_id] = std::accumulate(t.probs.begin(), t.probs.end(), dbl(0));
	}
	cout << fixed << setprecision(20) << ans << '\n';

	return 0;
}
