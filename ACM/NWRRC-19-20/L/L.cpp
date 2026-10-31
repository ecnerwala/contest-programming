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

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct SuffixTree {
	enum { N = 400010, ALPHA = 28 }; // N ~ 2*maxlen+10
	int toi(char c) { return c - 'a'; }
	string a; // v = cur node, q = cur position
	int t[N][ALPHA],l[N],r[N],p[N],s[N],v=0,q=0,m=2;

	void ukkadd(int i, int c) { suff:
		if (r[v]<=q) {
			if (t[v][c]==-1) { t[v][c]=m;  l[m]=i;
				p[m++]=v; v=s[v]; q=r[v];  goto suff; }
			v=t[v][c]; q=l[v];
		}
		if (q==-1 || c==toi(a[q])) q++; else {
			l[m+1]=i;  p[m+1]=m;  l[m]=l[v];  r[m]=q;
			p[m]=p[v];  t[m][c]=m+1;  t[m][toi(a[q])]=v;
			l[v]=q;  p[v]=m;  t[p[m]][toi(a[l[m]])]=m;
			v=s[p[m]];  q=l[m];
			while (q<r[m]) { v=t[v][toi(a[q])];  q+=r[v]-l[v]; }
			if (q==r[m])  s[m]=v;  else s[m]=m+2;
			q=r[v]-(q-r[m]);  m+=2;  goto suff;
		}
	}

	SuffixTree(string a) : a(a) {
		fill(r,r+N,sz(a));
		memset(s, 0, sizeof s);
		memset(t, -1, sizeof t);
		fill(t[1],t[1]+ALPHA,0);
		s[0] = 1; l[0] = l[1] = -1; r[0] = r[1] = p[0] = p[1] = 0;
		rep(i,0,sz(a)) ukkadd(i, toi(a[i]));
	}

	// example: find longest common substring (uses ALPHA = 28)
	pii best;
	int lcs(int node, int i1, int i2, int olen) {
		if (l[node] <= i1 && i1 < r[node]) return 1;
		if (l[node] <= i2 && i2 < r[node]) return 2;
		int mask = 0, len = node ? olen + (r[node] - l[node]) : 0;
		rep(c,0,ALPHA) if (t[node][c] != -1)
			mask |= lcs(t[node][c], i1, i2, len);
		if (mask == 3)
			best = max(best, {len, r[node] - len});
		return mask;
	}
	static pii LCS(string s, string t) {
		SuffixTree st(s + (char)('z' + 1) + t + (char)('z' + 2));
		st.lcs(0, sz(s), sz(s) + 1 + sz(t), 0);
		return st.best;
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	SuffixTree st(S + char('z'+1));
	pair<ll, ll> best(0,1);
	// lcp / delta
	auto checkPair = [&](int lcp, int delta) {
		if (lcp * best.second >= best.first * delta) {
			best = {lcp, delta};
		}
	};
	std::y_combinator([&] (auto self, int n, int depth) -> set<int> {
		set<int> s;
		for (int i = 0; i <= 26; i++) {
			int c = st.t[n][i];
			if (c == -1) continue;
			set<int> t = self(c, depth + (st.r[c] - st.l[c]));
			if (t.size() > s.size()) {
				swap(s, t);
			}
			for (int a : t) {
				auto it = s.lower_bound(a);
				if (it != s.end()) {
					checkPair(depth, *it - a);
				}
				if (it != s.begin()) {
					--it;
					checkPair(depth, a - *it);
				}
			}
		}

		if (s.empty()) {
			s.insert(depth);
		}

		return s;
	})(0, 0);

	ll g = __gcd(best.first, best.second);
	best.first /= g, best.second /= g;
	best.first += best.second;
	cout << best.first << '/' << best.second << '\n';

	return 0;
}
