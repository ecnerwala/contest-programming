#include <bits/stdc++.h>
using namespace std;

namespace kactl {

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, top = me;
	for (auto [y, e] : ed[at]) if (e != par) {
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				f();
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else {
				f();
			}
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}

#undef sz
#undef all
#undef rep

}

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
	std::array<int, 2> end_ids;
	int64_t series_mid;
	// [del u][del v][del extra][path];
};

void solve(){
	int N, M;
	cin >> N >> M;
	std::vector<int> v_weights(N);
	for (auto& w : v_weights) std::cin >> w;
	vector<pair<int,int> > edges; edges.reserve(M+1);
	std::map<std::pair<int, int>, int> edge_idx;
	int64_t ans = 0;
	for(int i = 0; i <= M; i++){
		int u, v;
		if (i == M) {
			// add this extra edge
			u = 0, v = N-1;
		} else {
			cin >> u >> v;
			u--; v--;
		}
		assert(u < v);
		if (u > v) std::swap(u, v);
		// subtract all given edges
		auto [it, did_insert] = edge_idx.insert({{u, v}, int(edges.size())});
		if (did_insert) {
			if (i != M) {
				ans -= v_weights[u] * v_weights[v];
			}
			edges.push_back({u, v});
		} else {
			// do nothing
		}
	}
	M = int(edges.size());
	// we need the whole thing to be just 1 bcc, let's check that first
	{
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			auto [u, v] = edges[e];
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
		}
		kactl::ed = std::move(adj);
		int cnt_bccs = 0;
		kactl::bicomps([&]() { cnt_bccs++; });
		assert(cnt_bccs >= 1);
		if (cnt_bccs != 1) {
			std::cout << "bad" << '\n';
			return;
		}
	}
	auto merge_into = [&](vtx_dp v, edge_dp a, vtx_dp w) -> vtx_dp {
		// do nothing
		ans += v_weights[a.end_ids[0]] * v_weights[a.end_ids[1]];
		std::cerr << "merge_into" << ' ' << a.end_ids[0] << ' ' << a.end_ids[1] << '\n';
		return vtx_dp{};
	};
	auto merge_series = [&](edge_dp a, vtx_dp v, edge_dp b) -> edge_dp {
		std::cerr << "merge_series" << ' ' << a.end_ids[0] << ' ' << b.end_ids[0] << ' ' << b.end_ids[1] << '\n';
		assert(a.end_ids[1] == b.end_ids[0]);
		ans += v_weights[a.end_ids[0]] * v_weights[a.end_ids[1]];
		ans += v_weights[b.end_ids[0]] * v_weights[b.end_ids[1]];
		ans += a.series_mid * b.series_mid;
		ans += a.series_mid * v_weights[b.end_ids[1]];
		ans += v_weights[a.end_ids[0]] * b.series_mid;
		return {{a.end_ids[0], b.end_ids[1]}, a.series_mid + v_weights[a.end_ids[1]] + b.series_mid};
	};
	auto merge_parallel = [&](edge_dp a, edge_dp b) -> edge_dp {
		std::cerr << "merge_parallel" << ' ' << a.end_ids[0] << ' ' << a.end_ids[1] << '\n';
		assert(a.end_ids == b.end_ids);
		return {a.end_ids, 0};
	};
	vector<vector<int> > adj(N);
	vector<int> deg(N, 0);
	vector<vtx_dp> v_dp(N, vtx_dp{});
	vector<edge_dp> e_dp(M);
	map<pair<int,int>, int> edge_to_id;
	for(int i = 0; i < M; i++){
		auto [x, y] = edges[i];
		e_dp[i] = {{x, y}, 0};
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
		std::swap(e_dp[eid].end_ids[0], e_dp[eid].end_ids[1]);
	};
	vector<int> deg0s;
	while(!verts.empty()){
		int v = verts.back();
		verts.pop_back();
		if(!vtx_alive[v]) continue;
		if(deg[v] == 0){
			deg0s.push_back(v);
			vtx_alive[v] = 0;
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
	// if we didn't kill everything, we're bad
	for (auto v : vtx_alive) {
		if (v) {
			std::cout << "bad" << '\n';
			return;
		}
	}
	for (auto e : edge_alive) {
		if (e) {
			std::cout << "bad" << '\n';
			return;
		}
	}
	cout << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);	
	int T; std::cin >> T;
	while(T--) solve();
}
