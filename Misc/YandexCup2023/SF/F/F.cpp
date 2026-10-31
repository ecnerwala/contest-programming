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

	int N; cin >> N;

	std::vector<int64_t> ans(N);

	// NOTE: we could do this as we go, but that's too much brain for 7am Andrew
	auto solve_tree = [&](const std::vector<std::pair<int, std::array<int, 2>>>& edges, int dir) {
		assert(std::is_sorted(edges.begin(), edges.end(), [&](auto a, auto b) { return a.first > b.first; }));

		// linearize first
		std::vector<std::pair<int, int>> nxt(N, {-1, -1});
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		std::vector<std::pair<int, int>> lst(N);
		for (int i = 0; i < N; i++) lst[i] = {i, i};
		auto merge = [&](int a, int b, int w) -> void {
			a = get_par(a), b = get_par(b);
			assert(a != b);
			nxt[lst[a].second] = {lst[b].first, w};
			std::pair<int, int> nlst = {lst[a].first, lst[b].second};
			if (par[a] > par[b]) std::swap(a, b);
			lst[a] = nlst;
			lst[b] = {-1, -1};
			par[a] += par[b];
			par[b] = a;
		};
		for (auto& [w, ends] : edges) {
			merge(ends[0], ends[1], w);
		}
		std::vector<int> weights; weights.reserve(N-1);
		std::vector<int> verts; verts.reserve(N);
		for (int cur = lst[get_par(0)].first; ; ) {
			verts.push_back(cur);
			if (nxt[cur].first == -1) break;
			weights.push_back(nxt[cur].second);
			cur = nxt[cur].first;
		}
		assert(int(weights.size()) == N-1);
		assert(int(verts.size()) == N);

		for (int z = 0; z < 2; z++, std::reverse(verts.begin(), verts.end()), std::reverse(weights.begin(), weights.end())) {
			std::vector<std::pair<int, int>> stk; stk.reserve(N);
			stk.push_back({-1, -1});
			int64_t cur_ans = 0;
			for (int i = 0; i < N; i++) {
				ans[verts[i]] += cur_ans * dir;
				if (i+1 < N) {
					// insert weights[i];
					int w = weights[i];
					while (w < stk.back().first) {
						auto [pw, pi] = stk.back(); stk.pop_back();
						assert(!stk.empty());
						cur_ans -= int64_t(pw) * int64_t(pi - stk.back().second);
					}
					assert(!stk.empty());
					cur_ans += int64_t(w) * int64_t(i - stk.back().second);
					stk.push_back({w, i});
				}
			}
		}
	};

	std::vector<std::pair<int, std::pair<int, std::array<int, 2>>>> all_edges; all_edges.reserve(2*(N-1));
	for (int z = 0; z < 2; z++) {
		std::vector<std::pair<int, std::array<int, 2>>> edges(N-1);
		for (auto& [w, ends] : edges) {
			cin >> ends[0] >> ends[1] >> w;
			ends[0]--, ends[1]--;
		}
		std::sort(edges.begin(), edges.end(), [&](auto a, auto b) { return a.first > b.first; });
		solve_tree(edges, +1);

		for (auto [w, ends] : edges) {
			all_edges.push_back({w, {z, ends}});
		}
	}

	std::sort(all_edges.begin(), all_edges.end(), [&](auto a, auto b) { return a.first > b.first; });
	std::vector<std::pair<int, std::array<int, 2>>> rect_edges; rect_edges.reserve(N-1);

	{
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};

		std::vector<std::array<int, 2>> coords(N);
		std::array<std::vector<std::unordered_map<int, int>>, 2> alives;
		alives[0].resize(N);
		alives[1].resize(N);
		for (int i = 0; i < N; i++) {
			coords[i] = {i, i};
			alives[0][coords[i][0]][coords[i][1]] = i;
			alives[1][coords[i][1]][coords[i][0]] = i;
		}
		for (auto& [w, z_ends] : all_edges) {
			auto [z, ends] = z_ends;
			int a = coords[get_par(ends[0])][z];
			int b = coords[get_par(ends[1])][z];
			if (alives[z][a].size() > alives[z][b].size()) std::swap(a, b);
			for (auto [k, v] : alives[z][a]) {
				assert(coords[v][z] == a);
				assert(coords[v][!z] == k);
				coords[v][z] = b;
				alives[!z][k].erase(a);
				auto [it, did_insert] = alives[z][b].insert({k, v});
				if (!did_insert) {
					rect_edges.push_back({w, {v, it->second}});
					assert(par[v] < 0 && par[it->second] < 0);
					par[v] = it->second;
				} else {
					alives[!z][k].insert({b, v});
				}
			}
			alives[z][a] = {};
		}
	}

	std::sort(rect_edges.begin(), rect_edges.end(), [&](auto a, auto b) { return a.first > b.first; });
	solve_tree(rect_edges, -1);

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
