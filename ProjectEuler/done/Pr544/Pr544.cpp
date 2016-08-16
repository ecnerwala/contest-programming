#include<bits/stdc++.h>
#include<boost/functional/hash.hpp>
using namespace std;

typedef long long ll;

template <typename Container> // we can make this generic for any container [1]
struct container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};

typedef vector<int> vi;

const ll MOD = 1e9 + 7;

ll inv(ll a, ll m) {
	return (a == 1) ? 1 : (m - inv(m % a, a) * m / a);
}

void make_canonical(vector<int> &v) {
	unordered_map<int, int> vals;
	for(int &i : v) {
		if(i == -1) continue;
		if(vals.count(i)) {
			i = vals[i];
		} else {
			int k = int(vals.size());
			vals[i] = k;
			i = k;
		}
	}
	v.push_back(int(vals.size()));
}

ll S(ll R, ll C, ll N) {
	if(R > C) swap(R, C);
	assert(R <= C);

	// DP state is stored as:
	// v[0..R-1] is the color of the rightmost visited square in the rth row ((r,c) or (r,c-1))
	// v[R] = total color used among v[0..R-1] (for bookkeeping, it's easy to recalculate)
	// v[R+1] = total number of colors used
	// The value is when colors are _unordered_ (it's a factor of tot_colors! off)
	unordered_map<vi, ll, container_hash<vi>> mp, nmp;

	mp.clear();

	vector<int> start;
	for(int r = 0; r < R; r++) {
		start.push_back(-1);
	}
	start.push_back(0);
	start.push_back(0);
	mp[start] = 1;

	for(int c = 0; c < C; c++) {
		for(int r = 0; r < R; r++) {
			nmp.clear();
			for(const auto &it : mp) {
				vector<int> v = it.first;
				ll ways = it.second % MOD;
				int tot_items = v.back(); v.pop_back();
				int used_items = v.back(); v.pop_back();
				assert(used_items <= tot_items);

				assert(int(v.size()) == R);

				/*
				cerr << "row " << r << ' ' << "col " << c << '\n';
				for(ll i = 0; i < R; i++) cerr << v[i] << ' '; cerr << '\n';
				cerr << "tot items " << tot_items << '\n';
				cerr << "used items " << used_items << '\n';
				cerr << "ways " << ways << '\n';
				cerr << '\n';
				*/

				vector<int> nv;

				// Case 1: use something entirely new
				nv = v;
				nv[r] = tot_items;
				make_canonical(nv);
				nv.push_back(tot_items + 1);
				nmp[nv] = (nmp[nv] + ways) % MOD;

				// Case 2: use something not used
				if(used_items < tot_items) {
					nv = v;
					nv[r] = used_items;
					make_canonical(nv);
					nv.push_back(tot_items);
					nmp[nv] = (nmp[nv] + ways * (tot_items - used_items)) % MOD;
				}

				// Case 3: use something already used
				for(int i = 0; i < used_items; i++) {
					nv = v;
					if(nv[r] == i) continue;
					if(r && nv[r - 1] == i) continue;
					nv[r] = i;
					make_canonical(nv);
					nv.push_back(tot_items);
					nmp[nv] = (nmp[nv] + ways) % MOD;
				}
			}
			mp = nmp;
			cerr << c << ' ' << r << ' ' << mp.size() << '\n';
		}
	}

	vector<ll> tot_ways(R * C + 1);

	for(const auto &it : mp) {
		int tot_items = it.first.back();
		assert(tot_items > 0);
		assert(tot_items <= R * C);
		ll ways = it.second % MOD;
		tot_ways[tot_items] += ways;
		tot_ways[tot_items] %= MOD;
	}

	ll res = 0;

	// sum of (0 choose i) + (1 choose i) + ... + (N choose i) = (N + 1 choose i + 1)
	// sum of i! (0 choose i) + i! (1 choose i) + ... + i! (N choose i) = i! (N + 1 choose i + 1)
	for(int items = 1; items <= R * C && items <= N; items++) {
		ll ways = tot_ways[items];
		//cerr << items << ' ' << ways << '\n';
		for(ll i = 0; i < items + 1; i++) {
			ways *= N + 1 - i;
			ways %= MOD;
		}
		ways *= inv(items + 1, MOD);
		ways %= MOD;
		res += ways;
		res %= MOD;
	}

	return res;
}

ll F(ll r, ll c, ll n) {
	return S(r, c, n) - S(r, c, n - 1);
}

int main() {
	assert(F(2, 2, 3) == 18);
	assert(F(2, 2, 20) == 130340);
	assert(F(3, 4, 6) == 102923670);
	assert(S(4, 4, 15) == 325951319);
	cout << S(9, 10, 1112131415) << '\n';
	return 0;
}
