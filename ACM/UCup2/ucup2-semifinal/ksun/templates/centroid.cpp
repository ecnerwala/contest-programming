#include <bits/stdc++.h>
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

using ll = int64_t;
void solve(){
	int N;
	cin >> N;
	vector<vector<pair<int, ll>>> adj(N);
	ll ans = 0;
	for(int i = 0; i < N-1; i++){
		int u, v;
		ll w;
		cin >> u >> v >> w;
		u--; v--;
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
		ans = max(ans, w);
	}
	vector<int> exists(N, 1);
	vector<int> sz(N);
	auto find_centroid = [&](int st) -> int {
		y_combinator(
			[&](auto self, int v, int p) -> void {
				sz[v] = 1;
				for(auto [w, c] : adj[v]){
					if(w == p || !exists[w]) continue;
					self(w, v);
					sz[v] += sz[w];
				}
			}
		)(st, -1);
		int total_sz = sz[st];
		int centroid = -1;
		y_combinator(
			[&](auto self, int v, int p) -> void {
				if(sz[v] * 2 >= total_sz) centroid = v;
				for(auto [w, c] : adj[v]){
					if(w == p || !exists[w]) continue;
					self(w, v);
				}
			}
		)(st, -1);
		return centroid;
	};
	y_combinator(
		[&](auto self, int st) -> void {
			st = find_centroid(st);
			vector<vector<int> > group;
			y_combinator(
				[&](auto self2, int v, int p, int d) -> void {
					for(auto [w, c] : adj[v]){
						if(w == p || !exists[w]) continue;
						self2(w, v, d+1);
					}
				}
			)(st, -1, 0);
			exists[st] = 0;
			for(auto [w, c] : adj[st]){
				if(!exists[w]) continue;
				self(w);
			}
		}
	)(0);
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}