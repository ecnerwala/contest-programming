#include <bits/extc++.h>

template <typename flow_t = int, typename cost_t = int64_t>
struct MCMF_Dinic {
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

	explicit MCMF_Dinic(int N_) : N(N_), adj(N), pi(N, 0) {}

	void add_edge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		assert(cost + pi[from] - pi[to] >= 0); // TODO: Remove this restriction
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
	static constexpr flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<cost_t> dist;
	__gnu_pbds::priority_queue<std::pair<cost_t, int>> q;
	std::vector<typename decltype(q)::point_iterator> its;
	cost_t dijkstra(int s, int t) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({-(dist[s] - pi[s]), s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		std::swap(pi, dist);
		return pi[t];
	}

	std::vector<int> buf;
	std::vector<int> level;
	flow_t dinic_dfs(int cur, int t, flow_t f) {
		if (cur == t) return f;
		flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur]) {
				flow_t v = dinic_dfs(nxt, t, std::min(f, edges[e].cap));
				edges[e].cap -= v;
				edges[e^1].cap += v;
				f -= v;
				cur_f += v;
				if (f == 0) break;
			}
		}
		return cur_f;
	}
	flow_t dinic(int s, int t) {
		flow_t tot_flow = 0;
		while (true) {
			buf.clear();
			buf.reserve(N);
			level.assign(N, -1);
			buf.push_back(s);
			level[s] = 0;
			for (int z = 0; z < int(buf.size()); z++) {
				int cur = buf[z];
				for (int e : adj[cur]) {
					int nxt = edges[e].dest;
					if (edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur] && level[nxt] == -1) {
						level[nxt] = level[cur] + 1;
						buf.push_back(nxt);
					}
				}
			}
			if (level[t] == -1) break;
			buf.assign(N, 0);
			tot_flow += dinic_dfs(s, t, INF_FLOW);
		}
		return tot_flow;
	}

	std::vector<std::pair<flow_t, cost_t>> all_flows(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		std::vector<std::pair<flow_t, cost_t>> res;
		while (dijkstra(s, t) <= max_cost) {
			assert(res.empty() || pi[t] > res.back().second);
			flow_t f = dinic(s, t);
			res.push_back({f, pi[t]});
		}
		return res;
	}

	std::pair<flow_t, cost_t> max_flow(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		flow_t tot_flow = 0; cost_t tot_cost = 0;
		while (dijkstra(s, t) <= max_cost) {
			int cur_flow = dinic(s, t);
			tot_flow += cur_flow;
			tot_cost += cur_flow * pi[t];
		}
		return {tot_flow, tot_cost};
	}
};

template <typename T> struct Point3D {
	using P = Point3D;

	T x, y, z;
	Point3D() : x(0), y(0), z(0) {}
	Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	template <typename U> explicit Point3D(const Point3D<U>& p) : x(T(p.x)), y(T(p.y)), z(T(p.z)) {}

	friend std::istream& operator >> (std::istream& i, P& p) { return i >> p.x >> p.y >> p.z; }
	friend std::ostream& operator << (std::ostream& o, const P& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }

	friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
	friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

	P& operator += (const P& o) { x += o.x, y += o.y, z += o.z; return *this; }
	P& operator -= (const P& o) { x -= o.x, y -= o.y, z -= o.z; return *this; }
	friend P operator + (const P& a, const P& b) { return P(a) += b; }
	friend P operator - (const P& a, const P& b) { return P(a) -= b; }

	P& operator *= (const T& t) { x *= t, y *= t, z *= t; return *this; }
	P& operator /= (const T& t) { x /= t, y /= t, z /= t; return *this; }
	friend P operator * (const P& p, const T& t) { return P(p) *= t; }
	friend P operator * (const T& t, const P& p) { return P(p) *= t; }
	friend P operator / (const P& a, const T& t) { return P(a) /= t; }

	friend P operator + (const P& a) { return P(+a.x, +a.y, +a.z); }
	friend P operator - (const P& a) { return P(-a.x, -a.y, -a.z); }

	friend T dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	friend T norm(const P& a) { return dot(a,a); }
	friend P cross(const P& a, const P& b) { return P(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	friend T int_norm(const P& p) {
		return gcd(gcd(abs(p.x), abs(p.y)), abs(p.z));
	}
	friend P int_unit(const P& p) {
		T g = int_norm(p);
		return g ? p / g : p;
	}

	friend T abs(const P& a) { return sqrt(std::max(T(0), norm(a))); }
	friend P unit(const P& a) { return a / abs(a); }

	friend T vol(const P& a, const P& b, const P& c, const P& d) { return dot(cross(b-a, c-a), d-a); }

	friend bool lexLess(const P& a, const P& b) { return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z); }

	friend bool parallelSame(const P& a, const P& b) {
		assert(a != P());
		assert(b != P());
		return lexLess(a, P()) == lexLess(b, P());
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	using pt_t = Point3D<int64_t>;
	struct drone_t {
		pt_t pos, dir;
	};
	std::vector<drone_t> drones(N);
	for (auto& [pos, dir] : drones) {
		cin >> pos >> dir;
	}

	auto solve = [&]() -> int {
		auto is_parallel = [&](pt_t a, pt_t b) -> bool {
			return cross(a, b) == pt_t(0, 0, 0);
		};
		auto signum = [&](pt_t p) {
			if (p.x) return (p.x > 0) - (p.x < 0);
			if (p.y) return (p.y > 0) - (p.y < 0);
			if (p.z) return (p.z > 0) - (p.z < 0);
			return 0;
		};
		auto same_dir = [&](pt_t a, pt_t b) -> bool {
			if (!is_parallel(a, b)) return false;
			return signum(a) == signum(b);
		};
		assert(N >= 1);
		if (N == 1) {
			return -1;
		}
		// Now, let's just check everything
		{
			bool is_line = true;
			pt_t d_pos = drones[1].pos - drones[0].pos;
			for (int i = 1; i < N; i++) {
				if (!is_parallel(drones[i].pos - drones[0].pos, d_pos)) {
					is_line = false;
					break;
				}
			}
			if (is_line) {
				if (N % 2 == 0) {
					int ans = 0;
					for (int i = 0, j = 1; i < N; i += 2, j += 2) {
						ans += !same_dir(drones[j].pos - drones[i].pos, drones[i].dir);
						ans += !same_dir(drones[i].pos - drones[j].pos, drones[j].dir);
					}
					return ans;
				}
				int ans = N+1;
				for (int z = 0; z < 2; z++, d_pos = -d_pos) {
					std::sort(drones.begin(), drones.end(), [&](const auto& a, const auto& b) -> bool {
						return dot(d_pos, a.pos) < dot(d_pos, b.pos);
					});
					// check them
					std::vector<bool> is_left(N);
					std::vector<bool> is_right(N);
					for (int i = 0; i < N; i++) {
						if (is_parallel(drones[i].dir, d_pos)) {
							if (dot(drones[i].dir, d_pos) > 0) {
								is_right[i] = true;
							} else {
								is_left[i] = true;
							}
						}
					}
					std::vector<int> pref_left(N+1);
					std::vector<int> pref_right(N+1);
					for (int i = 0; i < N; i++) {
						pref_left[i+1] = pref_left[i] + is_left[i];
						pref_right[i+1] = pref_right[i] + is_right[i];
					}

					for (int i = 0; i < N; i++) {
						// try to remove this guy
						int left_cost = 0;
						int left_cnt = 0;
						for (int j = 0; j < N; j++) {
							if (j == i) continue;
							if ((left_cnt & 1) == 0) {
								int cost = 0;
								int mid_cnt = 1;
								int mid_cost = 0;
								int right_cost = N+1;
								for (int k = j+1; k < N; k++) {
									if (k == i) continue;
									if (mid_cnt & 1) {
										mid_cost += !is_left[k];
										right_cost += !is_left[k];
										right_cost = std::min(right_cost, mid_cost);
									} else {
										mid_cost += !is_left[k];
										right_cost += !is_right[k];
									}
									mid_cnt++;
								}
								assert((mid_cnt & 1) == 0);
								cost = left_cost + right_cost;

								pt_t d1 = drones[i].dir;
								pt_t d2 = drones[j].dir;
								pt_t d3 = -d_pos;
								pt_t a = cross(d1, d2);
								pt_t b = cross(d2, d3);
								pt_t c = cross(d3, d1);
								if (a == pt_t() && b == pt_t() && c == pt_t()) {
									cost += 2;
								} else if (a == pt_t() || b == pt_t() || c == pt_t()) {
									cost++;
								} else if (dot(d3, a) != 0) {
									// not coplanar
									cost++;
								} else {
									// a, b, c should all be the same direction, just check if their sign is the same
									int sa = signum(a), sb = signum(b), sc = signum(c);
									if (sa == sb && sb == sc) {
									} else {
										cost++;
									}
								}

								ans = std::min(ans, cost);
							}

							if (left_cnt & 1) {
								left_cost += !is_left[j];
							} else {
								left_cost += !is_right[j];
							}
							left_cnt ++;
						}
					}
				}

				assert(ans <= N);
				return ans + 1000;
			}
		}

		MCMF_Dinic<int, int> flower(2*N+2);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (j == i) continue;
				pt_t dir = drones[j].pos - drones[i].pos;
				int64_t n = dot(dir, dir);
				bool bad = false;
				for (int k = 0; k < N; k++) {
					if (k == i || k == j) continue;
					pt_t o_dir = drones[k].pos - drones[i].pos;
					if (cross(dir, o_dir) == pt_t()) {
						int64_t d = dot(dir, o_dir);
						if (0 <= d && d <= n) {
							bad = true;
							break;
						}
					}
				}
				if (bad) continue;
				if (same_dir(dir, drones[i].dir)) {
					flower.add_edge(i, N+j, 1, 0);
				} else {
					flower.add_edge(i, N+j, 1, 1);
				}
			}
		}
		for (int i = 0; i < N; i++) {
			flower.add_edge(2*N, i, 1, 0);
			flower.add_edge(N+i, 2*N+1, 1, 0);
		}
		auto [flow, cost] = flower.max_flow(2*N, 2*N+1);
		assert(flow == N);
		return cost;
	};
	cout << solve() << '\n';

	return 0;
}
