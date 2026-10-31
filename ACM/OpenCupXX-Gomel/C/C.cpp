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
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct SuffixArray {
	vi sa, lcp;
	SuffixArray(const string& s, int lim=256) { // or basic_string<int>
		int n = sz(s) + 1, k = 0, a, b;
		vi x(all(s)+1), y(n), ws(max(n, lim)), rank(n);
		sa = lcp = y, iota(all(sa), 0);
		for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
			p = j, iota(all(y), n - j);
			rep(i,0,n) if (sa[i] >= j) y[p++] = sa[i] - j;
			fill(all(ws), 0);
			rep(i,0,n) ws[x[i]]++;
			rep(i,1,lim) ws[i] += ws[i - 1];
			for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			rep(i,1,n) a = sa[i - 1], b = sa[i], x[b] =
				(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
		}
		rep(i,1,n) rank[sa[i]] = i;
		for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
			for (k && k--, j = sa[rank[i] - 1];
					s[i + k] == s[j + k]; k++);
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		string A, B; cin >> A >> B;
		int LA = sz(A);
		int LB = sz(B);
		int L = LA+LB;
		SuffixArray SA(A+B);

		//for (int i = 0; i <= L; i++) { cerr << SA.sa[i] << ' '; } cerr << '\n';
		//for (int i = 0; i <= L; i++) { cerr << SA.lcp[i] << ' '; } cerr << '\n';

		int S = 1;
		while (S < L+1) S *= 2;
		assert(S >= L+1);
		vector<pair<int, int>> lcpSeg(2*S);
		vector<int> stSeg(2*S);

		for (int i = 0; i < L+1; i++) lcpSeg[S+i] = {SA.lcp[i], i};
		for (int i = S-1; i; i--) lcpSeg[i] = min(lcpSeg[2*i], lcpSeg[2*i+1]);

		for (int i = 0; i < L+1; i++) stSeg[S+i] = (SA.sa[i] < LA ? SA.sa[i] : -1);
		for (int i = S-1; i; i--) stSeg[i] = max(stSeg[2*i], stSeg[2*i+1]);

		ll ans = 0;

		// inclusive range
		std::y_combinator([&] (auto self, int l, int r, int len) -> void {
			//cerr << l << ' ' << r << ' ' << len << '\n';
			if (l > r) return;

			int bestSt = -1;
			for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
				if (a & 1) bestSt = max(bestSt, stSeg[a++]);
				if (b & 1) bestSt = max(bestSt, stSeg[--b]);
			}

			if (bestSt == -1) return;

			int nxtLen;
			int mid;
			if (l == r) {
				nxtLen = L - SA.sa[l];
				mid = -1;
			} else {
				pair<int, int> bestLcp = {L+1, -1};
				for (int a = S+l+1, b = S+r+1; a < b; a /= 2, b /= 2) {
					if (a & 1) bestLcp = min(bestLcp, lcpSeg[a++]);
					if (b & 1) bestLcp = min(bestLcp, lcpSeg[--b]);
				}
				nxtLen = bestLcp.first;
				mid = bestLcp.second;
				assert(mid != -1);
			}
			//cerr << nxtLen << '\n';

			assert(nxtLen >= len);
			int lo = max(len+1, LA + 1 - bestSt);
			assert(lo >= 2);
			if (nxtLen >= lo) {
				ans += nxtLen - lo + 1;
			}

			if (mid != -1) {
				self(l, mid-1, nxtLen);
				self(mid, r, nxtLen);
			}
		})(0, L, 0);
		cout << ans << '\n';
	}

	return 0;
}
