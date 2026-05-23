#include <bits/stdc++.h>
#include <cassert>
using namespace std;

template <typename T> void set_min(T& a, const T& b){
	if(b < a) a = b;
}
template <typename T> void set_max(T& a, const T& b){
	if(b > a) a = b;
}

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

// // smawck for max
// template<class F>
// std::vector<int> smawck(F f, const std::vector<int> &rows, const std::vector<int> &cols) {
//     std::vector<int> ans(rows.size(), -1);
//     if((int) std::max(rows.size(), cols.size()) <= 2) {
//         for(int i = 0; i < (int) rows.size(); i++) {
//             for(auto j : cols) {
//                 if(ans[i] == -1 || f(rows[i], ans[i], j)) {
//                     ans[i] = j;
//                 }
//             }
//         }
//     } else if(rows.size() < cols.size()) {
//         // reduce
//         std::vector<int> st;
//         for(int j : cols) {
//             while(1) {
//                 if(st.empty()) {
//                     st.push_back(j);
//                     break;
//                 } else if(f(rows[(int) st.size() - 1], st.back(), j)) {
//                     st.pop_back();
//                 } else if(st.size() < rows.size()) {
//                     st.push_back(j);
//                     break;
//                 } else {
//                     break;
//                 }
//             }
//         }
//         ans = smawck(f, rows, st);
//     } else {
//         std::vector<int> newRows;
//         for(int i = 1; i < (int) rows.size(); i += 2) {
//             newRows.push_back(rows[i]);
//         }
//         auto otherAns = smawck(f, newRows, cols);
//         for(int i = 0; i < (int) newRows.size(); i++) {
//             ans[2*i+1] = otherAns[i];
//         }
//         for(int i = 0, l = 0, r = 0; i < (int) rows.size(); i += 2) {
//             if(i+1 == (int) rows.size()) r = (int) cols.size();
//             while(r < (int) cols.size() && cols[r] <= ans[i+1]) r++;
//             ans[i] = cols[l++];
//             for(; l < r; l++) {
//                 if(f(rows[i], ans[i], cols[l])) {
//                     ans[i] = cols[l];
//                 }
//             }
//             l--;
//         }
//     }
//     return ans;
// }

// // max smawck
// // F(i, j, k) checks if M[i][j] <= M[i][k]
// // another interpretations is: 
// // F(i, j, k) checks if M[i][k] is at least as good as M[i][j]
// // higher == better
// // when comparing 2 columns as vectors
// // for j < k, column j can start better than column k
// // as soon as column k is at least as good, it's always at least as good
// template<class F>
// std::vector<int> smawck(F f, int n, int m) {
//     std::vector<int> rows(n), cols(m);
//     for(int i = 0; i < n; i++) rows[i] = i;
//     for(int i = 0; i < m; i++) cols[i] = i;
//     return smawck(f, rows, cols);
// }

// template<class T>
// std::vector<T> conv(std::vector<T> anyShape, const std::vector<T> &convexShape) {
//     if((int) convexShape.size() <= 1) return anyShape;
//     if(anyShape.empty()) anyShape.push_back(0);
//     int n = (int) anyShape.size(), m = (int) convexShape.size();
//     auto function = [&](int i, int j) {
//         return anyShape[j] + convexShape[i-j];
//     };
//     auto comparator = [&](int i, int j, int k) {
//         if(i < k) return false;
//         if(i - j >= m) return true;
//         return function(i, j) <= function(i, k);
//     };
//     const std::vector<int> best = smawck(comparator, n + m - 1, n);
//     std::vector<T> ans(n + m - 1);
//     for(int i = 0; i < n + m - 1; i++) {
//         ans[i] = function(i, best[i]);
//     }
//     return ans;
// }

template<class T>
std::vector<T> conv(std::vector<T>& a, std::vector<T> &b) {
    int n = (int) a.size(), m = (int) b.size();
    std::vector<T> ans(n + m - 1);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			set_max(ans[i+j], a[i] + b[j]);
		}
	}
    return ans;
}

template<class T>
std::vector<T> conv_t(std::vector<T>& a, std::vector<T> &b) {
    int n = (int) a.size(), m = (int) b.size();
    std::vector<T> ans(n - m + 1);
	for(int i = 0; i < n-m+1; i++){
		for(int j = 0; j < m; j++){
			set_max(ans[i], a[i+(m-1-j)] + b[j]);
		}
	}
    return ans;
}

using ll = int64_t;

void solve(){
	int N, K;
	cin >> N >> K;
	vector<ll> W(N);
	for(int i = 0; i < N; i++) cin >> W[i];
	vector<vector<int> > tree(N);
	for(int i = 0; i < N-1; i++){
		int u, v;
		cin >> u >> v;
		u--; v--;
		tree[u].push_back(v);
		tree[v].push_back(u);
	}
	vector<vector<int> > ch(N);
	vector<vector<ll> > down_dp(N);
	vector<vector<ll> > up_dp(N);
	y_combinator(
		[&](auto self, int v, int p) -> void {
			down_dp[v] = {0};
			for(int w : tree[v]){
				if(w == p) continue;
				ch[v].push_back(w);
				self(w, v);
				down_dp[v] = conv(down_dp[v], down_dp[w]);
			}
			down_dp[v].push_back(0);
			for(int i = (int)down_dp[v].size() - 1; i > 0; i--){
				set_max(down_dp[v][i], down_dp[v][i-1] + i * W[v]);
			}
		}
	)(0, -1);
	const int64_t INF = 1e18;
	up_dp[0] = std::vector<ll>(N+1, -INF);
	up_dp[0][N-K] = 0;
	vector<ll> ans(N);
	y_combinator(
		[&](auto self, int v) -> void {
			assert(up_dp[v].size() == down_dp[v].size());
			int L = (int)ch[v].size();
			vector<vector<ll> > l_dps(L+1);
			vector<vector<ll> > r_dps(L+1);
			l_dps[0] = up_dp[v];
			r_dps[L] = {0};
			for(int i = 0; i < L; i++){
				l_dps[i+1] = conv_t(l_dps[i], down_dp[ch[v][i]]);
			}
			for(int i = L-1; i >= 0; i--){
				r_dps[i] = conv(r_dps[i+1], down_dp[ch[v][i]]);
			}
			for(int c = 0; c < L; c++){
				int w = ch[v][c];
				up_dp[w] = conv_t(l_dps[c], r_dps[c+1]);
				up_dp[w].push_back(0);
				for(int i = (int)up_dp[w].size() - 1; i > 0; i--){
					set_max(up_dp[w][i], up_dp[w][i-1] + (K - (int(up_dp[w].size()) - 4 - i)) * W[v]);
				}
				up_dp[w].erase(up_dp[w].begin());
				up_dp[w].pop_back();
			}
			assert(l_dps[L].size() == 2);
			ans[v] = l_dps[L][0] + K * W[v];
			for(int w : ch[v]){
				self(w);
			}
		}
	)(0);
	for(int v = 0; v < N; v++){
		cout << ans[v] << " \n"[v+1 == N];
	}
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}
