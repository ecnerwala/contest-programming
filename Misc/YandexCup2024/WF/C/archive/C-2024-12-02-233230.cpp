#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	struct city_t {
		int a, b, c;
	};
	std::vector<city_t> cities(N);
	for (auto& [a, b, c] : cities) cin >> a >> b >> c;

	std::vector<std::vector<int>> nomes(M);
	for (auto& r : nomes) {
		int k; cin >> k;
		r.resize(k);
		for (auto& v : r) { cin >> v; v--; }
	}

	auto solve = [&]() -> std::optional<std::vector<std::pair<int, int>>> {
		std::vector<std::pair<int, int>> nome_bounds(M);

		for (int j = 0; j < M; j++) {
			nome_bounds[j] = {cities[nomes[j][0]].a, cities[nomes[j][0]].a};
			for (auto v : nomes[j]) {
				int a = cities[v].a;
				nome_bounds[j].first = std::max(nome_bounds[j].first, a);
				nome_bounds[j].second = std::min(nome_bounds[j].second, a);
			}
		}
		for (int j = 0; j+2 < M; j++) {
			if (nome_bounds[j].second <= nome_bounds[j+2].first) {
				return std::nullopt;
			}
		}
		std::vector<std::pair<int, int>> banned; banned.reserve(2*N);
		for (int j = M-2; j >= 0; j--) {
			if (nome_bounds[j].second < nome_bounds[j+1].first) {
				if (!banned.empty() && banned.back().first == nome_bounds[j].second) {
					banned.pop_back();
				} else {
					banned.push_back({nome_bounds[j].second, true});
				}
				banned.push_back({nome_bounds[j+1].first, false});
			}
		}

		struct spec_t {
			std::optional<std::pair<int, int>> musts;
			std::vector<std::pair<int, int>> cants;
		};
		std::map<int, spec_t> specs;
		for (int j = 0; j < M; j++) {
			for (auto v : nomes[j]) {
				auto [a, b, c] = cities[v];
				if (j > 0 && a >= nome_bounds[j-1].second) {
					// must take specialization c between b and a
					auto& m = specs[c].musts;
					if (!m) {
						m = {a, b};
					} else {
						m->first = std::max(m->first, a);
						m->second = std::min(m->second, b);
					}
				}
				if (j+1 < M && a <= nome_bounds[j+1].first) {
					// must not take
					if (a < b) {
						specs[c].cants.push_back({a, b});
					}
				}
			}
		}
		std::vector<std::pair<int, int>> res; res.reserve(3*N);
		for (auto& [c, s] : specs) {
			if (s.musts) {
				auto [ml, mr] = *s.musts;
				if (ml >= mr) return std::nullopt;
				std::vector<std::pair<int, int>> evts; evts.reserve(s.cants.size() * 2 + 2);
				evts.push_back({ml, 0});
				for (auto [l, r] : s.cants) {
					if (r <= ml) continue;
					if (mr <= l) continue;
					evts.push_back({std::max(l, ml), +1});
					evts.push_back({std::min(r, mr), -1});
				}
				evts.push_back({mr, 0});
				std::sort(evts.begin() + 1, evts.end() - 1);
				bool found = false;
				int pref = 0;
				for (int z = 0; z+1 < int(evts.size()); z++) {
					pref += evts[z].second;
					if (pref == 0 && evts[z].first < evts[z+1].first) {
						auto it = std::lower_bound(banned.begin(), banned.end(), std::pair<int, int>{evts[z+1].first, false});
						if (it == banned.end() || it->second) {
							res.push_back({evts[z+1].first, c});
							found = true;
							break;
						} else if (evts[z].first < std::prev(it)->first) {
							res.push_back({std::prev(it)->first, c});
							found = true;
							break;
						}
					}
				}
				if (!found) return std::nullopt;
			}
		}

		int nxt_spec = 1;
		for (auto [a, b, c] : cities) {
			auto it = std::lower_bound(banned.begin(), banned.end(), std::pair<int, int>{a, false});
			if (it == banned.end() || it->second) {
				// allowed, insert two a's
				for (int z = 0; z < 2; z++) {
					while (specs.count(nxt_spec)) {
						nxt_spec++;
					}
					res.push_back({a, nxt_spec});
					nxt_spec++;
				}
			}
		}
		return res;
	};
	auto res = solve();
	if (res) {
		cout << res->size() << '\n';
		for (auto [d, s] : *res) cout << d << ' ' << s << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
