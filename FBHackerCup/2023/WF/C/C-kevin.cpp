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

using ll = int64_t;
void solve(int t){
	int N, M;
	cin >> N >> M;
	vector<ll> P(N);
	for(int i = 0; i < N; i++){
		cin >> P[i];
	}
	UF uf(N);
	vector<int> ans_par(N, -1);
	vector<ll> ans_min(N, 0);
	vector<vector<int> > adj(N);
	for(int i = 0; i < M; i++){
		int A, B;
		cin >> A >> B;
		A--; B--;
		adj[A].push_back(B);
		adj[B].push_back(A);
	}
	vector<ll> total_power = P;
	set<pair<ll, int> > cities; // sorted by total size
	for(int i = 0; i < N; i++){
		cities.insert({total_power[i], i});
	}
	vector<set<int> > ccs(N);
	for(int i = 0; i < N; i++){ ccs[i] = {i}; }

	vector<int> cur_cc_idx(N);
	for(int i = 0; i < N; i++) cur_cc_idx[i] = i;
	auto combine_ccs = [&](int x, int y) -> int {
		if(ccs[x].size() < ccs[y].size()) swap(x, y);
		ccs[x].insert(ccs[y].begin(), ccs[y].end());
		ccs[y].clear();
		return x;
	};

	vector<int> ans_order;

	int root = -1;
	while(!cities.empty()){
		auto [pp, v] = *cities.begin();
		cities.erase({pp, v});
		if(pp != total_power[v] && pp == P[v]){
			// doing city for the first time, search neighbors
			int vr = uf.find(v);
			y_combinator(
				[&](auto self, int c) -> void {
					for(int w : adj[c]){
						if(uf.find(w) == uf.find(vr)){
							continue;
						}
						if(P[w] <= pp){
							int wr = uf.find(w);
							// cerr << "merge0 " << wr << "->" << vr << '\n';
							total_power[vr] += total_power[wr];
							total_power[wr] = 0;
							assert(ans_par[wr] == -1);
							ans_par[wr] = vr;
							ans_order.push_back(wr);
							uf.join(wr, vr);
							cur_cc_idx[v] = combine_ccs(cur_cc_idx[vr], cur_cc_idx[wr]);
							self(wr);
						}
					}
				}
			)(v);
			cities.insert({total_power[v], v});
			continue;
		}
		if(pp != total_power[v]) continue;
		if(uf.find(v) != v) continue;
		int found = 0;
		// cerr << "solving " << v << " power " << pp << '\n';
		y_combinator(
			[&](auto self, int c) -> void {
				for(int w : adj[c]){
					if(uf.find(w) == uf.find(v)){
						continue;
					}
					if(P[w] <= pp){
						int wr = uf.find(w);
						// cerr << "merge1 " << wr << "->" << v << '\n';
						total_power[v] += total_power[wr];
						total_power[wr] = 0;
						assert(ans_par[wr] == -1);
						ans_par[wr] = v;
						ans_order.push_back(wr);
						uf.join(wr, v);
						cur_cc_idx[v] = combine_ccs(cur_cc_idx[v], cur_cc_idx[wr]);
						found = 1;
						self(wr);
					}
				}
			}
		)(v);
		if(found){
			cities.insert({total_power[v], v});
			// merged into larger
			continue;
		} else {
			int best = -1;
			for(int a : ccs[cur_cc_idx[v]]){
				for(int w : adj[a]){
					if(uf.find(w) == uf.find(v)) continue;
					if(best == -1 || P[w] <= P[best]) best = w;
				}
			}
			if(best == -1){
				assert((int)ccs[cur_cc_idx[v]].size() == N);
				root = v;
				break;
			}
			// cerr << total_power[v] << ' ' << P[best] << ' ' << total_power[uf.find(best)] << '\n';
			// cerr << "merge2 " << v << "->" << best << ' ' << uf.find(best) << '\n';
			assert(total_power[v] <= P[best]);
			ans_min[v] = max(ans_min[v], P[best] - total_power[v]);

			// merge v into w
			int w = uf.find(best);
			assert(w != v);
			total_power[w] += total_power[v];
			total_power[v] = 0;
			assert(ans_par[v] == -1);
			ans_par[v] = w;
			ans_order.push_back(v);
			uf.join(v, w);
			cur_cc_idx[w] = combine_ccs(cur_cc_idx[w], cur_cc_idx[v]);
			cities.insert({total_power[w], w});
		}
	}
	assert(root >= 0);
	reverse(ans_order.begin(), ans_order.end());
	for(int x : ans_order){
		ans_min[x] = max(ans_min[x], ans_min[ans_par[x]]);
	}
	ll sum = 0;
	for(ll x : ans_min) sum += x;
	cout << "Case #" << t << ": " << sum << '\n';
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