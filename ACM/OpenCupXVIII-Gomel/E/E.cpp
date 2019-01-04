#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2e5;
const int MAXM = 2e5;
ll N, M;
ll K;

struct lingen {
	static const int MAXV = 1.1e5;
	ll mod;
	ll v1, a, b;
	friend std::istream& operator >> (std::istream& o, lingen& g) {
		return o >> g.v1 >> g.a >> g.b;
	}
	ll next(ll v) {
		assert(0 <= v && v < mod);
		return (v * a + b) % mod;
	}
	vector<ll> start;
	vector<ll> cycle;
	void run() {
		vector<int> vis(mod, -1);
		ll v = v1;
		while (true) {
			if (vis[v] != -1) {
				cycle.assign(start.begin() + vis[v], start.end());
				start.resize(vis[v]);
				break;
			}
			vis[v] = int(start.size());
			start.push_back(v);
			v = next(v);
		}
	}

	void expand_start(int sz) {
		assert(sz >= int(start.size()));
		int i = 0;

		while (sz > int(start.size())) {
			start.push_back(cycle[i]);
			i++, i %= int(cycle.size());
		}

		vector<ll> newCycle(cycle.begin() + i, cycle.end());
		newCycle.insert(newCycle.end(), cycle.begin(), cycle.begin() + i);
		cycle = std::move(newCycle);
	}
} ngen, mgen;

vector<ll> row_extras[MAXN];
bool processed[MAXN];

struct countSet {
	set<ll> st;
	ll sum;
	ll c2(ll a) const {
		return a * (a-1) / 2;
	}
	ll val(ll a, ll b) const {
		assert(a < b);
		return c2(b-a);
	}
	countSet() {
		ll len = M;
		st.insert(-1);
		st.insert(len);
		sum = c2(len - (-1));
	}
	void insert(ll v) {
		assert(-1 < v && v < *st.rbegin());
		auto jt = st.lower_bound(v);
		assert(jt != st.end());
		assert(jt != st.begin());
		assert(*jt != v);
		auto it = jt; --it;

		auto nt = st.insert(it, v);
		sum -= val(*it, *jt);
		sum += val(*it, *nt);
		sum += val(*nt, *jt);
	}
	void erase(ll v) {
		auto nt = st.find(v);
		assert(nt != st.end());
		assert(nt != st.begin());
		auto it = nt; --it;
		auto jt = nt; ++jt;
		assert(jt != st.end());

		sum -= val(*it, *nt);
		sum -= val(*nt, *jt);
		sum += val(*it, *jt);

		st.erase(nt);
	}
};

int main() {
	// the first thing we do is find the cycles mod N and mod M
	cin >> N >> M >> K;
	ngen.mod = N;
	mgen.mod = M;
	cin >> ngen >> mgen;
	ngen.run();
	mgen.run();

	ll start_len = ll(max(ngen.start.size(), mgen.start.size()));
	ngen.expand_start(int(start_len));
	mgen.expand_start(int(start_len));

	if (K < start_len) {
		start_len = K;
	}
	for (ll i = 0; i < start_len; i++) {
		row_extras[ngen.start[i]].push_back(mgen.start[i]);
	}
	K -= start_len;

	// just kill the last cycle for convenience
	while (K % int(ngen.cycle.size()) != 0) {
		K--;
		row_extras[ngen.cycle[K % int(ngen.cycle.size())]].push_back(mgen.cycle[K % int(mgen.cycle.size())]);
	}

	K /= int(ngen.cycle.size()); // number of people per row

	ll ans = 0;
	if (K > 0) {
		ll G = __gcd(ll(ngen.cycle.size()), ll(mgen.cycle.size()));
		for (ll g = 0; g < G; g++) {
			vector<ll> ncyc, mcyc;
			for (ll i = g; i < int(ngen.cycle.size()); i += G) {
				ncyc.push_back(ngen.cycle[i]);
			}
			for (ll i = g; i < int(mgen.cycle.size()); i += G) {
				mcyc.push_back(mgen.cycle[i]);
			}
			assert(__gcd(ncyc.size(), mcyc.size()) == 1);
			assert(K <= int(mcyc.size()));

			countSet s;
			for (ll i = 0; i < K; i++) {
				s.insert(mcyc[i * int(ncyc.size()) % int(mcyc.size())]);
			}
			for (ll z = 0; z < int(mcyc.size()); z++) {
				for(ll i = z * int(ncyc.size()) % int(mcyc.size()); i < int(ncyc.size()); i += int(mcyc.size())) {
					ll row = ncyc[i];
					for (ll v : row_extras[row]) {
						s.insert(v);
					}
					ans += s.sum;
					processed[row] = true;
					for (ll v : row_extras[row]) {
						s.erase(v);
					}
				}
				s.erase(mcyc[z * int(ncyc.size()) % int(mcyc.size())]);
				s.insert(mcyc[(K + z) * int(ncyc.size()) % int(mcyc.size())]);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (!processed[i]) {
			countSet s;
			for (ll v : row_extras[i]) {
				s.insert(v);
			}
			ans += s.sum;
			processed[i] = true;
		}
	}
	cout << ans << '\n';

	return 0;
}
