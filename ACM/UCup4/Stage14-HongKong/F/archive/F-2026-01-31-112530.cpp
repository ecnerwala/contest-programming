#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int RUN; cin >> RUN;
	if (RUN == 1) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) { cin >> u >> v; u--, v--; }
		int K; cin >> K;
		std::vector<int> P(K);
		for (auto& p : P) { cin >> p; p--; }
		std::vector<int> cyc_idx(N, -1);
		for (int i = 0; i < K; i++) cyc_idx[P[i]] = i;
		int nxt = K;
		for (int i = 0; i < N; i++) {
			if (cyc_idx[i] == -1) {
				cyc_idx[i] = nxt;
				nxt++;
			}
		}
		assert(nxt == N);

		for (auto [u, v] : edges) {
			if (cyc_idx[u] > cyc_idx[v]) std::swap(u, v);
			assert(cyc_idx[u] < cyc_idx[v]);
			if (cyc_idx[u] == 0 && cyc_idx[v] == K-1) std::swap(u, v);
			cout << u+1 << ' ' << v+1 << '\n';
		}
	} else if (RUN == 2) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> fadj(N);
		std::vector<std::vector<int>> radj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			fadj[u].push_back(v);
			radj[v].push_back(u);
		}

		// First, prune all leaves
		int K = N;
		std::vector<bool> alive(N, true);
		{
			std::vector<int> out_deg(N);
			for (int i = 0; i < N; i++) out_deg[i] = int(fadj[i].size());
			std::vector<int> q; q.reserve(N);
			for (int i = 0; i < N; i++) {
				if (!out_deg[i]) q.push_back(i);
			}
			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				assert(!out_deg[cur]);
				alive[cur] = false;
				K--;
				for (int nxt : radj[cur]) {
					if (!--out_deg[nxt]) {
						q.push_back(nxt);
					}
				}
			}
		}
		assert(K >= 3);

		std::vector<int> base_cyc; base_cyc.reserve(N);
		// Find any cycle
		{
			std::vector<int> stk; stk.reserve(N);
			std::vector<bool> in_stack(N, false);

			int cur = 0;
			while (!alive[cur]) cur++;

			while (!in_stack[cur]) {
				assert(alive[cur]);
				stk.push_back(cur);
				in_stack[cur] = true;

				assert(!radj[cur].empty());
				cur = radj[cur][0];
				assert(alive[cur]);
			}
			assert(in_stack[cur]);
			while (stk.back() != cur) {
				base_cyc.push_back(stk.back());
				stk.pop_back();
			}
			base_cyc.push_back(cur);
		}
		int L = int(base_cyc.size());
		std::vector<int> base_idx(N, -1);
		for (int i = 0; i < L; i++) base_idx[base_cyc[i]] = i;

		// Now, for each guy, find one dest and one source
		auto find_srcs = [&](const std::vector<std::vector<int>>& adj) -> std::vector<int> {
			std::vector<int> val(N, -1);
			std::vector<int> q; q.reserve(K);
			for (int i = 0; i < N; i++) {
				if (base_idx[i] != -1) {
					val[i] = base_idx[i];
					q.push_back(i);
				}
			}
			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				for (int nxt : adj[cur]) {
					if (!alive[nxt]) continue;
					if (val[nxt] != -1) continue;
					assert(base_idx[nxt] == -1);
					val[nxt] = val[cur];
					q.push_back(nxt);
				}
			}
			assert(int(q.size()) == K);
			return val;
		};
		std::vector<int> st = find_srcs(fadj);
		std::vector<int> en = find_srcs(radj);
		std::vector<int> pref(L+1, 0);
		for (int i = 0; i < N; i++) {
			if (!alive[i]) continue;
			for (int j : fadj[i]) {
				if (!alive[j]) continue;
				// if we're a cycle edge, skip it
				if (base_idx[i] != -1 && base_idx[j] != -1 && (base_idx[i]+1)%L == base_idx[j]) continue;
				int lo = st[i], hi = en[j];
				assert(lo != hi);
				if (lo < hi) {
					pref[lo]++;
					pref[hi]--;
				} else {
					pref[0]++;
					pref[hi]--;
					pref[lo]++;
					pref[L]--;
				}
			}
		}
		for (int i = 0; i < L; i++) {
			pref[i+1] += pref[i];
		}
		assert(pref[L] == 0);
		int good_idx = -1;
		for (int i = 0; i < L; i++) {
			if (pref[i] == 0) {
				good_idx = i;
			}
		}
		assert(good_idx != -1);
		// remove the i -> i+1 edge
		int cyc_st = base_cyc[good_idx], cyc_en = base_cyc[(good_idx+1)%L];
		for (auto x : fadj[cyc_st]) { assert(!alive[x] || x == cyc_en); }
		for (auto x : radj[cyc_en]) { assert(!alive[x] || x == cyc_st); }
		std::vector<int> ans; ans.reserve(K);
		alive[cyc_st] = false;
		alive[cyc_en] = false;
		{
			std::vector<int> in_deg(N);
			for (int i = 0; i < N; i++) {
				if (!alive[i]) continue;
				for (int j : fadj[i]) {
					if (!alive[j]) continue;
					in_deg[j]++;
				}
			}
			for (int i = 0; i < N; i++) {
				if (!alive[i]) continue;
				if (!in_deg[i]) ans.push_back(i);
			}
			for (int z = 0; z < int(ans.size()); z++) {
				int cur = ans[z];
				assert(alive[cur]);
				for (int nxt : fadj[cur]) {
					if (!alive[nxt]) continue;
					if (!--in_deg[nxt]) ans.push_back(nxt);
				}
			}
		}
		ans.push_back(cyc_st);
		ans.push_back(cyc_en);
		assert(int(ans.size()) == K);
		for (int i = 0; i < K; i++) {
			cout << ans[i]+1 << " \n"[i+1==K];
		}
	} else assert(false);

	return 0;
}
