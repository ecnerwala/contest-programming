#include <bits/extc++.h>

template <typename flow_t = int, typename cost_t = long long>
struct MCMF_SSPA {
	const int64_t BASE_COST = int64_t(1e12) + 100;

	int N;
	std::vector<std::vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	std::vector<edge_t> edges;

	std::vector<char> seen;
	std::vector<cost_t> pi;
	std::vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

	void addEdge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		assert(cost + pi[from] - pi[to] >= 0); // TODO: Remove this restriction
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	const cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<int64_t> dist;
	__gnu_pbds::priority_queue<std::pair<int64_t, int>> q;
	std::vector<typename decltype(q)::point_iterator> its;

	void path(int s) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		assert(q.empty());
		its.assign(N, q.end());
		for (int i = 0; i < N; i++) {
			its[i] = q.push({-(dist[i] - pi[i]), i});
		}

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

	std::pair<flow_t, cost_t> maxflow(int s, int t, flow_t mx, cost_t upper) {
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (totFlow < mx && (path(s), pi[t] < upper)) {
			flow_t curFlow = std::numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = std::min(curFlow, edges[e].cap);
				cur = nxt;
			}
			assert(curFlow == 1);
			curFlow = std::min(curFlow, mx - totFlow);
			totFlow += curFlow;
			totCost += (upper - pi[t]) * curFlow;
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

	std::pair<flow_t, cost_t> maxflowFast(int s, int t, flow_t mx, cost_t upper) {
		assert(mx == 1);
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (totFlow < mx && (pi[t] < upper)) {
			flow_t curFlow = std::numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = std::min(curFlow, edges[e].cap);
				cur = nxt;
			}
			curFlow = std::min(curFlow, mx - totFlow);
			assert(curFlow == 1);
			totFlow += curFlow;
			totCost += (upper - pi[t]) * curFlow;
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

	cost_t addNegEdgeFast(int from, int to, int cap, int64_t cost) {
		auto [flow, val] = maxflowFast(to, from, cap, -cost);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap - flow, cost});
		edges.emplace_back(edge_t{from, flow, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
		return val;
	}
	cost_t addNegEdge(int from, int to, int cap, int64_t cost) {
		auto [flow, val] = maxflow(to, from, cap, -cost);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap - flow, cost});
		edges.emplace_back(edge_t{from, flow, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
		return val;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;

	const int SOURCE = 0;
	const int SINK = 1;

	MCMF_SSPA<int, int64_t> flower(2+N+M);

	for (int k = 0; k < K; k++) {
		int a, b; int64_t c; cin >> a >> b >> c; a--, b--;
		a = 2+a;
		b = 2+N+b;
		flower.addEdge(a, b, 1, flower.BASE_COST - c);
	}
	for (int i = 0; i < N; i++) {
		flower.addEdge(SOURCE, 2+i, 1, 0);
		//flower.addEdge(SOURCE, 2+i, 1, flower.BASE_COST + 1);
	}
	for (int j = 0; j < M; j++) {
		//flower.addEdge(SOURCE, 2+N+j, 1, flower.BASE_COST + 1);
		flower.addEdge(2+N+j, SINK, 1, 0);
	}
	//flower.addEdge(SOURCE, SINK, 1, flower.BASE_COST + 1);

	int64_t orig = flower.addNegEdge(SINK, SOURCE, std::min(N, M) + 100, -flower.BASE_COST);
	//cerr << orig << '\n';
	flower.path(SINK);

	for (int k = 0; k < K; k++) {
		if (flower.edges[2*k].cap == 1) {
			// unused, we can just keep going
			cout << 0 << '\n';
		} else {
			int a = flower.edges[2*k+1].dest;
			int b = flower.edges[2*k].dest;
			int64_t c = flower.BASE_COST - flower.edges[2*k].cost;

			MCMF_SSPA<int, int64_t> flower2 = flower;
			//cerr << a << ' ' << b << '\n';
			int64_t res = 0;
			res += flower2.addNegEdgeFast(b, SINK, 1, 0);
			res += flower2.addNegEdge(SOURCE, a, 1, 0);
			cout << c - res << '\n';
		}
	}

	return 0;
}
