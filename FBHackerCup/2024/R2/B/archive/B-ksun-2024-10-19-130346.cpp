#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()

struct FT {
	vector<ll> s;
	FT(int n) : s(n) {}
	void update(int pos, ll dif) { // a[pos] += dif
		for (; pos < sz(s); pos |= pos + 1) s[pos] += dif;
	}
	ll query(int pos) { // sum of values in [0, pos)
		ll res = 0;
		for (; pos > 0; pos &= pos - 1) res += s[pos-1];
		return res;
	}
	int lower_bound(ll sum) {// min pos st sum of [0, pos] >= sum
		// Returns n if no sum is >= sum, or -1 if empty sum is.
		if (sum <= 0) return -1;
		int pos = 0;
		for (int pw = 1 << 25; pw; pw >>= 1) {
			if (pos + pw <= sz(s) && s[pos + pw-1] < sum)
				pos += pw, sum -= s[pos-1];
		}
		return pos;
	}
};
void solve(int t){
	cout << "Case #" << t << ": ";
	int R, C;
	cin >> R >> C;
	ll K;
	cin >> K;
	vector<vector<int> > A(R, vector<int>(C));
	int G = R * C;
	vector<vector<pair<int,int>>> z(G);
	for(int i = 0; i < R; i++){
		for(int j = 0; j < C; j++){
			cin >> A[i][j];
			A[i][j]--;
			z[A[i][j]].push_back({i, j});
		}
	}
	// find the smallest score with >= K which is <= it
	ll s = 0;
	ll e = max(R, C);

	FT ft(C);
	while(s + 1 < e){
		// how many pairs where |r - r| <= D, |c - c| <= D
		ll m = (s + e) / 2;

		ll D = m;
		ll tcnt = 1;
		{
			ll rcnt = 0;
			for(int r = 0; r < R; r++){
				ll ar = max(r - D, ll(0));
				ll br = min(r + D, ll(R-1));
				rcnt += (br - ar + 1);
			}
			tcnt *= rcnt;
		}
		{
			ll ccnt = 0;
			for(int c = 0; c < C; c++){
				ll ac = max(c - D, ll(0));
				ll bc = min(c + D, ll(C-1));
				ccnt += (bc - ac + 1);
			}
			tcnt *= ccnt;
		}
		tcnt -= R * C;
		for(int color = 0; color < G; color++){
			auto& v = z[color];
			int li = 0;
			for(int i = 0; i < (int)v.size(); i++){
				auto [x, y] = v[i];
				// inc y
				while(li < (int)v.size() && v[li].first < x - D){
					ft.update(v[li].second, -1);
					li++;
				}
				int lcol = max(y - D, ll(0));
				int rcol = min(y + D, ll(C - 1));
				if(rcol >= lcol){
					tcnt -= 2 * (ft.query(rcol + 1) - ft.query(lcol));
				}
				ft.update(y, 1);
			}
			while(li < (int)v.size()){
				ft.update(v[li].second, -1);
				li++;
			}
		}
		if(tcnt >= K){
			e = m;
		} else {
			s = m;
		}
	}
	cout << e << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	for(int t = 1; t <= T; t++){
		solve(t);
		cout << flush;
	}
}
