#include <bits/stdc++.h>
#include <cassert>
using namespace std;

template <typename T> void set_min(T& a, const T& b){
	if(b < a) a = b;
}
template <typename T> void set_max(T& a, const T& b){
	if(b > a) a = b;
}

using ll = int64_t;

using i128 = __int128_t;

using pt = pair<ll, ll>;
bool cmp(pt a, pt b, ll x){
	assert(a.first <= x && b.first <= x);
	return (a.first-x) * (a.first-x) * b.second <= (b.first-x) * (b.first-x) * a.second;
}

bool cmp3(pt a, pt b, pt c){
	ll v1 = c.first-b.first;
	ll v3 = b.first-a.first;
	ll v2 = c.first-a.first;
	assert(v1 >= 0 && v2 >= 0 && v3 >= 0);

	ll a1 = v1*v1*a.second;
	ll a2 = v2*v2*b.second;
	ll a3 = v3*v3*c.second;
	if(a2 < a1 + a3) return false;
	return i128(i128(a2) - a1 - a3) * i128(i128(a2) - a1 - a3) >= 4 * i128(a1) * i128(a3);
}

struct hull {
	vector<pair<pt, int>> hull_pts;
	int idx = 0;
	void add(pt r, int id){
		if(hull_pts.empty() || r.second < hull_pts.back().first.second){
			while(idx+1 < (int)hull_pts.size() && cmp3(hull_pts[hull_pts.size() - 2].first, hull_pts[hull_pts.size() - 1].first, r)){
				hull_pts.pop_back();
			}
			hull_pts.push_back({r, id});
		}
		ll cx = r.first;
		while(idx + 1 < (int)hull_pts.size() && cmp(hull_pts[idx].first, hull_pts[idx+1].first, cx)){
			idx++;
		}
	}
	pair<pt, int> query(ll cx){
		while(idx + 1 < (int)hull_pts.size() && cmp(hull_pts[idx].first, hull_pts[idx+1].first, cx)){
			idx++;
		}
		if(idx == hull_pts.size()) return {{0, 0}, -1};
		return hull_pts[idx];
	}
};

// x, q, id

struct UF {
	int n;
	vector<int> par;
	UF(int _n) : n(_n) {
		for(int i = 0; i < n; i++) par.push_back(i);
	}
	int find(int a){
		if(a != par[a]) par[a] = find(par[a]);
		return par[a];
	}
	bool join(int a, int b){
		a = find(a);
		b = find(b);
		par[a] = b;
		return (a != b);
	}
};

void solve(){
	int N;
	cin >> N;
	vector<tuple<ll, ll, char>> charges;
	for(int i = 0; i < N; i++){
		ll q, x;
		cin >> q >> x;
		char c;
		cin >> c;
		charges.push_back({x, q, c});
	}
	sort(charges.begin(), charges.end());
	UF uf(N+1);
	bool has_letter = false;
	vector<ll> X(N), Q(N);
	UF uf_bipartite(2*N+2);
	for(int i = 0; i < N; i++){
		auto [x, q, c] = charges[i];
		X[i] = x;
		Q[i] = q;
		if(c == 'A'){
			uf_bipartite.join(2*i, 2*N+1);
			uf_bipartite.join(2*i+1, 2*N);
			uf.join(N, i);
			has_letter = true;
		} else if(c == 'B'){
			uf_bipartite.join(2*i, 2*N);
			uf_bipartite.join(2*i+1, 2*N+1);
			uf.join(N, i);
			has_letter = true;
		}
	}
	if(!has_letter){
		uf_bipartite.join(0, 2*N);
		uf_bipartite.join(1, 2*N+1);
		uf.join(0, N);
	}
	vector<pair<int,int>> mst;
	while(true){
		bool done = true;
		for(int i = 0; i < N; i++) if(uf.find(i) != uf.find(0)) done = false;
		if(done) break;

		int V = 0;
		std::vector<int> cc_id(N+1, -1);
		for (int i = 0; i <= N; i++) {
			if (uf.find(i) == i) {
				cc_id[i] = V++;
			}
		}
		// best edge per vertex
		vector<pair<pair<ll, ll>, pair<int,int> >> best_per(N+1, pair<pair<ll, ll>, pair<int,int> >{{0, 0}, {-1, -1}});
		for(int dir = 0; dir < 2; dir++){
			// cerr << "dir " << dir << '\n';
			vector<hull> hulls(2*V);
			for(int it = 0; it < N; it++){
				int id = (dir == 0) ? it : (N-1-it);
				ll x = (dir == 0) ? X[id] : (-X[id]);
				auto upd = [&](int p, pt r){
					p += V;
					while(p > 1) {
						hulls[p].add(r, id);
						p >>= 1;
					}
				};
				auto max_pt = [&](pair<pt, int> a, pair<pt, int> b) -> pair<pt, int> {
					if(a.second == -1) return b;
					if(b.second == -1) return a;
					return (cmp(a.first, b.first, x) ? b : a);
				};
				auto query = [&](int p){
					pair<pt, int> res = {{0, 0}, -1};
					p += V;
					while (p > 1) {
						res = max_pt(res, hulls[p ^ 1].query(x));
					}
					return res;
				};

				int cc = cc_id[uf.find(id)];
				auto best = query(cc);

				// for(int x = 0; x < 2*V; x++){
				// 	// cerr << "hull " << x << ' ';
				// 	for(int j = hulls[x].idx; j < (int)hulls[x].hull_pts.size(); j++){
				// 		auto r = hulls[x].hull_pts[j];
				// 		cerr << r.first.first << ' ' << r.first.second << ' ' << r.second << " | ";;
				// 	}
				// 	cerr << '\n';
				// }
				if(best.second >= 0){
					int j = best.second;
					pair<pair<ll, ll>, pair<int,int>> score = {{(X[j] - X[id]) * (X[j] - X[id]), Q[j] * Q[id]}, {id, j}};
					auto [sx, sy] = score.first;
					auto [px, py] = best_per[cc].first;
					if(best_per[cc].second.first == -1 || sx * py >= sy * px) best_per[cc] = score;
					// cerr << id << ' ' << j << ' ' << sx << '/' << sy << '\n';
				}
				upd(cc, {x, Q[id]});
			}
		}

		for(int cc = 0; cc < N; cc++){
			auto [i, j] = best_per[cc].second;
			if(j >= 0 && uf.join(i, j)){
				// cerr << i << ' ' << j << '\n';
				uf_bipartite.join(2*i, 2*j+1);
				uf_bipartite.join(2*i+1, 2*j);
			}
		}
	}
	vector<vector<pair<ll, ll> >> sides(2);
	for(int i = 0; i < N; i++){
		bool res = (uf_bipartite.find(2*i) == uf_bipartite.find(2*N));
		sides[res].push_back({X[i], Q[i]});
	}
	pair<ll, ll> best = {1, 2.26e8};
	for(auto side : sides){
		hull side_hull;
		int id = 0;
		for(auto [x, q] : side){
			auto prv = side_hull.query(x);
			if(prv.second >= 0){
				auto [px, pq] = prv.first;
				pair<ll, ll> new_score = {(x-px)*(x-px), q*pq};
				if(new_score.first * best.second > new_score.second * best.first) best = new_score;
			}
			side_hull.add({x, q}, id);
			id++;
		}
	}
	ll a = best.second;
	ll b = best.first;
	ll g = gcd(a, b);
	a /= g; b /= g;
	cout << a << '/' << b << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}
