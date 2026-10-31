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

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int TEST = 1;
	int N;
	cin >> N;
	vector<int> ans(N);
	bool dead = false;
	int n_ops = 0;
	if(TEST){
		mt19937 mt(chrono::steady_clock::now().time_since_epoch().count());
		for(int i = 0; i < N; i++) ans[i] = i;
		shuffle(ans.begin(), ans.end(), mt);
	}
	vector<int> a(N);
	auto query_min = [&](int r, int s) -> int {
		if(r == s) return r;
		// which index is smaller
		int val;
		if(TEST){
			n_ops--;
			if(n_ops <= 0) dead = true;
			if(dead) return -1;
			int x = r;
			int y = s;
			if(x > y) swap(x, y);
			val = ans[x] > ans[y];
			if(ans[x] > ans[y]) swap(ans[x], ans[y]);
		} else {
			cout << (r+1) << ' ' << (s+1) << '\n';
			cout << flush;
			cin >> val;
		}
		if(val == -1) exit(0);
		if(val == 0) return min(r, s);
		return max(r, s);
	};
	mt19937 mt(48);
	for(int it = 0; it < (TEST ? 2 : 1); it++){
		dead = false;
		n_ops = 750;

		int suffix_min = N-1;
		int start_loc = 0;
		for(int i = N-1; i >= 0; i--){
			suffix_min = query_min(suffix_min, i);
			if(dead) break;
			if(suffix_min != i){
				start_loc = i;
			}
		}
		y_combinator(
			[&](auto self, int L, int R) -> void {
				if(dead) return;
				if(R - L <= 1) return;
				if(R - L == 2){
					query_min(L, L+1);
					return;
				}
				for(int x = L; x < R; x++){
					cerr << ans[x] << ' ';
				}
				cerr << '\n';
				int pivot_idx;
				if(R - L <= 40){
					pivot_idx = L + (int)(mt() % (R-L));
				} else {
					// find median of 3?
					pivot_idx = L + (int)(mt() % (R-L));
				}
				int s = L;
				int e = R-1;
				while(s < e){
					if(pivot_idx == s){
						int v = query_min(pivot_idx, e);
						if(dead) return;
						if(v == e){
							pivot_idx = e;
							s++;
						} else if(v == s){
							e--;
						} else assert(false);
					} else if(pivot_idx == e){
						int v = query_min(pivot_idx, s);
						if(dead) return;
						if(v == s){
							s++;
						} else if(v == e){
							pivot_idx = s;
							e--;
						} else assert(false);
					} else {
						int v = query_min(pivot_idx, s);
						if(dead) return;
						if(v == s){
							s++;
						} else {
							pivot_idx = s;
						}
					}
				}
				self(L, pivot_idx);
				self(pivot_idx + 1, R);
			}
		)(suffix_min, N);
		if(TEST){
			for(int i = 0; i < N; i++){
				cerr << ans[i] << ' ';
			}
			cerr << '\n';
			if(it == 1){
				for(int i = 0; i < N; i++){
					assert(ans[i] == i);
				}
			}
		}
	}
}