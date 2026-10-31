#include <bits/stdc++.h>

using std::cerr;
bool solve(std::string S, std::string T, std::vector<std::array<int, 2>> edges) {
	if (S == T) return true;
}

void test() {
	int N = 6;
	int st = 0b000111;
	std::vector<bool> poss(1 << N); poss[st] = true;
	std::vector<int> q; q.push_back(st);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int a = cur; true; a = (a-1) & cur) {
			int b = cur ^ a;
			int nxt = (b >> 1) | (b << (N-1)) | (a >> (N-1)) | (a << 1);
			nxt &= (1 << N) - 1;
			if (!poss[nxt]) {
				poss[nxt] = true;
				q.push_back(nxt);
			}
			if (a == 0) break;
		}
	}
	for (int a = 0; a < (1 << N); a++) {
		if (__builtin_popcount(a) == __builtin_popcount(st)) {
			if (!poss[a]) {
				cerr << a << '\n';
			}
		}
	}
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::string S; cin >> S;
		std::string T; cin >> T;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) { cin >> u >> v; u--, v--; }
		cout << (solve(S, T, edges) ? "Yes" : "No") << '\n';
	}

	return 0;
}
