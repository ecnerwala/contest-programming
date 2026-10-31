#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;
ll f[200][200];
ll nim(ll x, ll y);
ll _nim(ll x, ll y) {
  if (!x || !y)  return 1ll << (x + y);
  if (f[x][y] != -1)  return f[x][y];
  ll ret = 1, e = 1;
  for (ll i = 0; (1ll<<i) <= max(x, x^y); i++)
    if ((x ^ y) & (1ll << i))  e <<= (1ll << i);
    else  if (x & (1ll << i))  ret = nim(ret, 3 * (1ll << ((1ll << i)-1)));
  f[x][y] = nim(ret, e);
  return f[x][y];
}
ll nim(ll x, ll y) {
  ll ret = 0;
  for (ll i = 0; (1ll << i) <= x; i++)
    if (x & (1ll << i))
      for (ll j = 0; (1ll << j) <= y; j++)
        if (y & (1ll << j))  ret ^= _nim(i, j);
  return ret;
}
class Rectoggle {
	int getNimber(int v, int m) {
		v++;
		int d = __builtin_ctz(v);
		while ((1 << d) > m) {
			d --;
		}
		return (1 << d);
	}

	vector<int> solveNimber(int numRows, int maxRows) {
		vector<int> res(numRows+1);
		for (int i = 0; i <= numRows; i++) {
			set<int> s;
			int v = 0;
			s.insert(v);
			for (int j = 1; j <= i && j < maxRows; j++) {
				v ^= res[i - j];
				s.insert(v);
			}
			res[i] = 0;
			while (s.count(res[i])) {
				res[i]++;
			}
		}
		return res;
	}

	void test() {
		cerr << nim((1 << 18), (1 << 18)) << '\n';
		for (int z = 1; z <= 1000; z++) {
			solveNimber(1000, z);
		}
		solveNimber(1000, 511);
		solveNimber(1000, 512);
		solveNimber(1000, 513);
	}

public:
	int whoWins(vector <int> ledrow, vector <int> ledcol, int maxrows, int maxcols) {
	  memset(f, -1, sizeof(f));
	  for (int i = 0; i < 18; i++) {
	    for (int j = 0; j < 18; j++) {
	      ll v = nim((1<<i),(1<<j));
	      cerr << i << ' ' << j << ' ' << v<<'\n';
	    }
	  }
	  cerr << nim(getNimber(3, 2), getNimber(0, 2)) << '\n';
	  cerr << nim(getNimber(0, 2), getNimber(0, 2)) << '\n';
		//test(); return 0;
		ll res = 0;
		for (size_t i = 0; i < ledrow.size(); i++) {
			res ^= nim(getNimber(ledrow[i], maxrows), getNimber(ledcol[i], maxcols));
		}
		return (res == 0) ? 2 : 1;
	}
};

}  // namespace ecnerwala

using ecnerwala::Rectoggle;

// vim:ft=cpp
