#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

ll INF = 1e17;
ll dist(pair<ll, ll> a, pair<ll, ll> b){
	return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

#define sz(x) (int)(x).size()

struct FT {
	vector<ll> s;
	FT(int n) : s(n, INF) {}
	void update(int pos, ll dif) { // a[pos] += dif
		for (; pos < sz(s); pos |= pos + 1) s[pos] = min(s[pos], dif);
	}
	ll query(int pos) { // sum of values in [0, pos)
		ll res = INF;
		for (; pos > 0; pos &= pos - 1) res = min(res, s[pos-1]);
		return res;
	}
};

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q;
	cin >> N >> Q;
	vector<pair<ll, ll> > pts(N);
	for(auto& [x, y] : pts) cin >> x >> y;
	vector<vector<int> > prev_cand(N);
	vector<vector<int> > cands(N);
	int MAX_BUCKET = 9;
	for(int LVL = 0; LVL < 29; LVL++){
		ll d_thresh = ll(1 << LVL) * ll(1 << LVL);
		map<pair<ll, ll>, vector<int> > buckets;
		for(int i = N-1; i >= 0; i--){
			pair<ll, ll> loc = {pts[i].first >> LVL, pts[i].second >> LVL};
			for(ll a = loc.first - 1; a <= loc.first + 1; a++){
				for(ll b = loc.second - 1; b <= loc.second + 1; b++){
					for(int r : buckets[{a, b}]){
						if(dist(pts[r], pts[i]) <= d_thresh){
							cands[i].push_back(r);
						}
					}
				}
			}
			buckets[loc].push_back(i);
			if((int)buckets[loc].size() > MAX_BUCKET){
				buckets[loc].erase(buckets[loc].begin());
			}
		}
	}
	for(int i = 0; i < N; i++){
		vector<int> c = cands[i];
		sort(c.begin(), c.end());
		c.resize(unique(c.begin(), c.end()) - c.begin());
		cands[i] = {};
		ll pbest = INF;
		for(int j : c){
			ll d = dist(pts[i], pts[j]);
			if(d < pbest){
				pbest = d;
				cands[i].push_back(j);
			}
		}
	}

	vector<pair<int,int> > queries(Q);
	for(auto& [l, r] : queries){
		cin >> l >> r;
		l--; r--;
	}
	vector<vector<int> > qloc(N);
	for(int q = 0; q < Q; q++){
		qloc[queries[q].first].push_back(q);
	}
	vector<ll> qans(Q);
	FT ft(N+1);
	for(int i = N-1; i >= 0; i--){
		for(int j : cands[i]){
			ft.update(j, dist(pts[i], pts[j]));
		}
		for(int q : qloc[i]){
			qans[q] = ft.query(queries[q].second + 1);
		}
	}
	for(ll x : qans){
		cout << x << '\n';
	}
}