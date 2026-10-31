#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<pair<array<int, 2>, int>> E(N+2);
	const int INF = int(1e9) + 10;
	E[0] = {{INF, INF+1}, 0};
	for (int i = 1; i <= N; i++) {
		cin >> E[i].first[0] >> E[i].first[1];
		E[i].second = i;
	}
	E[N+1] = {{-1,0}, N+1};

	vector<array<int, 20>> par(N+2);
	par[0].fill(0);

	{
		vector<pair<int, int>> lefts(N+1);
		lefts[0] = {INF, 0};
		vector<pair<int, int>> rights(N+1);
		rights[0] = {0, N+1};
		for (int i = 1; i <= N; i++) {
			lefts[i] = {E[i].first[0], i};
			rights[i] = {E[i].first[1], i};
		}
		sort(lefts.begin(), lefts.end(), std::greater());
		sort(rights.begin(), rights.end(), std::greater());

		int best_right = INF+2;
		int best_val = -1;
		auto it = lefts.begin();
		for (auto [r, i] : rights) {
			for (; it != lefts.end() && it->first >= r; ++it) {
				int j = it->second;
				if (E[j].first[1] < best_right) {
					best_right = E[j].first[1];
					best_val = j;
				}
			}
			assert(best_val != -1);
			par[i][0] = best_val;
			for (int z = 0; par[i][z]; z++) {
				par[i][z+1] = par[par[i][z]][z];
			}
		}
	}

	auto get_cnt = [&](pair<array<int, 2>, int> prev_take, pair<array<int, 2>, int> next_take) -> int {
		int prev_r = prev_take.first[1];
		int next_l = next_take.first[0];
		if (prev_r > next_l) {
			return -N-1;
		}
		int len = 0;
		int cur = prev_take.second;
		int z = 0;
		while (E[par[cur][z]].first[1] <= next_l) {
			z++;
		}
		while (z > 0) {
			assert(E[par[cur][z]].first[1] > next_l);
			--z;
			if (E[par[cur][z]].first[1] <= next_l) {
				len += (1 << z);
				cur = par[cur][z];
			}
		}
		return len;
	};

	set<pair<array<int, 2>, int>> takes;
	takes.insert(E[N+1]);
	takes.insert(E[0]);
	assert(E[N+1] < E[0]);
	int cur_cnt = get_cnt(E[N+1], E[0]);
	if (cur_cnt < K) {
		cout << -1 << '\n';
		exit(0);
	}
	vector<int> ans; ans.reserve(K);
	for (int i = 1; int(ans.size()) < K && i <= N; i++) {
		auto it = takes.insert(E[i]).first;
		auto pt = std::prev(it);
		auto nt = std::next(it);
		int cnd_cnt = cur_cnt - get_cnt(*pt, *nt) + 1 + get_cnt(*pt, *it) + get_cnt(*it, *nt);
		if (cnd_cnt >= K) {
			cur_cnt = cnd_cnt;
			ans.push_back(i);
		} else {
			takes.erase(it);
		}
	}

	assert(int(ans.size()) == K);

	for (int v : ans) cout << v << '\n';

	return 0;
}
