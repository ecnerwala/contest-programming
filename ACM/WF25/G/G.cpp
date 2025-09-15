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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

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
		std::vector<int> is_outside; is_outside.reserve(6 * N);
		auto get_edge_id = [&](int a, int b) -> int {
			int v;
			if (auto it = edge_ids.find(b * N + a); it != edge_ids.end()) {
				v = it->second ^ 1;
			} else {
				v = E;
				E += 2;
				edges.push_back({a, b});
				is_outside.push_back(0);
				edges.push_back({b, a});
				if (pts[a].p[1] == pts[b].p[1] && pts[a].p[1] == 0) is_outside.push_back(1); // bottom
				else if (pts[a].p[0] == pts[b].p[0] && pts[a].p[0] == MAXX) is_outside.push_back(2); // right
				else if (pts[a].p[1] == pts[b].p[1] && pts[a].p[1] == MAXY) is_outside.push_back(3); // top
				else if (pts[a].p[0] == pts[b].p[0] && pts[a].p[0] == 0) is_outside.push_back(4); // left
				else is_outside.push_back(0);
			}
			auto [_, did_insert] = edge_ids.insert({a * N + b, v});
			assert(did_insert);

			return v;
		};

		std::vector<std::vector<int>> vert_triangles(N);

		for (auto [t_id, t] : std::views::enumerate(tris)) {
			// t is a reference
			auto [a, b, c] = t.vs;
			std::array<std::tuple<int, int, int>, 3> tri_edges{
				std::tuple<int, int, int>{pts[a].h, a, get_edge_id(b, c)},
				std::tuple<int, int, int>{pts[b].h, b, get_edge_id(c, a)},
				std::tuple<int, int, int>{pts[c].h, c, get_edge_id(a, b)},
			};
			std::sort(tri_edges.begin(), tri_edges.end());
			t.es[0] = std::get<2>(tri_edges[2]);
			t.es[1] = std::get<2>(tri_edges[1]);
			t.es[2] = std::get<2>(tri_edges[0]);
			t.v = std::get<1>(tri_edges[1]);
			t.h = std::get<0>(tri_edges[0]);
			vert_triangles[t.v].push_back(int(t_id));
		}

		cerr << "hi" << '\n';
		int start_edge = 0;
		while (is_outside[start_edge] != 4) start_edge++;

		int end_edge = 0;
		while (is_outside[end_edge] != 2) start_edge++;
		cerr << "hi" << '\n';

		int h_min = std::min(pts[corners[0][0]].h, pts[corners[0][1]].h);
		int h_max = std::max(pts[corners[0][0]].h, pts[corners[0][1]].h);

		// We can just maintain which edges are currently in the path
		std::vector<bool> in_path(2*E);

		std::vector<int> tri_next(E, -1);
		for (auto t : tris) {
			tri_next[t.es[0]] = t.es[1] ^ 1;
			tri_next[t.es[1]] = t.es[0] ^ 1;
		}

		// We maintain the linked list of edges on our current path
		std::vector<int> prv(E); std::ranges::iota(prv, 0);
		std::vector<int> nxt(E); std::ranges::iota(nxt, 0);
	}

	return 0;
}
