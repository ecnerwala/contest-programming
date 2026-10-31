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
			std::array<int, 3> es; // low, long, hi
		};
		std::vector<tri_t> tris(M);
		for (auto& t : tris) { for (auto& v : t.vs) { cin >> v; v--; } }

		hash_map<int64_t, int> edge_ids;
		int E = 0;
		auto get_edge_id = [&](int a, int b) -> int {
			int v;
			if (auto it = edge_ids.find(b * N + a); it != edge_ids.end()) {
				v = it->second ^ 1;
			}
			else {
				v = E;
				E += 2;
			}
			auto [_, did_insert] = edge_ids.insert({a * N + b, v});
			assert(did_insert);
			return v;
		};
		int start_edge = get_edge_id(corners[0][0], corners[0][1]);
		int end_edge = get_edge_id(corners[1][1], corners[1][0]);
		for (auto t : tris) {
			auto [a, b, c] = t.vs;
			std::array<std::tuple<int, int, int>, 3> tri_edges{
				std::tuple<int, int, int>{pts[a].h, a, get_edge_id(b, c)},
				std::tuple<int, int, int>{pts[b].h, b, get_edge_id(c, a)},
				std::tuple<int, int, int>{pts[c].h, c, get_edge_id(a, b)},
			};
			std::ranges::sort(tri_edges);
			t.es[0] = std::get<2>(tri_edges[2]);
			t.es[1] = std::get<2>(tri_edges[1]);
			t.es[2] = std::get<2>(tri_edges[0]);
			t.v = std::get<1>(tri_edges[1]);
		}
		std::vector<std::array<int, 2>> edges;
	}

	return 0;
}
