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

struct vtx_dp {
	// [del v][cnt]
};

struct edge_dp {
	ll v;
	// [del u][del v][del extra][path];
};

void solve(){
	int N, M;
	cin >> N >> M;
	vector<pair<int,int> > edges; edges.reserve(M);
	vector<int> weights; weights.reserve(M);
	std::map<std::pair<int, int>, int> edge_idx;
	const int64_t INF = 1e18;
	int64_t ans = -INF;
	for(int i = 0; i < M; i++){
		int u, v, w;
		cin >> u >> v >> w;
		u--; v--;
		if (u > v) std::swap(u, v);
		auto [it, did_insert] = edge_idx.insert({{u, v}, int(edges.size())});
		if (did_insert) {
			edges.push_back({u, v});
			weights.push_back(w);
		} else {
			ans = std::max(ans, int64_t(w) + it->second);
			it->second = std::max(it->second, w);
		}
	}
	int M = int(edges.size());
	auto merge_into = [&](vtx_dp v, edge_dp a, vtx_dp w) -> vtx_dp {
		// do nothing
		return vtx_dp{};
	};
	auto merge_series = [&](edge_dp a, vtx_dp v, edge_dp b) -> edge_dp {
		return {a.v + b.v};
	};
	auto merge_parallel = [&](edge_dp a, edge_dp b) -> edge_dp {
		ans = std::max(ans, a.v + b.v);
		return {std::max(a.v, b.v)};
	};
	vector<vector<int> > adj(N);
	vector<int> deg(N, 0);
	vector<vtx_dp> v_dp(N, vtx_dp{});
	vector<edge_dp> e_dp(M);
	map<pair<int,int>, int> edge_to_id;
	for(int i = 0; i < M; i++){
		e_dp[i].v = weights[i];
		auto [x, y] = edges[i];
		deg[x]++;
		deg[y]++;
		edge_to_id[{x, y}] = i;
		edge_to_id[{y, x}] = i;
		adj[x].push_back(i);
		adj[y].push_back(i);
	}
	vector<int> vtx_alive(N, 1);
	vector<int> edge_alive(M, 1);
	vector<int> verts;
	for(int v = 0; v < N; v++) verts.push_back(v);
	auto get_eids = [&](int v) -> vector<int> {
		vector<int> eids;
		for(int eid : adj[v]){
			if(!edge_alive[eid]) continue;
			eids.push_back(eid);
		}
		return eids;
	};
	auto flip = [&](int eid) -> void {
		swap(edges[eid].first, edges[eid].second);
	};
	vector<int> deg0s;
	while(!verts.empty()){
		int v = verts.back();
		verts.pop_back();
		if(!vtx_alive[v]) continue;
		if(deg[v] == 0){
			deg0s.push_back(v);
		} else if(deg[v] == 1 || deg[v] == 2){
			// cout << "trying " << v << ' ' << deg[v] << '\n';
			vector<int> eids = get_eids(v);
			// for(int xx : eids){
			// 	cout << xx << ' ' << edges[xx].first << ' ' << edges[xx].second << '\n';
			// }
			assert((int)eids.size() == deg[v]);
			if(deg[v] == 1){
				int eid = eids[0];
				if(edges[eid].second != v) flip(eid);
				assert(edges[eid].second == v);
				int w = edges[eid].first ^ edges[eid].second ^ v;
				v_dp[w] = merge_into(v_dp[w], e_dp[eid], v_dp[v]);

				deg[v]--;
				deg[w]--;
				vtx_alive[v] = 0;
				edge_alive[eid] = 0;
				verts.push_back(w);
				edge_to_id.erase({v, w});
				edge_to_id.erase({w, v});
			} else if(deg[v] == 2){
				int eid_0 = eids[0];
				int eid_1 = eids[1];
				if(edges[eid_0].second != v) flip(eid_0);
				assert(edges[eid_0].second == v);
				if(edges[eid_1].first != v) flip(eid_1);
				assert(edges[eid_1].first == v);

				int u = edges[eid_0].first;
				int w = edges[eid_1].second;
				deg[v] = 0;
				vtx_alive[v] = 0;
				edge_alive[eid_0] = edge_alive[eid_1] = 0;
				{
					edge_to_id.erase({u, v});
					edge_to_id.erase({w, v});
					edge_to_id.erase({v, w});
					edge_to_id.erase({v, u});
				}
				deg[u]--;
				deg[w]--;
				auto combined_dp = merge_series(e_dp[eid_0], v_dp[v], e_dp[eid_1]);
				if(edge_to_id.count({u, w})){
					int eid = edge_to_id[{u, w}];
					if(edges[eid].first != u) flip(eid);
					assert(edges[eid].first == u);
					assert(edges[eid].second == w);
					e_dp[eid] = merge_parallel(e_dp[eid], combined_dp);
					verts.push_back(u);
					verts.push_back(w);
				} else {
					int eid = M;
					M++;
					edges.push_back({u, w});
					edge_alive.push_back(1);
					e_dp.push_back(combined_dp);
					deg[u]++; deg[w]++;
					edge_to_id[{u, w}] = eid;
					edge_to_id[{w, u}] = eid;
					adj[u].push_back(eid);
					adj[w].push_back(eid);
				}
			}
		}
	}
	assert(ans != -INF);
	cout << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);	
	int T; 
	T = 1;
	while(T--) solve();
}
