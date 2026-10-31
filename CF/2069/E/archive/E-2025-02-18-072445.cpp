#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int mA, mB, mAB, mBA; cin >> mA >> mB >> mAB >> mBA;
		cout << ([&]() -> bool {
			int cntA = 0, cntB = 0;
			for (char c : S) {
				if (c == 'A') cntA++;
				else if (c == 'B') cntB++;
				else assert(false);
			}
			int min_two = std::max({0, cntA - mA, cntB - mB});
			if (min_two > mAB + mBA) return false;
			assert(mAB + mBA >= min_two);

			int cnt_either = 0;
			std::vector<int> lenAB, lenBA;
			lenAB.reserve(S.size());
			lenBA.reserve(S.size());
			for (int i = 0, j = 0; i < int(S.size()); i = j) {
				j = i+1;
				while (j < int(S.size()) && S[j] != S[j-1]) j++;
				int l = j - i;
				if (l & 1) {
					cnt_either += (l / 2);
				} else {
					if (S[i] == 'A') {
						lenAB.push_back(l / 2);
					} else if (S[i] == 'B') {
						lenBA.push_back(l / 2);
					} else assert(false);
				}
			}
			min_two -= cnt_either;

			std::sort(lenAB.begin(), lenAB.end());
			std::sort(lenBA.begin(), lenBA.end());
			int cntAB = std::accumulate(lenAB.begin(), lenAB.end(), 0);
			int cntBA = std::accumulate(lenBA.begin(), lenBA.end(), 0);
			if (cntAB + cntBA < min_two) return false;
			assert(cntAB + cntBA >= min_two);
			assert(mAB + mBA >= min_two);
			if (std::min(cntAB, mAB) + std::min(cntBA, mBA) >= min_two) {
				return true;
			}
			assert(cntAB != mAB);
			assert(cntBA != mBA);
			assert((cntAB > mAB) != (cntBA > mBA));
			if (cntAB < mAB) {
				assert(cntBA > mBA);
				std::swap(cntAB, cntBA);
				std::swap(lenAB, lenBA);
				std::swap(mAB, mBA);
			}
			assert(cntAB > mAB);
			while (!lenAB.empty()) {
				int l = lenAB.back(); lenAB.pop_back();
				cntAB -= l;
				l--;
				min_two -= l;
				if (std::min(cntAB, mAB) + std::min(cntBA, mBA) >= min_two) {
					return true;
				}
			}
			return false;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
