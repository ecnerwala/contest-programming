#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) { cin >> a; a--; }

	vector<int> ans; ans.reserve(N);

	vector<int> curString; curString.reserve(N);
	// store the midpoint
	vector<int> maxPalEnding; maxPalEnding.reserve(N+1);
	// number of folds
	vector<int> numSuffFolds; numSuffFolds.reserve(N+1);

	// store the half-length
	vector<int> minPalStarting; minPalStarting.reserve(N);
	// store the radius
	vector<int> maxPalCenter; maxPalCenter.reserve(N+1);

	maxPalEnding.push_back(0);
	numSuffFolds.push_back(0);
	maxPalCenter.push_back(0);

	vector<pair<int, int>> minPalStartingUpdates; minPalStartingUpdates.reserve(N);

	auto addChar = [&](int v) {
		int r = int(curString.size());
		int m = maxPalEnding.back();
		vector<int> minPalStartingCnds;
		{
			int nm = m;
			int nr = r+1;
			while (nm < nr) {
				if (maxPalCenter[nm] == r - nm && 2 * nm - nr >= 0 && curString[2 * nm - nr] == v) {
					maxPalCenter[nm]++;
					if (minPalStarting[2 * nm - nr] == -1) {
						minPalStartingUpdates.emplace_back(nr, 2 * nm - nr);
						minPalStarting[2 * nm - nr] = nr - nm;
					}
					break;
				}

				assert(int(minPalStarting.size()) == nm);
				minPalStarting.push_back(-1);
				minPalStartingCnds.push_back(nm);

				nm++;

				maxPalCenter.push_back(nm == nr ? 0 : min(r - nm, maxPalCenter[2 * m - nm]));
				assert(int(maxPalCenter.size()) == nm+1);
				while (minPalStartingCnds.back() >= nm - maxPalCenter.back()) {
					int p = minPalStartingCnds.back();
					minPalStartingCnds.pop_back();
					minPalStarting[p] = nm - p;
				}
			}
			m = nm, r = nr;
		}

		curString.push_back(v);
		maxPalEnding.push_back(m);
		numSuffFolds.push_back(m < r ? numSuffFolds[r - minPalStarting[2*m-r]] + 1 : 0);
	};

	int curFolds = 0;

	for (int v : A) {
		addChar(v);

		int r = int(curString.size());
		int m = maxPalEnding.back();

		if (m < r && maxPalCenter[2*m-r] >= r-m) {
			// trim and 2 folds
			curFolds += 2;

			int nl = 2*m - r;
			curString.resize(nl);
			maxPalEnding.resize(nl+1);
			numSuffFolds.resize(nl+1);
			m = maxPalEnding[nl];
			maxPalCenter.resize(m+1);
			// hard reset this guy
			maxPalCenter.back() = nl - m;
			while (!minPalStartingUpdates.empty() && minPalStartingUpdates.back().first > nl) {
				minPalStarting[minPalStartingUpdates.back().second] = -1;
				minPalStartingUpdates.pop_back();
			}
			minPalStarting.resize(m);
		} else if (2 * m - r == 0) {
			// bisect
			curFolds++;

			vector<int> nString(curString.begin() + m, curString.end());
			curString.clear();
			maxPalEnding.clear(); maxPalEnding.push_back(0);
			numSuffFolds.clear(); numSuffFolds.push_back(0);
			maxPalCenter.clear(); maxPalCenter.push_back(0);
			minPalStarting.clear();
			minPalStartingUpdates.clear();

			for (int x : nString) addChar(x);
		}

		ans.push_back(curFolds + numSuffFolds.back());
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
