#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, Q; cin >> N >> M >> Q;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> invA(N);
		for (int i = 0; i < N; i++) invA[A[i]] = i;
		std::vector<int> B(M);
		for (auto& b : B) { cin >> b; b = invA[b]; }
		std::vector<std::set<int>> times(N);
		for (int j = 0; j < M; j++) {
			times[B[j]].insert(j);
		}
		std::vector<int> start_time(N);
		for (int i = 0; i < N; i++) {
			start_time[i] = times[i].empty() ? M : *times[i].begin();
		}
		int num_bad = 0;
		for (int i = 0; i+1 < N; i++) {
			if (start_time[i] > start_time[i+1]) {
				num_bad++;
			}
		}
		cout << (num_bad ? "TIDAK" : "YA") << '\n';
	}

	return 0;
}
