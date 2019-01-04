#include<bits/stdc++.h>
using namespace std;

using pid = pair<int, double>;

const int MAXN = 200;
int N;
int P;
pid D[MAXN];

double go() {
	vector<pid> dp;
	dp.emplace_back(0, 0);
	for (int z = 0; z < N; z++) {
		vector<pid> dp2;
		dp2.reserve(dp.size());
		for (auto it : dp) {
			dp2.emplace_back(it.first + D[z].first, it.second + D[z].second);
		}

		vector<pid> ndp;
		for (size_t i = 0, j = 0; i < dp.size() || j < dp2.size(); ) {
			int st = P+1;
			if (i < dp.size()) {
				st = min(st, dp[i].first);
			}
			if (j < dp2.size()) {
				st = min(st, dp2[j].first);
			}
			if (st > P) {
				break;
			}
			double en = double(st);
			while (true) {
				if (i < dp.size() && (dp[i].first == st || dp[i].first <= en)) {
					en = max(en, dp[i].second);
					i ++;
				} else if (j < dp2.size() && (dp2[j].first == st || dp2[j].first <= en)) {
					en = max(en, dp2[j].second);
					j ++;
				} else {
					break;
				}
			}
			ndp.emplace_back(st, en);
		}
		dp = std::move(ndp);
		assert(dp.back().first <= P);
	}
	assert(dp.back().first <= P);
	return min(dp.back().second, double(P));
}

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		scanf("%d %d", &N, &P);
		int p = 0;
		for (int i = 0; i < N; i++) {
			int w, h;
			scanf("%d %d", &w, &h);
			p += 2 * w + 2 * h;
			if (w > h) swap(w, h);
			D[i].first = 2 * w;
			D[i].second = 2 * sqrt(double(w * w + h * h));
		}
		P -= p;
		double res = go() + p;
		printf("Case #%d: %.9f\n", case_num, res);
	}

	return 0;
}
