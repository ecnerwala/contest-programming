#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	{
		const int V = 100;
		std::vector<int> max_parittion_ways(V+1, 1);
		for (int a = 1; a <= V; a++) {
			for (int n = V; n >= 0; n--) {
				for (int c = 1; a * c <= n; c++) {
					max_parittion_ways[n] = std::max(max_parittion_ways[n], max_parittion_ways[n - a * c] * (c + 1));
				}
			}
		}
		for (int n = 0; n <= V; n++) { cerr << n << ' ' << max_parittion_ways[n] << '\n'; }
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			merge(u, v);
		}
		std::vector<int> cnt_freq(N+1, 0);
		for (int i = 0; i < N; i++) {
			if (par[i] < 0) cnt_freq[-par[i]] ++;
		}
		std::vector<std::pair<int, int>> cnts; cnts.reserve(N);
		for (int i = 0; i <= N; i++) {
			if (cnt_freq[i]) cnts.push_back({i, cnt_freq[i]});
		}

		cerr << "Case #" << case_num << ": " << N << '\n';
		for (auto [c, f] : cnts) { cerr << c << 'x' << f << ' '; } cerr << '\n';

		std::vector<int> bases(cnts.size() + 1);
		bases[0] = 1;
		for (int i = 0; i < int(cnts.size()); i++) {
			bases[i+1] = bases[i] * (cnts[i].second + 1);
		}

		int S = bases.back();
		std::vector<int> sums(S);
		for (int i = 0; i < int(cnts.size()); i++) {
			for (int j = bases[i]; j < bases[i+1]; j++) {
				sums[j] = sums[j - bases[i]] + cnts[i].first;
			}
		}

		std::vector<int> pos;

		for (int g = N; g >= 1; g--) {
			if (g < cnts.back().first) continue;
			if (N % g != 0) continue;

			std::vector<bool> reachable(S);
			reachable[S-1] = true;
			for (int v = S-1; v >= 0; v--) {
				if (reachable[v]) {
					for (int i = 0; i < int(cnts.size()); i++) {
						if (v % bases[i+1] / bases[i]) {
							assert(sums[v] - sums[v - bases[i]] == cnts[i].first);
							if (sums[v - bases[i]] >= (sums[v] - 1) / g * g) {
								reachable[v - bases[i]] = true;
							}
						}
					}
				}
			}
			if (reachable[0]) {
				pos.push_back(N/g);
			}
		}

		std::sort(pos.begin(), pos.end());
		cout << "Case #" << case_num << ":";
		for (int v : pos) { cout << ' ' << v; } cout << '\n';
	}

	return 0;
}
