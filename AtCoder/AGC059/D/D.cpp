#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;
		std::vector<int> A(N+K-1);
		auto solve = [&]() -> bool {
			std::vector<bool> has_in_edge(N+K-1);
			std::vector<bool> has_out_edge(N+K-1);
			for (int i = B[0]; i < K; i++) {
				has_in_edge[i] = true;
			}
			for (int i = 0; i+1 < N; i++) {
				if (B[i+1] - B[i] == 1) {
					has_out_edge[i] = true;
					has_in_edge[i+K] = false;
				} else if (B[i+1] - B[i] == -1) {
					has_out_edge[i] = false;
					has_in_edge[i+K] = true;
				} else if (B[i+1] - B[i] == 0) {
					// could be both false, but we'll just connect them in that case
					has_out_edge[i] = true;
					has_in_edge[i+K] = true;
				} else {
					return false;
				}
			}
			for (int i = N-1; i < N+K-1-B[N-1]; i++) {
				has_out_edge[i] = true;
			}
			std::vector<int> in_edges; in_edges.reserve(N+K-1);
			std::vector<int> out_edges; out_edges.reserve(N+K-1);
			for (int i = 0; i < N+K-1; i++) {
				if (has_in_edge[i]) in_edges.push_back(i);
				if (has_out_edge[i]) out_edges.push_back(i);
			}
			assert(in_edges.size() == out_edges.size());
			for (int z = 0; z < int(in_edges.size()); z++) {
				int d = in_edges[z] - out_edges[z];
				if (!(1 <= d && d <= K)) return false;
			}
			std::iota(A.begin(), A.end(), 1);
			for (int z = 0; z < int(in_edges.size()); z++) {
				A[in_edges[z]] = A[out_edges[z]];
			}
			return true;
		};

		if (solve()) {
			cout << "YES" << '\n';
			for (int i = 0; i < int(A.size()); i++) {
				cout << A[i] << " \n"[i+1==int(A.size())];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
