#include <bits/stdc++.h>
using namespace std;

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

ll solve_graph(int N, vector<tuple<int, int, ll> > edges){
	int E = (int)edges.size();
	vector<ll> costs(E);
	vector<vector<int> > in_ids(N), out_ids(N);
	for(int i = 0; i < E; i++){
		auto [u, v, cost] = edges[i];
		costs[i] = cost;
		in_ids[v].push_back(i);
		out_ids[u].push_back(i);
	}
	for(int v = 0; v < N; v++){
		sort(out_ids[v].begin(), out_ids[v].end(), [&](int e1, int e2){
			return costs[e1] > costs[e2];
		});
	}
	vector<int> used_in(E, 0);
	vector<int> used_out(E, 0);
	ll base_score = 0;
	UF uf_e(E);
	for(int v = 0; v < N; v++){
		int X = (int)min(in_ids[v].size(), out_ids[v].size());
		for(int j = 0; j < X; j++){
			int e_in = in_ids[v][j];
			int e_out = out_ids[v][j];
			base_score += costs[e_out];
			used_in[e_in] = 1;
			used_out[e_out] = 1;
			uf_e.join(e_in, e_out);
		}
	}
	UF uf_v(N);
	vector<int> cycle_used_in(N);
	{
		vector<vector<int> > cc(E);
		for(int e = 0; e < E; e++) cc[uf_e.find(e)].push_back(e);

		for(int e_cc = 0; e_cc < E; e_cc++){
			if(cc[e_cc].empty()) continue;
			if(!used_in[e_cc] && !used_out[e_cc]) continue;
			bool is_cycle = true;
			for(int e : cc[e_cc]){
				if(used_in[e] != used_out[e]) is_cycle = false;
			}
			if(is_cycle){
				for(int e : cc[e_cc]){
					auto [u, v, cost]= edges[e];
					cycle_used_in[v]++;
					uf_v.join(u, v);
				}
			}
		}
	}
	vector<vector<int> > cc_v(N);
	for(int i = 0; i < N; i++) cc_v[uf_v.find(i)].push_back(i);
	ll ans = base_score;
	for(int z = 0; z < N; z++){
		if(cc_v[z].empty()) continue;
		int total_cycles_in = 0;
		int total_in = 0;
		for(int v : cc_v[z]){
			total_cycles_in += cycle_used_in[v];
			total_in += (int)in_ids[v].size();
		}
		if(total_in != total_cycles_in) continue;
		if(total_cycles_in == 0) continue;


		ll worst = 1e12;
		for(int v : cc_v[z]){
			int X = (int)in_ids[v].size();
			int Y = (int)out_ids[v].size();
			for(int i = 0; i < X; i++){
				worst = min(worst, costs[out_ids[v][i]]);
			}
			for(int i = X; i < Y; i++){
				worst = min(worst, costs[out_ids[v][X-1]] - costs[out_ids[v][i]]);
			}
		}
		ans -= worst;
	}
	return ans;
}

void solve(){
	int N, M, K, start_state;
	cin >> N >> M >> K >> start_state;
	start_state--;
	int V = M + K + 1;
	vector<tuple<int, int, ll> > edges;
	for(int i = 0; i < N; i++){
		int a, c; ll b;
		cin >> a >> b >> c;
		a--; c--;
		edges.push_back({a, c, b});
	}
	{
		int cur = M;
		vector<int> starts;
		for(int i = 0; i < K; i++){
			int a;
			cin >> a;
			a--;
			starts.push_back(a);
		}
		starts.push_back(start_state);
		for(int x : starts){
			edges.push_back({cur, x, 0});
			cur++;		
		}
		assert(cur <= V);
	}
	ll ans = solve_graph(V, edges);
	cout << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}