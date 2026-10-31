#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct frac {
	ll p, q;
	frac() : p(0), q(1) {}
	frac(ll p_, ll q_) : p(p_), q(q_) {}
	friend bool operator == (const frac& a, const frac& b) { return a.p == b.p && a.q == b.q; }
	friend bool operator != (const frac& a, const frac& b) { return a.p != b.p || a.q != b.q; }
	friend bool operator < (const frac& a, const frac& b) { return a.p * b.q < b.p * a.q; }
	friend bool operator <= (const frac& a, const frac& b) { return a.p * b.q <= b.p * a.q; }
	friend bool operator > (const frac& a, const frac& b) { return a.p * b.q > b.p * a.q; }
	friend bool operator >= (const frac& a, const frac& b) { return a.p * b.q >= b.p * a.q; }
	friend std::ostream& operator << (std::ostream& o, const frac& f) { return o << f.p << '/' << f.q; }
};

template <typename F> pair<frac, frac> bsearch(F f, ll maxDen) {
	frac lo(0,1), hi(1,0);
	assert(!f(lo));
	// f(hi) is too hard to evaluate because inf
	while (true) {
		frac md(lo.p + hi.p, lo.q + hi.q);
		bool mdVal = f(md);
		frac o = mdVal ? lo : hi;

		int i = 0;
		while (md.q + (1ll << i) * o.q <= maxDen && f(frac(md.p + (1ll<<i) * o.p, md.q + (1ll<<i) * o.q)) == mdVal) i++;
		while (i > 0) {
			i--;
			if (md.q + (1 << i) * o.q <= maxDen && f(frac(md.p + (1ll<<i) * o.p, md.q + (1ll<<i) * o.q)) == mdVal) {
				md.p += (1ll<<i) * o.p;
				md.q += (1ll<<i) * o.q;
			}
		}

		if (mdVal) hi = md;
		else lo = md;

		if (md.q > maxDen) break;
	}
	return {lo, hi};
}

const int MAXP = 3.1e4;
ll V;
int P;
ll S[MAXP];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> V >> P;
	for (int i = 0; i < P; i++) {
		cin >> S[i];
	}

	frac lo = bsearch([](frac f) {
		ll numVoters = 0;
		for (int i = 0; i < P; i++) {
			numVoters += (f.p * (S[i] + 1)) / f.q;
		}
		return V <= numVoters;
	}, int(3e4) + 1).second;

	frac hi = bsearch([](frac f) {
		ll numVoters = 0;
		for (int i = 0; i < P; i++) {
			numVoters += (f.p * S[i] + f.q - 1) / f.q;
		}
		return V < numVoters;
	}, int(3e4) + 1).first;

	if (lo > hi) {
		cout << "impossible" << '\n';
		exit(0);
	}

	vector<pair<frac, ll>> loCnds;
	auto addLoCnd = [&](frac f) {
		if (f < lo || f > hi) return;
		ll numVoters = 0;
		for (int i = 0; i < P; i++) {
			numVoters += (f.p * (S[i] + 1)) / f.q;
		}
		loCnds.emplace_back(f, numVoters);
	};
	addLoCnd(lo);
	addLoCnd(hi);

	for (int z = 0; z < 2; z++) {
		auto vals = bsearch([z](frac f) {
			ll numVoters = 0;
			for (int i = 0; i < P; i++) {
				numVoters += (f.p * (S[i] + 1)) / f.q;
			}
			numVoters -= f.p / f.q;
			numVoters += z;
			return V < numVoters;
		}, int(3e4) + 1);
		addLoCnd(vals.first);
		addLoCnd(vals.second);
	}

	vector<pair<frac, ll>> hiCnds;
	auto addHiCnd = [&](frac f) {
		if (f < lo || f > hi) return;
		ll numVoters = 0;
		for (int i = 0; i < P; i++) {
			numVoters += (f.p * S[i] + f.q - 1) / f.q;
		}
		hiCnds.emplace_back(f, numVoters);
	};
	addHiCnd(lo);
	addHiCnd(hi);

	for (int z = 0; z < 2; z++) {
		auto vals = bsearch([z](frac f) {
			ll numVoters = 0;
			for (int i = 0; i < P; i++) {
				numVoters += (f.p * S[i] + f.q - 1) / f.q;
			}
			numVoters += f.p / f.q;
			numVoters -= z;
			return V < numVoters;
		}, int(3e4) + 1);
		addHiCnd(vals.first);
		addHiCnd(vals.second);
	}
	//cerr << "lo " << lo << '\n';
	//cerr << "hi " << hi << '\n';
	//for (auto it : loCnds) { cerr << "loCnd " << it.first << ' ' << it.second << '\n'; }
	//for (auto it : hiCnds) { cerr << "hiCnd " << it.first << ' ' << it.second << '\n'; }

	for (int i = 0; i < P; i++) {
		ll curLo = 1e18;
		ll curHi = 0;
		for (auto it : loCnds) {
			frac f = it.first;
			curLo = min(curLo, max(V - (it.second - (f.p * (S[i] + 1)) / f.q), (f.p * S[i] + f.q - 1) / f.q));
		}
		for (auto it : hiCnds) {
			frac f = it.first;
			curHi = max(curHi, min(V - (it.second - (f.p * S[i] + f.q - 1) / f.q), (f.p * (S[i] + 1)) / f.q));
		}
		//cerr << curLo << ' ' << curHi << '\n';
		assert(curLo <= curHi);
		cout << curLo << ' ' << curHi << '\n';
	}

	return 0;
}
