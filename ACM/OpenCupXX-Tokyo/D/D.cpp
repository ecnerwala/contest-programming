#include<bits/stdc++.h>
using namespace std;

int64_t solve(int64_t N, int64_t S, int64_t X) {
	int64_t V = max(S, X);
	int L = (V ? 64 - __builtin_clzll(V) : 0);

	vector<pair<int64_t, int64_t>> possExtra(L+1);
	possExtra[0] = {0,0};
	for (int i = 0; i < L; i++) {
		int64_t minInc = 0, maxInc = N;
		if ((minInc ^ (X>>i)) & 1) minInc++;
		if ((maxInc ^ (X>>i)) & 1) maxInc--;

		if (minInc > maxInc) return -1;

		int64_t minExtra = possExtra[i].first, maxExtra = possExtra[i].second;
		if ((minExtra ^ ((S^X)>>i)) & 1) minExtra++;
		if ((maxExtra ^ ((S^X)>>i)) & 1) maxExtra--;

		if (minExtra > maxExtra) return -1;

		possExtra[i+1] = {(minInc + minExtra) / 2, (maxInc + maxExtra) / 2};
		assert(possExtra[i+1].first <= 1);
	}

	if (possExtra[L].first > 0) return -1;

	vector<pair<int64_t, int64_t>> cnds;
	cnds.emplace_back(0, 0); // number of free ones, number of extra

	int64_t ans = 0;
	for (int i = L-1; i >= 0; i--) {
		ans *= 2;

		for (auto& it : cnds) {
			it.second = it.second * 2 + ((S >> i) & 1);
		}

		vector<pair<int64_t, int64_t>> nCnds;

		for (int z = 0; z < 2; z++) {
			for (auto it : cnds) {
				int64_t numFree = it.first, numExtra = it.second;

				int64_t numTake = min(z ? N : numFree, numExtra);
				if ((numTake ^ (X >> i)) & 1) {
					numTake--;
					if (numTake < 0) continue;
				}

				if (!(possExtra[i].first <= numExtra - numTake)) {
					numTake -= 2;
					if (numTake < 0) continue;
				}

				for (; numTake >= 0 && numExtra - numTake <= possExtra[i].second; numTake -= 2) {
					int64_t nxtFree = z ? min(N - numTake + numFree, N) : numFree;
					int64_t nxtExtra = numExtra - numTake;

					nCnds.emplace_back(nxtFree, nxtExtra);
					if (!(z && nxtFree < 3)) break;
				}
			}
			if (!nCnds.empty()) {
				ans += z;
				break;
			}
		}
		assert(!nCnds.empty());

		cnds.clear();
		sort(nCnds.begin(), nCnds.end(), [&](auto a, auto b) { return a.first != b.first ? a.first < b.first : a.second > b.second; });
		for (auto it : nCnds) {
			if (!cnds.empty() && it.second <= cnds.back().second) {
				cnds.pop_back();
			}
			cnds.push_back(it);
		}

		assert(!cnds.empty());
		for (auto it : cnds) {
			assert(possExtra[i].first <= it.second && it.second <= possExtra[i].second);
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, S, X; cin >> N >> S >> X;
		int64_t ans = solve(N, S, X);
		cout << ans << '\n';
	}

	return 0;
}
