#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> inEdges(N);
	std::vector<int> outDeg(N);
	for (int i = 0; i < N; i++) {
		int c; cin >> c;
		inEdges[i].resize(c);
		for (auto& a : inEdges[i]) { cin >> a; a--; outDeg[a]++; }
	}

	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (outDeg[i] == 0) {
			q.push_back(i);
		}
	}
	for (int z = 0; z < int(q.size()); z++) {
		for (int nxt : inEdges[q[z]]) {
			if (!--outDeg[nxt]) {
				q.push_back(nxt);
			}
		}
	}
	std::reverse(q.begin(), q.end());
	for (int i = 0; i < N; i++) cout << q[i]+1 << " \n"[i+1==N];

	return 0;
}
