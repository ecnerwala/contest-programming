#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

using ll = long long;

template <typename flow_t = long long, typename cost_t = long long>
struct MCMF_SSPA {
	int N;
	vector<vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	vector<edge_t> edges;

	vector<char> seen;
	vector<cost_t> pi;
	vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

	void addEdge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	const cost_t INF_COST = numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = numeric_limits<flow_t>::max() / 4;
	vector<cost_t> dist;
	__gnu_pbds::priority_queue<pair<cost_t, int>> q;
	vector<typename decltype(q)::point_iterator> its;
	void path(int s) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({0, s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						prv[j] = e;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		swap(pi, dist);
	}

	pair<flow_t, cost_t> maxflow(int s, int t) {
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (path(s), pi[t] < INF_COST) {
			flow_t curFlow = numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = min(curFlow, edges[e].cap);
				cur = nxt;
			}
			totFlow += curFlow;
			totCost += pi[t] * curFlow;
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				edges[e].cap -= curFlow;
				edges[e^1].cap += curFlow;
				cur = nxt;
			}
		}
		return {totFlow, totCost};
	}
};

template<typename flow_t = ll, typename cost_t = ll>
struct mcSFlow{
	struct Edge{
		cost_t c;
		flow_t f;
		int to, rev;
		Edge(int _to, cost_t _c, flow_t _f, int _rev):c(_c), f(_f), to(_to), rev(_rev){}
	};
	static constexpr cost_t INFCOST = numeric_limits<cost_t>::max()/2;
	cost_t eps;
	int N, S, T;
	vector<vector<Edge> > G;
	vector<unsigned int> isq, cur;
	vector<flow_t> ex;
	vector<cost_t> h;
	mcSFlow(int _N, int _S, int _T):eps(0), N(_N), S(_S), T(_T), G(_N){}
	Edge add_edge(int a, int b, cost_t cost, flow_t cap){
		assert(cap>=0);
		assert(a>=0&&a<N&&b>=0&&b<N);
		assert(a!=b);
		cost*=N;
		eps = max(eps, abs(cost));
		G[a].emplace_back(b, cost, cap, G[b].size());
		Edge ret = G[a].back();
		G[b].emplace_back(a, -cost, 0, G[a].size()-1);
		return ret;
	}
	void add_flow(Edge& e, flow_t f) {
		Edge &back = G[e.to][e.rev];
		if (!ex[e.to] && f)
			hs[h[e.to]].push_back(e.to);
		e.f -= f; ex[e.to] += f;
		back.f += f; ex[back.to] -= f;
	}
	vector<vector<int> > hs;
	vector<int> co;
	// fast max flow, lowest label version
	flow_t max_flow() {
		ex.assign(N, 0);
		h.assign(N, 0); hs.resize(2*N);
		co.assign(2*N, 0); cur.assign(N, 0);
		h[S] = N;
		ex[T] = 1;
		co[0] = N-1;
		for(auto &e:G[S]) add_flow(e, e.f);
		if(hs[0].size())
			for (int hi = 0;hi>=0;) {
				int u = hs[hi].back();
				hs[hi].pop_back();
				while (ex[u] > 0) { // discharge u
					if (cur[u] == G[u].size()) {
						h[u] = 1e9;
						for(unsigned int i=0;i<G[u].size();++i){
							auto &e = G[u][i];
							if (e.f && h[u] > h[e.to]+1){
								h[u] = h[e.to]+1;
								cur[u] = i;
							}
						}
						if (++co[h[u]], !--co[hi] && hi < N)
							for(int i=0;i<N;++i){
								if (hi < h[i] && h[i] < N){
									--co[h[i]];
									h[i] = N + 1;
								}
							}
						hi = h[u];
					} else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to]+1){
						add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
					} else ++cur[u];
				}
				while (hi>=0 && hs[hi].empty()) --hi;
			}
		return -ex[S];
	}
	// begin min cost flow
	bool look_ahead(int u){
		if(ex[u]) return false;
		cost_t newHeight = h[u]-N*eps;
		for(auto const&e:G[u]){
			if(e.f == 0) continue;
			if(h[u] + e.c - h[e.to] < 0) return false; // outgoing admissible arc
			else newHeight = max(newHeight, h[e.to] - e.c); // try to make arc admissible
		}
		h[u] = newHeight - eps;
		return true;
	}
	void push(Edge &e, flow_t amt){
		if(e.f < amt) amt=e.f;
		e.f-=amt; ex[e.to]+=amt;
		G[e.to][e.rev].f+=amt; ex[G[e.to][e.rev].to]-=amt;
	}
	void relabel(int vertex){
		cost_t newHeight = -INFCOST;
		for(unsigned int i=0;i<G[vertex].size();++i){
			Edge const&e = G[vertex][i];
			if(e.f && newHeight < h[e.to]-e.c){
				newHeight = h[e.to] - e.c;
				cur[vertex] = i;
			}
		}
		h[vertex] = newHeight - eps;
	}
	static constexpr int scale=2;
	template<bool use_look_ahead = true>
		pair<flow_t, cost_t> minCostMaxFlow(){
			cost_t retCost = 0;
			for(int i=0;i<N;++i)
				for(Edge &e:G[i])
					retCost += e.c*(e.f);
			// remove this for circulation
			flow_t retFlow = max_flow();
			h.assign(N, 0); ex.assign(N, 0);
			isq.assign(N, 0); cur.assign(N,0);
			stack<int> q;
			for(;eps;eps>>=scale){
				fill(cur.begin(), cur.end(), 0);
				for(int i=0;i<N;++i)
					for(auto &e:G[i])
						if(h[i] + e.c - h[e.to] < 0 && e.f)
							push(e, e.f);
				for(int i=0;i<N;++i){
					if(ex[i]>0){
						q.push(i);
						isq[i]=1;
					}
				}
				while(!q.empty()){
					int u=q.top();q.pop();
					isq[u]=0;
					while(ex[u]>0){
						if(cur[u] == G[u].size())
							relabel(u);
						for(unsigned int &i=cur[u], max_i = G[u].size();i<max_i;++i){
							Edge &e=G[u][i];
							if(e.f == 0) continue;
							if(h[u] + e.c - h[e.to] < 0){
								if(use_look_ahead && look_ahead(e.to)){
									--i;
									continue;
								}
								push(e, ex[u]);
								if(isq[e.to]==0){
									q.push(e.to);
									isq[e.to]=1;
								}
								if(ex[u]==0) break;
							}
						}
					}
				}
				if(eps>1 && eps>>scale==0){
					eps = 1<<scale;
				}
			}
			for(int i=0;i<N;++i){
				for(Edge &e:G[i]){
					retCost -= e.c*(e.f);
				}
			}
			return make_pair(retFlow, retCost/2/N);
		}
	flow_t getFlow(Edge const &e){
		return G[e.to][e.rev].f;
	}
};


int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int S = N, T = N+1;
	MCMF_SSPA<ll, ll> mcmf(N+2);
	//mcSFlow<ll, ll> mcs(N+2, S, T);
	auto addEdge =[&](int u, int v, ll cap, ll cost) {
		mcmf.addEdge(u, v, cap, cost);
		//mcs.add_edge(u, v, cost, cap);
	};
	ll totA = 0;
	ll totL = 0;
	ll totR = 0;
	for (int i = 0; i < N; i++) {
		ll a, l, r; cin >> a >> l >> r;
		totA += a;
		totL += l;
		totR += r;
		assert(l <= r);
		ll base = min(a, l);
		a -= base, l -= base, r -= base;
		if (a) addEdge(S, i, a, 0);
		if (l) addEdge(i, T, l, 0);
		if (r - l) addEdge(i, T, r-l, N+1);
	}

	assert(totL <= totA && totA <= totR);

	for (int i = 0; i < N; i++) {
		addEdge(i, (i+1)%N, totA, 1);
		addEdge((i+1)%N, i, totA, 1);
	}

	cerr << "starting flow\n";
	ll flowCost = mcmf.maxflow(S, T).second;
	//ll flowCost = mcs.minCostMaxFlow().second;

	cout << flowCost - (totA - totL) * (N+1) << '\n';

	return 0;
}
