#include <bits/extc++.h>

#define john_lennox_vs_richard_dawkins main

int john_lennox_vs_richard_dawkins() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<string> labels(N);
	vector<int> amt(N);
	for (int i = 0; i < N; i++) {
		cin >> labels[i];
		string s; cin >> s;
		if (s == "?") amt[i] = -1;
		else amt[i] = std::stoi(s);
	}

	for (int i = 0; i < N; i++) {
		int lo, hi;
		if (amt[i] != -1) {
			lo = hi = amt[i];
		} else {
			// first, we minimize
			{
				int num_eq = 0;
				while (i + num_eq < N && amt[i + num_eq] == -1) num_eq++;
				lo = (i+num_eq >= N) ? 0 : amt[i+num_eq];
				int tot = 100;
				int cur = 100;
				for (int j = 0; j < N; j++) {
					if (j == i) cur = 0;
					if (amt[j] != -1) cur = amt[j];
					tot -= cur;
				}
				lo = max(lo, (tot + num_eq - 1) / num_eq);
			}
			{
				int num_eq = 0;
				while (i - num_eq >= 0 && amt[i - num_eq] == -1) num_eq++;
				hi = (i - num_eq < 0) ? 100 : amt[i - num_eq];
				int tot = 100;
				int cur = 0;
				for (int j = N-1; j >= 0; j--) {
					if (j == i) cur = 0;
					if (amt[j] != -1) cur = amt[j];
					tot -= cur;
				}
				hi = min(hi, tot / num_eq);
			}
		}
		cout << labels[i] << ' ' << lo << ' ' << hi << '\n';
	}

	return 0;
}

// We have one constraint which is sum a_i = 100
// We also have constraints that 0 <= a_1 <= a_2 <= a_3 <= ...
// The upper and lower bound are just the greedy bounds...
