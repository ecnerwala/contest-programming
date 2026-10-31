#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, N, K; cin >> T >> N >> K;
	std::vector<int> A(N);
	std::vector<int> colors(N);
	std::vector<int> cyc; cyc.reserve(N);
	for (int case_num = 1; case_num <= T; case_num ++) {
		while (true) {
			for (auto& a : A) { cin >> a; a--; }
			std::fill(colors.begin(), colors.end(), -1);
			for (int i = 0; i < N; i++) {
				if (colors[i] != -1) continue;
				cyc.push_back(i);
				for (int st = A[i]; st != i; st = A[st]) {
					cyc.push_back(st);
				}
				while (cyc.size() >= 6) {
					int a = cyc.back(); cyc.pop_back();
					int b = cyc.back(); cyc.pop_back();
					int c = cyc.back(); cyc.pop_back();
					colors[a] = colors[b] = colors[c] = a;
				}
				for (int z = 0; z < int(cyc.size()); z++) {
					colors[cyc[z]] = cyc[0];
				}
				cyc.clear();
			}
			for (int i = 0; i < N; i++) {
				cout << colors[i]+1 << " \n"[i+1==N];
			}
			cout << flush;
			int resp; cin >> resp;
			if (resp == -1) exit(0);
			else if (resp == 1) break;
			else if (resp == 0) continue;
			else assert(false);
		}
	}

	return 0;
}
