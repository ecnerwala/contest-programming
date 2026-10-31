#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int Hmin, Hmax; cin >> Hmin >> Hmax;

	vector<int> A(N);
	for (int i = 0; i < N; i++) { cin >> A[i]; }
	sort(A.begin(), A.end());

	vector<pair<int, int>> gaps;
	for (int i = 0; i+1 < N; i++) {
		gaps.emplace_back(A[i+1] - A[i], i);
	}
	sort(gaps.begin(), gaps.end());

	set<pair<int, int>> mp;
	int curXor = 0;
	for (int i = 0; i < N; i++) {
		mp.emplace(i, i);
		curXor ^= 1;
	}

	gaps.emplace_back(int(2.1e9), -1);
	int curH = 0;
	for (auto gt : gaps) {
		// (curH, nxtH] is curXor
		int nxtH = gt.first;
		if (nxtH > curH && curXor == 0 && nxtH >= Hmin && curH < Hmax) {
			cout << max(Hmin, curH + 1) << '\n';
			exit(0);
		}

		curH = nxtH;

		int ind = gt.second;
		if (ind == -1) continue;

		auto it = mp.lower_bound(pair<int, int>(ind+1, -1));
		assert(it->first == ind + 1);
		int hi = it->second;
		curXor ^= (it->second - it->first + 1);

		auto jt = it; jt --;
		assert(jt->second == ind);
		int lo = jt->first;
		curXor ^= (jt->second - jt->first + 1);

		mp.erase(it);
		mp.erase(jt);

		mp.emplace(lo, hi);
		curXor ^= (hi - lo + 1);
	}

	cout << -1 << '\n';
	return 0;
}
