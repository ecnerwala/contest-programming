#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<pair<int, pair<int, int>>> edges(M);
	for (int e = 0; e < M; e++) {
		int a, b, p; cin >> a >> b >> p; a--, b--;
		edges[e] = {p, {a,b}};
	}
	sort(edges.begin(), edges.end());

	vector<pair<int, int>> ans(M+1);
	ans[M] = {0,0};

	vector<int> deg(N, 0);

	vector<int> cntDeg(M+1);
	cntDeg[0] = N;

	vector<int> par(N, -1);
	vector<int> numV(N, 1);
	vector<int> numE(N);
	vector<bool> hasBig(N);

	auto getPar = [&](int a) {
		vector<int> stk;
		while (par[a] >= 0) stk.push_back(a), a = par[a];
		for (auto v : stk) par[v] = a;
		return a;
	};

	auto isCyc = [&](int a) {
		assert(par[a] < 0);
		return numV[a] == numE[a] && !hasBig[a];
	};

	int numCyc = 0;

	for (int e = M-1; e >= 0; e--) {
		int a, b; tie(a, b) = edges[e].second;
		// add the degree
		cntDeg[deg[a]]--;
		cntDeg[deg[b]]--;
		deg[a]++;
		deg[b]++;
		cntDeg[deg[a]]++;
		cntDeg[deg[b]]++;

		int pa = getPar(a);
		int pb = getPar(b);

		if (pa != pb) {
			numCyc -= isCyc(pa);
			numCyc -= isCyc(pb);

			if (par[pb] < par[pa]) {
				swap(pa, pb);
			}
			par[pa] += par[pb];
			par[pb] = pa;
			numE[pa] += numE[pb];
			numV[pa] += numV[pb];
			hasBig[pa] = hasBig[pa] || hasBig[pb];
		} else {
			numCyc -= isCyc(pa);
		}

		numE[pa]++;
		if (deg[a] > 2 || deg[b] > 2) {
			hasBig[pa] = true;
		}

		numCyc += isCyc(pa);

		ans[e] = {N - cntDeg[0] - cntDeg[2] + numCyc, (M-e) - cntDeg[2] + numCyc};
	}

	int Q; cin >> Q;
	while (Q--) {
		int v; cin >> v;
		int e = int(lower_bound(edges.begin(), edges.end(), pair<int, pair<int, int>>{v, {-1, -1}}) - edges.begin());
		cout << ans[e].first << ' ' << ans[e].second << '\n';
	}

	return 0;
}
