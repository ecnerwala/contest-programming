#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		N++;

		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) cin >> P[i];

		std::vector<bool> is_key(N);
		for (int k = 0; k < M; k++) {
			int v; cin >> v;
			is_key[v] = true;
		}
	
		std::vector<std::vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[P[i]].push_back(i);
		}
		std::vector<int> q; q.reserve(N);
		q.push_back(0);
		std::vector<std::array<int, 2>> by_depth; by_depth.reserve(N);
		for (int st = 0, en; st < int(q.size()); st = en) {
			en = int(q.size());
			by_depth.push_back({st, en});
			for (int z = st; z < en; z++) {
				int cur = q[z];
				for (int nxt : ch[cur]) {
					q.push_back(nxt);
				}
			}
		}

		std::string C(N-1, '?');
		std::vector<int> vals(N, -1);
		using dp_t = std::array<int, 27>;
		std::vector<std::pair<dp_t, int>> buf; buf.reserve(N);
		for (int d = int(by_depth.size()) - 1; d >= 0; d--) {
			auto [st, en] = by_depth[d];
			buf.clear();
			for (int z = st; z < en; z++) {
				int cur = q[z];
				std::array<int, 27> val;
				val.fill(N+1);
				val[0] = is_key[cur] ? 0 : 1;
				std::sort(ch[cur].begin(), ch[cur].end(), [&](auto a, auto b) {
					return std::make_pair(vals[a], a) < std::make_pair(vals[b], b);
				});
				for (int i = 0; i < int(ch[cur].size()); i++) {
					C[ch[cur][i]-1] = char('a' + i);
					val[i+1] = vals[ch[cur][i]];
					assert(val[i+1] != -1);
				}
				buf.push_back({val, cur});
			}
			std::sort(buf.begin(), buf.end());
			int cur_val = 0;
			for (int z = 0; z < int(buf.size()); z++) {
				if (z > 0 && buf[z].first > buf[z-1].first) {
					cur_val++;
				}
				vals[buf[z].second] = cur_val;
			}
		}
		cout << C << '\n';
	}

	return 0;
}
