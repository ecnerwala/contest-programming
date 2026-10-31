#include <bits/extc++.h>

template <typename flow_t = int, typename cost_t = long long>
struct MCMF_SSPA {
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

	int addEdge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		assert(cost + pi[from] - pi[to] >= 0); // TODO: Remove this restriction
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
		return e;
	}

	const cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<cost_t> dist;
	__gnu_pbds::priority_queue<std::pair<cost_t, int>> q;
	std::vector<typename decltype(q)::point_iterator> its;
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

	std::pair<flow_t, cost_t> maxflow(int s, int t) {
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (path(s), pi[t] < INF_COST) {
			flow_t curFlow = std::numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = std::min(curFlow, edges[e].cap);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	struct constraint_t {
		int64_t w;
		int x1, y1, x2, y2;
	};
	std::vector<constraint_t> constraints(K);

	std::vector<int64_t> horiz_cost((N+1)*(M+1));
	std::vector<int64_t> vert_cost((N+1)*(M+1));
	for (auto& c : constraints) {
		cin >> c.w >> c.x1 >> c.y1 >> c.x2 >> c.y2;
		if (c.x1 + c.y1 > c.x2 + c.y2) {
			std::swap(c.x1, c.x2);
			std::swap(c.y1, c.y2);
		}
		if (c.x1 == c.x2) {
			assert(c.y2 == c.y1 + 1);
			horiz_cost[c.x1 * (M+1) + c.y1] += c.w;
		} else if (c.y1 == c.y2) {
			assert(c.x2 == c.x1 + 1);
			vert_cost[c.x1 * (M+1) + c.y1] += c.w;
		}
	}

	const bool is_odd = ((N+1) * (M+1)) & 1;
	const int GRID = (N+1)*(M+1) + is_odd;
	const int BORDER = GRID+0;
	const int S = GRID+1;
	const int T = GRID+2;
	const int V = GRID+3;
	MCMF_SSPA<int, int64_t> flower(V);

	const int64_t FLOW_CONST = 5.01e12;

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			if ((i ^ j) & 1) {
				flower.addEdge(i*(M+1)+j, T, 1, 0);
				if (i == 0 || i == N || j == 0 || j == M) {
					flower.addEdge(BORDER, i*(M+1)+j, 1, 0);
				}
			} else {
				flower.addEdge(S, i*(M+1)+j, 1, 0);
				if (i == 0 || i == N || j == 0 || j == M) {
					flower.addEdge(i*(M+1)+j, BORDER, 1, FLOW_CONST);
				}
			}
		}
	}

	if (is_odd) {
		flower.addEdge(GRID-1, T, 1, 0);
		flower.addEdge(BORDER, GRID-1, 1, 0);
	}

	std::vector<int> horiz_edges((N+1)*(M+1));
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j+1 <= M; j++) {
			int a = i*(M+1)+j;
			int b = (i-1)*(M+1)+j;
			if ((i ^ j) & 1) std::swap(a, b);
			horiz_edges[i*(M+1)+j] = flower.addEdge(a, b, 1, FLOW_CONST-horiz_cost[i*(M+1)+j]);
		}
	}
	std::vector<int> vert_edges((N+1)*(M+1));
	for (int i = 1; i+1 <= N; i++) {
		for (int j = 1; j <= M; j++) {
			int a = i*(M+1)+j;
			int b = i*(M+1)+(j-1);
			if ((i ^ j) & 1) std::swap(a, b);
			vert_edges[i*(M+1)+j] = flower.addEdge(a, b, 1, FLOW_CONST-vert_cost[i*(M+1)+j]);
		}
	}

	auto [flow, cost] = flower.maxflow(S, T);
	assert(flow == GRID / 2);
	int64_t ans = flow * FLOW_CONST - cost;
	cout << ans << '\n';
	int v = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1, cur = v; j <= M; j++) {
			cout << "ABC"[cur];
			if (j+1 <= M) {
				if (flower.edges[horiz_edges[i*(M+1)+j]].cap > 0) {
					if ((i^j) & 1) {
						cur += 1;
					} else {
						cur += 2;
					}
					cur %= 3;
				}
			}
		}
		cout << '\n';
		if (i+1 <= N) {
			int j = 1;
			if (flower.edges[vert_edges[i*(M+1)+j]].cap > 0) {
				if ((i^j) & 1) {
					v += 2;
				} else {
					v += 1;
				}
				v %= 3;
			}
		}
	}

	return 0;
}
