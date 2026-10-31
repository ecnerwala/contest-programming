#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	using std::cin, std::cerr, std::cout;

	int N; cin >> N;
	struct pt_t {
		int x, y;
	};
	struct circ_t {
		pt_t p; int r;
	};
	std::vector<circ_t> C(N);
	for (auto& c : C) {
		cin >> c.p.x >> c.p.y >> c.r;
	}

	struct evt_t {
		int x;
		int i;
		int y;
	};
	std::vector<evt_t> evts;
	evts.reserve(2*N);
	for (int i = 0; i < N; i++) {
		evts.push_back({std::max(0, C[i].p.x - C[i].r), ~i, C[i].p.y});
		evts.push_back({std::min(int(1e9), C[i].p.x + C[i].r), i, C[i].p.y});
	}
	std::sort(evts.begin(), evts.end(), [&](auto a, auto b) {
		return std::tie(a.x, a.i) < std::tie(b.x, b.i);
	});

	std::vector<std::array<pt_t, 2>> ans; ans.reserve(N-1);
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	std::map<int, int> alive;

	pt_t last_pt{-1, -1};
	int last_idx = -1;

	for (int z = 0, w = 0; z < int(evts.size()); z = w) {
		assert(w == z);
		if (evts[z].i < 0) {
			int x = evts[z].x;
			while (w < int(evts.size()) && evts[w].x == x && evts[w].i < 0) {
				int y = evts[w].y;
				int i = ~evts[w].i;
				if (alive.empty() && last_idx != -1) {
					bool did_merge = merge(last_idx, i);
					assert(did_merge);
					ans.push_back({last_pt, {x, y}});
				}
				auto [_, did_insert] = alive.insert({y, i});
				assert(did_insert);
				w++;
			}
			for (int a = z; a < w; a++) {
				int y = evts[a].y;
				int i = ~evts[a].i;
				auto it = alive.find(y);
				assert(it->second == i);
				if (it != alive.begin()) {
					auto pit = std::prev(it);
					if (merge(pit->second, i)) {
						ans.push_back({pt_t{x, pit->first}, {x, it->first}});
					}
				}
				auto nit = std::next(it);
				if (nit != alive.end()) {
					if (merge(i, nit->second)) {
						ans.push_back({pt_t{x, it->first}, {x, nit->first}});
					}
				}
			}
		} else {
			int num_erased = int(alive.erase(evts[z].y));
			assert(num_erased == 1);
			last_pt = {evts[z].x, evts[z].y};
			last_idx = evts[z].i;
			w++;
		}
	}

	assert(int(ans.size()) == N-1);
	cout << "YES" << '\n';
	for (auto [a, b] : ans) {
		cout << a.x << ' ' << a.y << ' ' << b.x << ' ' << b.y << '\n';
	}
}
