#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

struct polyline {
	ll yInt = 0;
	ll totSlope = 0;
	map<ll, ll> inflections;

	friend polyline& operator += (polyline& a, polyline&& b) {
		if (a.inflections.size() < b.inflections.size()) {
			swap(a,b);
		}
		a.yInt += b.yInt;
		a.totSlope += b.totSlope;
		for (const auto& it : b.inflections) {
			a.inflections[it.first] += it.second;
		}
		return a;
	}

	void update(ll d, ll c) {
		while (totSlope > c) {
			assert(!inflections.empty());
			auto it = inflections.begin();
			assert(it != inflections.end());
			ll diff = min(totSlope - c, it->second);
			totSlope -= diff;
			yInt -= diff * it->first;
			it->second -= diff;
			if (it->second == 0) {
				inflections.erase(it);
			}
		}
		assert(totSlope <= c);
		
		assert(d >= 0);
		while (!inflections.empty() && inflections.begin()->first < d) {
			auto it = inflections.begin();
			totSlope -= it->second;
			yInt -= it->second * it->first;
			inflections.erase(it);
		}

		if (totSlope < c) {
			ll diff = c - totSlope;
			inflections[d] += diff;
			totSlope += diff;
			yInt += d * diff;
		}
	}
};

class CoverTreePaths {

public:

	ll minimumCost(int n, vector <int> p, vector <int> d, vector <int> c, vector <int> params) {
		int t = int(p.size()) + 1;
		p.resize(n-1);
		for (int i = t-1; i <= n-2; i++) {
			p[i] = int((ll(params[0]) * p[i-1] + params[1]) % (i+1));
		}
		assert(int(d.size()) == t);
		d.resize(n);
		for (int i = t; i <= n-1; i++) {
			d[i] = int(1 + (ll(params[2]) * d[i-1] + params[3]) % params[4]);
		}
		assert(int(c.size()) == t);
		c.resize(n);
		for (int i = t; i <= n-1; i++) {
			c[i] = int(1 + (ll(params[5]) * c[i-1] + params[6]) % params[7]);
		}

		vector<polyline> dp(n);
		for (int i = n-1; i >= 0; i--) {
			dp[i].update(d[i], c[i]);
			if (i > 0) {
				dp[p[i-1]] += std::move(dp[i]);
			}
		}
		return dp[0].yInt;
	}
};

}  // namespace ecnerwala

using ecnerwala::CoverTreePaths;

// vim:ft=cpp
