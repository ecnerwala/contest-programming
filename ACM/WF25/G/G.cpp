#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	using std::cin, std::cout, std::cerr;

	int T;
	cin >> T;
	while (T--) {
		int MAXX, MAXY, N, M;
		cin >> MAXX >> MAXY >> N >> M;
		struct point_t {
			int h;
			std::array<int, 2> p;
		};
		std::vector<point_t> pts(N);
		for (auto& p : pts) {
			cin >> p.p[0] >> p.p[1] >> p.h;
		}

		std::vector<std::pair<int, int>> by_height(N);
		for (int i = 0; i < N; i++) by_height[i] = {pts[i].h, i};
		std::ranges::sort(by_height);

		std::array<std::array<int, 2>, 2> corners{std::array<int, 2>{-1, -1}, {-1, -1}};
		for (int i = 0; i < N; i++) {
			auto [x, y] = pts[i].p;
			if (x == 0 || x == MAXX) {
				assert(y == 0 || y == MAXY);
				corners[x == MAXX][y == MAXY] = i;
			}
		}
		for (int x = 0; x < 2; x++) for (int y = 0; y < 2; y++) { assert(corners[x][y] != -1); }

		struct tri_t {
			std::array<int, 3> vs;
			int v; // middle vertex
			int h; // height
			std::array<int, 3> es; // low, long, hi
		};
		std::vector<tri_t> tris(M);
		for (auto& t : tris) { for (auto& v : t.vs) { cin >> v; v--; } }

		hash_map<int64_t, int> edge_ids;
		int E = 0;
		std::vector<std::array<int, 2>> edges; edges.reserve(6 * N);
		std::vector<int> edge_tri; edge_tri.reserve(6 * N);
		auto get_edge_id = [&](int a, int b, int t) -> int {
			int v;
			if (auto it = edge_ids.find(b * N + a); it != edge_ids.end()) {
				v = it->second ^ 1;
			} else {
				v = E;
				E += 2;
				edges.push_back({a, b});
				edge_tri.push_back(-5);
				edges.push_back({b, a});
				if (pts[a].p[1] == pts[b].p[1] && pts[a].p[1] == 0) edge_tri.push_back(-1); // bottom
				else if (pts[a].p[0] == pts[b].p[0] && pts[a].p[0] == MAXX) edge_tri.push_back(-2); // right
				else if (pts[a].p[1] == pts[b].p[1] && pts[a].p[1] == MAXY) edge_tri.push_back(-3); // top
				else if (pts[a].p[0] == pts[b].p[0] && pts[a].p[0] == 0) edge_tri.push_back(-4); // left
				else edge_tri.push_back(-5);
				if (edge_tri.back() != -5) {
					cerr << "have " << edge_tri.back() << '\n';
				}
			}
			auto [_, did_insert] = edge_ids.insert({a * N + b, v});
			assert(did_insert);
			assert(edge_tri[v] == -5);
			edge_tri[v] = t;

			cerr << "edge " << v << " in " << t << " is " << a << '-' << b << '\n';
			return v;
		};

		std::vector<std::vector<int>> vert_triangles(N);

		for (auto [t_id, t] : std::views::enumerate(tris)) {
			// t is a reference
			auto [a, b, c] = t.vs;
			std::array<std::tuple<int, int, int>, 3> tri_edges{
				std::tuple<int, int, int>{pts[a].h, a, get_edge_id(b, c, int(t_id))},
				std::tuple<int, int, int>{pts[b].h, b, get_edge_id(c, a, int(t_id))},
				std::tuple<int, int, int>{pts[c].h, c, get_edge_id(a, b, int(t_id))},
			};
			std::sort(tri_edges.begin(), tri_edges.end());
			t.es[0] = std::get<2>(tri_edges[2]);
			t.es[1] = std::get<2>(tri_edges[1]);
			t.es[2] = std::get<2>(tri_edges[0]);
			t.v = std::get<1>(tri_edges[1]);
			t.h = std::get<0>(tri_edges[0]);
			vert_triangles[t.v].push_back(int(t_id));
		}

		int left_edge = 0;
		while (edge_tri[left_edge] != -4) left_edge++;

		int right_edge = 0;
		while (edge_tri[right_edge] != -2) right_edge++;

		int h_min = pts[corners[0][0]].h;
		int h_max = pts[corners[0][1]].h;

		bool is_flipped = h_min > h_max;
		if (is_flipped) std::swap(h_min, h_max);

		// we're at h_cur + eps
		int h_cur = h_min;
		std::vector<int> nxt(E, -1);

		// We can just maintain which edges are currently in the path
		std::vector<bool> in_path(E);

		for (auto t : tris) {
			int d = h_cur >= t.h ? 0 : 2;
			cerr << "connecting " << t.es[d] << ' ' << t.es[1] << '\n';
			nxt[t.es[d]] = t.es[1] ^ 1;
			nxt[t.es[1]] = t.es[d] ^ 1;
		}

		{
			for (int cur = left_edge ^ 1; edge_tri[cur] >= 0; cur = nxt[cur]) {
				cerr << "visit " << cur << ' ' << edges[cur][0] << ' ' << edges[cur][1] << ' ' << edge_tri[cur] << ' ' << nxt[cur] << '\n';
				int t = edge_tri[cur];
				// get t's value
				in_path[cur] = true;
				assert(nxt[cur] != -1);
			}
		}
	}

	return 0;
}
