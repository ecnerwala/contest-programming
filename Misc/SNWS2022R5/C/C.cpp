#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int next_first = 0;
	std::vector<int> seconds; seconds.reserve(2*N);

	std::vector<int> label(2*N, -1);
	for (int z = 0; z < 2 * N - 1; z++) {
		int a; cin >> a; a--;
		if (label[a] == -1) {
			if (next_first == N) {
				label[a] = seconds.back();
				seconds.pop_back();
			} else {
				label[a] = next_first++;
				seconds.push_back(label[a]);
			}
		}
		cout << label[a]+1 << '\n' << flush;

		int b; cin >> b; b--;
		if (label[b] == -1) {
			if (seconds.empty()) {
				label[b] = next_first++;
				seconds.push_back(label[b]);
			} else if (seconds.back() == label[a]) {
				seconds.pop_back();
				if (!seconds.empty()) {
					label[b] = seconds.back();
					seconds.pop_back();
					seconds.push_back(label[a]);
				} else if (next_first < N) {
					label[b] = next_first++;
					seconds.push_back(label[b]);
					seconds.push_back(label[a]);
				} else {
					label[b] = label[a];
				}
			} else {
				label[b] = seconds.back();
				seconds.pop_back();
			}
		}
		cout << label[b]+1 << '\n' << flush;
	}

	return 0;
}
