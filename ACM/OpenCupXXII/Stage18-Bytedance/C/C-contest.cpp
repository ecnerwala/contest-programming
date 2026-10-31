#include <bits/stdc++.h>

namespace dacin21 {
using namespace std;
using ll = long long;
template<typename flow_t = ll, typename cost_t = ll> struct mcSFlow{
	struct Edge{
		cost_t c;
		flow_t f;
		int to, rev;
		Edge(int _to, cost_t _c, flow_t _f, int _rev):c(_c), f(_f), to(_to), rev(_rev){}
	};
	static constexpr cost_t INFCOST = numeric_limits<cost_t>::max()/2;
	cost_t eps;
	int N;
	//int N, S, T;
	vector<vector<Edge> > G;
	vector<unsigned int> isq, cur;
	vector<flow_t> ex;
	vector<cost_t> h;
	mcSFlow(int _N):eps(0), N(_N), G(_N){}
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
		cost_t minCostMaxFlow(){
			cost_t retCost = 0;
			for(int i=0;i<N;++i)
				for(Edge &e:G[i])
					retCost += e.c*(e.f);
			// remove this for circulation
			//flow_t retFlow = max_flow();
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
			return retCost/2/N;
			//return make_pair(retFlow, retCost/2/N);
		}
	flow_t getFlow(Edge const &e){
		return G[e.to][e.rev].f;
	}
};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::pair<std::array<int, 2>, int>> edges(M);
	for (auto& [x, w] : edges) {
		cin >> x[0] >> x[1] >> w; x[0]--, x[1]--; x[1] += N;
	}
	dacin21::mcSFlow flower(2*N+2);
	int SOURCE = 2*N;
	int SINK = 2*N+1;
	for (int i = 0; i < N; i++) {
		flower.add_edge(SOURCE, i, 0, 1);
		flower.add_edge(i+N, SINK, 0, 1);
	}

	for (auto [x, w] : edges) {
		flower.add_edge(x[0], x[1], -w, 1);
	}

	std::vector<int> vals_at_cost(6);
	for (int w = 4; w >= 0; w--) {
		const auto& e = flower.add_edge(SINK, SOURCE, w, N);
		//flower.eps = std::max<int64_t>(flower.eps, 5*N);
		flower.eps = N;
		int val = int(-flower.minCostMaxFlow<false>());
		vals_at_cost[w] = val;
		//cerr << w << ' ' << vals_at_cost[w] << '\n';
	}

	std::vector<int> deltas(N);
	for (int w = 0; w <= 4; w++) {
		for (int i = 0; i < vals_at_cost[w]; i++) deltas[i]++;
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += deltas[i];
		cout << ans << '\n';
	}

	return 0;
}
