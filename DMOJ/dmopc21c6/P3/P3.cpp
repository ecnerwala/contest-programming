#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<int> A(N*M);
	for (auto& v : A) cin >> v;

	std::vector<std::pair<int, int>> q; q.reserve(N*M);
	for (int i = 0; i < N*M; i++) {
		if (A[i]) {
			q.push_back({A[i], i});
		}
	}
	std::sort(q.begin(), q.end());
	auto qit = q.begin();
	for (int z = 0; z < K; z++) {
		for (auto en = q.end(); qit != en; qit++) {
			auto check = [&](int x, int y) {
				int v = x*M+y;
				if (A[v] != 0) return;
				A[v] = qit->first;
				q.push_back({A[v], v});
			};
			int i = qit->second / M;
			int j = qit->second % M;
			if (i-1 >= 0) check(i-1, j);
			if (i+1 < N) check(i+1, j);
			if (j-1 >= 0) check(i, j-1);
			if (j+1 < M) check(i, j+1);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << A[i*M+j] << " \n"[j+1==M];
		}
	}

	return 0;
}
