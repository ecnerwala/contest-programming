#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct edge {
	int src;
	int dest;
	ll length;
	ll edge_heal;
	edge (int a, int d, ll b, ll c) : src(a), dest(d), length(b), edge_heal(c) {}
	edge(){}
};
vector<ll> vertex_heal;
vector<vector<edge> > edges;

vector<edge> par;
vector<int> vis, depth, used;

vector<ll> min_longest_down;
vector<ll> cycle_min_longest_down; // from top of cycle
vector<vector<int> > up_children;
vector<edge> up_edge;

vector<ll> cycle_min_longest_up;

// solve longest path problem for a cycle
ll calc_z(const vector<ll>& min_paths, const vector<ll>& verts, const vector<edge>& edges){
	int n = (int)edges.size();
	vector<ll> fsum(n+1, 0);
	vector<ll> bsum(n+1, 0);
	for(ll i = 0; i < n; i++) fsum[i+1] = fsum[i] + edges[i].length;
	for(ll i = n-1; i >= 0; i--) bsum[i] = bsum[i+1] + edges[i].length;

	vector<ll> addfsum = fsum;
	vector<ll> addbsum = bsum;
	for(int i = 0; i <= n; i++) addfsum[i] += min_paths[i];
	for(int i = 0; i <= n; i++) addbsum[i] += min_paths[i];
	for(int i = 0; i < n; i++) addfsum[i+1] = max(addfsum[i], addfsum[i+1]);
	for(int i = n-1; i >= 0; i--) addbsum[i] = max(addbsum[i], addbsum[i+1]);
	ll z = 1e18;
	for(int f = 0; f < (int)edges.size(); f++){
		ll val = max(addfsum[f], addbsum[f+1]);
		val = max(val, fsum[f] + edges[f].edge_heal + vertex_heal[verts[f]]);
		val = max(val, bsum[f+1] + edges[f].edge_heal + vertex_heal[verts[f+1]]);
		z = min(z, val);
	}
	return z;
}

vector<multiset<ll> > here_costs;

void dfs(int v, edge pe, int d){
	int p = pe.src ^ pe.dest ^ v;
	if(vis[v]){
		if(depth[p] < depth[v]){
			// p over v
			up_edge[v] = pe;
			up_children[p].push_back(v);
			vector<edge> edges;
			vector<ll> min_paths;
			vector<ll> verts;
			int x = v;
			min_paths.push_back(0);
			verts.push_back(p);

			edges.push_back(pe);
			while(x != p){
				min_paths.push_back(min_longest_down[x]);
				verts.push_back(x);
				edges.push_back(par[x]);
				x = par[x].src ^ par[x].dest ^ x;
			}
			min_paths.push_back(0);
			verts.push_back(p);

			cycle_min_longest_down[v] = calc_z(min_paths, verts, edges);
			min_longest_down[p] = min(min_longest_down[p], cycle_min_longest_down[v]);
			//cerr << p << " " << v << " " << cycle_min_longest_down[v] << '\n';
		} else {
			// up
			int x = p;
			while(x != v){
				assert(!used[x]);
				used[x] = 1;
				x = par[x].src ^ par[x].dest ^ x;
			}
		}
		return;
	}
	depth[v] = d; par[v] = pe; vis[v] = 1;
	for(edge e : edges[v]){
		int w = e.src ^ e.dest ^ v;
		if(w == p) continue;
		dfs(w, e, d+1);
	}
	if(p != -1 && !used[v]){
		// bridge
		up_edge[v] = pe;
		up_children[p].push_back(v);
		cycle_min_longest_down[v] = min_longest_down[v] + pe.length;
		min_longest_down[p] = max(min_longest_down[p], cycle_min_longest_down[v]);		
		//cerr << p << " " << v << " " << cycle_min_longest_down[v] << '\n';
	}
}

void dfs2(int v, int p){
	for(int w : up_children[v]){
		here_costs[v].insert(cycle_min_longest_down[w]);
	}
	if(p >= 0){
		here_costs[v].insert(cycle_min_longest_up[v]);
	}
	for(int w : up_children[v]){
		here_costs[v].erase(here_costs[v].find(cycle_min_longest_down[w]));
		if(!used[w]){
			cycle_min_longest_up[w] = cycle_min_longest_up[v] + par[w].length;
		} else {
			vector<ll> verts;
			vector<ll> min_paths;
			vector<edge> edges;

			verts.push_back(w);
			min_paths.push_back(0);

			int x = w;
			while(x != v){
				edges.push_back(par[x]);
				x = par[x].src ^ par[x].dest ^ x;
				verts.push_back(x);
				min_paths.push_back((x == v) ? *here_costs[v].rbegin() : min_longest_down[x]);
			}

			edges.push_back(up_edge[w]);
			verts.push_back(w);
			min_paths.push_back(0);

			ll z = calc_z(min_paths, verts, edges);
			cycle_min_longest_up[w] = z;
		}
		//cerr << v << " " << w << " " << cycle_min_longest_up[w] << '\n';
		dfs2(w, v);
		here_costs[v].insert(cycle_min_longest_down[w]);
	}
}

ll ans = 1e18;

namespace wala {

struct range_data {
	ll top_len = 0;
	ll max_left = 0;
	ll max_right = 0;
	ll max_in = 0;
};

range_data vert(ll v) {
	return range_data{0, v, v, v};
}
range_data horiz(ll v) {
	return range_data{v, v, v, v};
}

range_data merge(range_data x, range_data y) {
	range_data r;
	r.top_len = x.top_len + y.top_len;
	r.max_left = max(x.max_left, x.top_len + y.max_left);
	r.max_right = max(x.max_right + y.top_len, y.max_right);
	r.max_in = max(max(x.max_in, y.max_in), x.max_right + y.max_left);
	return r;
}

void solve_island(const vector<ll>& min_paths, const vector<ll>& verts_heal, const vector<edge>& edges){
	int n = (int)min_paths.size();

	vector<range_data> pref(n);
	vector<range_data> suff(n);
	range_data cur_pref;
	for (int i = 0; i < n; i++) {
		cur_pref = merge(cur_pref, vert(min_paths[i]));
		cur_pref = merge(cur_pref, vert(min_paths[i]));
		pref[i] = merge(cur_pref, horiz(verts_heal[i] + edges[i].edge_heal));
		// extend it with the edge
		cur_pref = merge(cur_pref, horiz(edges[i].length));
	}

	range_data cur_suff;
	for (int i = n-1; i >= 0; i--) {
		suff[i] = merge(horiz(verts_heal[(i+1)%n] + edges[i].edge_heal), cur_suff);
		cur_suff = merge(horiz(edges[i].length), cur_suff);
		cur_suff = merge(vert(min_paths[i]), cur_suff);
		cur_suff = merge(vert(min_paths[i]), cur_suff);
	}

	ll curAns = ll(1e18);
	for (int i = 0; i < n; i++) {
		ll cur = merge(suff[i], pref[i]).max_in;
		curAns = min(curAns, cur);
	}
	ans = min(ans, curAns);

	//cerr << "==============" << '\n';
	for(int i = 0; i < n; i++){
		//cerr << "path = " << min_paths[i] << " v = " << verts_heal[i] << " len = " << edges[i].length << " " << edges[i].edge_heal << '\n';
	}
	//cerr << curAns << '\n';
}
}
using wala::solve_island;

void dfs3(int v){
	for(int w : up_children[v]){
		// solve this ver
		here_costs[v].erase(here_costs[v].find(cycle_min_longest_down[w]));
		here_costs[w].erase(here_costs[w].find(cycle_min_longest_up[w]));
		if(!used[w]){
			// bridge
			ll up = *here_costs[v].rbegin();
			ll len = up_edge[w].length;
			ll down = cycle_min_longest_down[w];
			if(abs(up - down) <= len){
				ans = min(ans, up + down + len);
			}
		} else {
			// help
			int x = w;
			vector<ll> verts;
			vector<ll> min_paths;
			vector<edge> edges;
			while(x != v){
				verts.push_back(vertex_heal[x]);
				min_paths.push_back(*here_costs[x].rbegin());
				edges.push_back(par[x]);

				x = par[x].src ^ par[x].dest ^ x;
			}
			verts.push_back(vertex_heal[x]);
			min_paths.push_back(*here_costs[x].rbegin());
			edges.push_back(up_edge[w]);
			solve_island(min_paths, verts, edges);
		}
		here_costs[v].insert(cycle_min_longest_down[w]);
		here_costs[w].insert(cycle_min_longest_up[w]);
	}
	for(int w : up_children[v]){
		dfs3(w);
	}
}

int main(){
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int n, m;
	cin >> n >> m;
	vertex_heal.resize(n);
	for(int i = 0; i < n; i++) cin >> vertex_heal[i];
	edges.assign(n, {}), par.resize(n), vis.assign(n, 0), depth.resize(n), used.assign(n, 0);
	up_edge.resize(n), up_children.resize(n);
	min_longest_down.resize(n, 0);
	cycle_min_longest_down.resize(n, 0);
	cycle_min_longest_up.resize(n, 0);
	here_costs.resize(n);
	for(int i = 0; i < n; i++){
		here_costs[i].insert(0);
	}
	for(int i = 0; i < m; i++){
		int a, b;
		ll l, e;
		cin >> a >> b >> l >> e;
		a--; b--;
		edges[a].push_back(edge(a, b, l, e));
		edges[b].push_back(edge(b, a, l, e));
	}
	edge nothing(-1, 0, 0, 0);
	dfs(0, nothing, 0);
	dfs2(0, -1);
	dfs3(0);
	cout << ans << '\n';
}
