#include<bits/stdc++.h>
using namespace std;

int N;
vector<int> cutoffs[2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int z = 1; z >= 0; z--) {
		int x; cin >> x;
		cutoffs[z].reserve(2*x+2);

		cutoffs[z].push_back(0);
		for (int i = 0; i < x; i++) {
			int a, b; cin >> a >> b; a--;
			cutoffs[z].push_back(a);
			cutoffs[z].push_back(b);
		}
		cutoffs[z].push_back(N);
	}

	const int Tsmall = min(N, 100000);
	int ans = 0;
	for (int t = 1; t <= Tsmall; t++) {
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < int(cutoffs[z].size()); i += 2) {
				int a = cutoffs[z][i];
				int b = cutoffs[z][i+1];
				b /= t;
				if ((b & 1) != z) --b;
				if (b * t > a) {
					goto notGood;
				}
			}
		}
		//cerr << "small good " << t << '\n';
		ans++;
notGood:;
	}

	vector<pair<int, int>> evts;
	evts.reserve(2.1e7);
	// mark the boundaries
	auto markRange = [&](int l, int r) {
		evts.emplace_back(l, 1);
		evts.emplace_back(r, -1);
	};
	markRange(0, Tsmall);
	for (int k = 1; k * (Tsmall+1) <= N; k++) {
		int z = k%2;
		for (int i = 0; i < int(cutoffs[z].size()); i += 2) {
			int a = cutoffs[z][i];
			int b = cutoffs[z][i+1];
			markRange(a / k, b / k);
		}
	}

	sort(evts.begin(), evts.end());
	evts.emplace_back(N, 0);

	int curSum = 0;
	int prv = 0;
	for (auto it : evts) {
		if (curSum == 0) {
			//cerr << "big good " << prv << " - " << it.first << '\n';
			ans += (it.first - prv);
		}
		prv = it.first;
		curSum += it.second;
	}

	cout << ans << '\n';

	return 0;
}
