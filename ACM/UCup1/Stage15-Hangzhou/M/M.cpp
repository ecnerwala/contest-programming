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

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M;
	cin >> N >> M;
	vector<ll> back_edges(N);
	vector<ll> A(N), B(N);
	for(int i = 1; i < N; i++){
		cin >> A[i] >> B[i];
	}
	for(int i = 0; i < M; i++){
		int u, v;
		cin >> u >> v;
		u--; v--;
		back_edges[v] |= ll(1) << u;
	}
	auto solve1 = [&](){
		vector<ll> del(1 << N, 0);
		for (int j = 0; j < N; j++) {
			ll v = B[j] - A[j];
			for (int m = 0; m < (1 << j); m++) {
				del[m+(1<<j)] = del[m] + v;
			}
		}
		vector<ll> dp(1 << N, 1e18);
		dp[1] = 0;
		for(int x = 1; x < (1 << N); x += 2) {
			for(int j = 1; j < N; j++){
				if(x & (1 << j)){
					if(back_edges[j] & x){
						dp[x] = min(dp[x], max(dp[x ^ (1 << j)], A[j] - del[x ^ (1 << j)]));
					}
				}
			}
		}
		return dp[(1 << N) - 1];
	};
	auto solve2 = [&](){
		ll best_ans = 1e18;
		vector<int> par(N, -1);

		vector<vector<int> > ch(N);
		for (int i = 0; i < N; i++) {
			ch[i].reserve(N);
		}
		vector<vector<pair<ll,ll> > > dp(N);
		for (int i = 0; i < N; i++) {
			dp[i].reserve(N);
		}
		vector<pair<ll,ll> > up, down;
		up.reserve(N);
		down.reserve(N);

		auto solve = [&](){
			for (int i = 0; i < N; i++) {
				ch[i].clear();
			}
			for(int v = 1; v < N; v++) ch[par[v]].push_back(v);
			for(int v = N-1; v >= 0; v--){
				dp[v].clear();
				up.clear();
				down.clear();
				for(int x : ch[v]){
					for(auto [a, b] : dp[x]){
						if(a <= b){
							up.push_back({a, b});
						} else {
							down.push_back({a, b});
						}
					}
				}
				sort(up.begin(), up.end(), [&](pair<ll,ll> a, pair<ll,ll> b) { return a.first < b.first; });
				sort(down.begin(), down.end(), [&](pair<ll,ll> a, pair<ll,ll> b) { return a.second > b.second; });
				up.insert(up.begin(), {A[v], B[v]});
				up.insert(up.end(), down.begin(), down.end());
				for(auto [a, b] : up){
					while(!dp[v].empty()){
						auto [pa, pb] = dp[v].back();

						ll s1 = 0;
						ll s2 = pb - pa;
						ll s3 = pb - pa + b - a;
						if(s2 >= s1 && s2 >= s3){
							break;
						}
						if(s1 <= s2 && s2 <= s3 && a > pb){
							break;
						}
						if(s1 >= s2 && s2 >= s3 && a < pb){
							break;
						}
						ll na = max(pa, pa + a - pb);
						ll nb = max(b, b + pb - a);
						a = na;
						b = nb;
						dp[v].pop_back();
					}
					dp[v].push_back({a, b});
				}
			}
			// for(int x : par){
			// 	cerr << x << ' ';
			// }
			// cerr << '\n';
			// for(int j = N-1; j >= 0; j--){
			// 	cerr << j << ": ";
			// 	for(auto [x, y] : dp[j]){
			// 		cerr << x << '/' << y << ' ';
			// 	}
			// 	cerr << '\n';
			// }
			ll res = 0;
			ll worst = 0;
			for(auto [x, y] : dp[0]){
				res -= x;
				worst = max(worst, -res);
				res += y;
			}
			best_ans = min(best_ans, worst);
		};
		y_combinator(
			[&](auto self, int x) -> void {
				if(x == N){
					solve();
					return;
				}
				for(int j = 0; j < N; j++){
					if(back_edges[x] & (ll(1) << j)){
						par[x] = j;
						self(x+1);
					}
				}
			}
		)(1);
		return best_ans;
	};
	// if(N + M <= 20){
	// 	ll a1 = solve1();
	// 	ll a2 = solve2();
	// 	cerr << a1 << ' ' << a2 << '\n';
	// 	assert(solve1() == solve2());
	// }
	ll ans;
	if(N <= 25){
		ans = solve1();
	} else {
		ans = solve2();
	}
	cout << ans << '\n';
}
