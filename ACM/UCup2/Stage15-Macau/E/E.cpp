#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }
	std::vector<int> B(N);
	for (auto& b : B) { cin >> b; b--; }
	std::vector<std::array<int, 2>> edges; edges.reserve(2*N-1);
	for (int z = 0; z < 2; z++) {
		// Add the edges
		std::vector<int> inv_B(N);
		for (int i = 0; i < N; i++) inv_B[B[i]] = i;
		// earliest guy in b
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		for (auto a : A) {
			while (!stk.empty() && stk.back().first < a) {
				auto r = stk.back().second;
				stk.pop_back();
				if (!stk.empty()) stk.back().second = std::min(stk.back().second, r);
			}
			if (!stk.empty()) {
				if (stk.back().second < inv_B[a]) {
					edges.push_back({B[stk.back().second], a});
				} else {
					cout << "No" << '\n';
					exit(0);
				}
			}
			stk.push_back({a, inv_B[a]});
		}
		std::swap(A, B);
		for (auto& a : A) { a = N-1-a;}
		for (auto& b : B) { b = N-1-b; }
		for (auto& [u, v] : edges) { u = N-1-u, v = N-1-v; }
	}
	cout << "Yes" << '\n';
	for (auto [u, v] : edges) {
		cout << u+1 << ' ' << v+1 << '\n';
	}

	return 0;
}
