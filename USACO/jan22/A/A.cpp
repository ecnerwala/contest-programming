#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<std::pair<int, int>> vals(N);
	for (int i = 0; i < N; i++) {
		cin >> vals[i].first;
		vals[i].second = i;
	}
	std::sort(vals.begin(), vals.end());
	std::vector<int> A(N);
	std::vector<int> Alo(N);
	std::vector<int> Ahi(N);
	for (int i = 0, a = 0, b = 0; i < N; i++) {
		while (a < N && vals[i].first - vals[a].first > K) a++;
		while (b < N && vals[b].first - vals[i].first <= K) b++;
		assert(a <= i && i < b);
		A[vals[i].second] = i;
		Alo[vals[i].second] = a;
		Ahi[vals[i].second] = b-1;
		// both exclusive
	}

	const int MAXV = N * 20;
	int V = N;
	std::vector<std::vector<int>> outEdges; outEdges.reserve(MAXV);
	outEdges.resize(V);
	std::vector<int> inDeg; inDeg.reserve(MAXV);
	inDeg.resize(V);
	{
		std::vector<int> lo_bit(N-1, -1);
		std::vector<int> hi_bit(N-1, -1);
		for (int i = 0; i < N; i++) {
			auto addDep = [&](int v) {
				if (v == -1) return;
				outEdges[v].push_back(i);
				inDeg[i]++;
			};
			for (int a = Alo[i]; a > 0; a &= a-1) addDep(lo_bit[a-1]);
			for (int a = Ahi[i]; a < N-1; a |= a+1) addDep(hi_bit[a]);

			auto insertSelf = [&](int& v) {
				if (v == -1) {
					v = i;
				} else {
					int nv = V++;
					outEdges.emplace_back();
					inDeg.push_back(2);
					outEdges[v].push_back(nv);
					outEdges[i].push_back(nv);
					v = nv;
				}
			};

			for (int a = A[i]; a > 0; a &= a-1) insertSelf(hi_bit[a-1]);
			for (int a = A[i]; a < N-1; a |= a+1) insertSelf(lo_bit[a]);
		}
	}

	min_pq<int> pq;
	std::vector<int> q; q.reserve(V);
	for (int i = 0; i < V; i++) {
		if (inDeg[i] == 0) q.push_back(i);
	}

	for (int i = 0; i < N; i++) {
		while (!q.empty()) {
			int cur = q.back(); q.pop_back();
			if (cur < N) {
				pq.push(A[cur]);
			} else {
				for (int nxt : outEdges[cur]) {
					if (!--inDeg[nxt]) q.push_back(nxt);
				}
			}
		}
		cout << vals[pq.top()].first << '\n';
		int cur = vals[pq.top()].second;
		pq.pop();
		for (int nxt : outEdges[cur]) {
			if (!--inDeg[nxt]) q.push_back(nxt);
		}
	}

	return 0;
}
