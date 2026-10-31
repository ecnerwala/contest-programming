#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		int R, C; cin >> R >> C;
		int V = R * C;
		vector<int> S(V);
		for (int i = 0; i < V; i++) {
			cin >> S[i];
		}
		vector<int> prvR(V);
		vector<int> nxtR(V);
		vector<int> prvC(V);
		vector<int> nxtC(V);
		for (int i = 0; i < V; i++) {
			prvR[i] = (i % C == 0) ? -1 : i-1;
			nxtR[i] = (i % C == C-1) ? -1 : i+1;
			prvC[i] = (i / C == 0) ? -1 : i-C;
			nxtC[i] = (i / C == R-1) ? -1 : i+C;
		}

		int64_t ans = 0;

		int64_t curSum = 0;
		for (auto v : S) curSum += v;

		vector<int> toCheck(V); toCheck.reserve(4*V);
		iota(toCheck.begin(), toCheck.end(), 0);

		vector<int> toRem; toRem.reserve(V);
		while (true) {
			ans += curSum;
			toRem.clear();
			for (auto i : toCheck) {
				if (i == -1 || !S[i]) continue;
				int totDiff = 0;
				auto checkNei = [&](int j) { if (j != -1) totDiff += S[j] - S[i]; };
				checkNei(prvR[i]);
				checkNei(nxtR[i]);
				checkNei(prvC[i]);
				checkNei(nxtC[i]);
				if (totDiff > 0) {
					toRem.push_back(i);
				}
			}
			toCheck.clear();
			if (toRem.empty()) {
				break;
			}
			for (auto i : toRem) {
				curSum -= S[i]; S[i] = 0;
				if (prvR[i] != -1) nxtR[prvR[i]] = nxtR[i];
				if (nxtR[i] != -1) prvR[nxtR[i]] = prvR[i];
				if (prvC[i] != -1) nxtC[prvC[i]] = nxtC[i];
				if (nxtC[i] != -1) prvC[nxtC[i]] = prvC[i];
				toCheck.push_back(prvR[i]);
				toCheck.push_back(nxtR[i]);
				toCheck.push_back(prvC[i]);
				toCheck.push_back(nxtC[i]);
				prvR[i] = nxtR[i] = prvC[i] = nxtC[i] = -1;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
