#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	int B; cin >> B;

	for (int case_num = 1; case_num <= T; case_num ++) {
		vector<int> ans(B);
		int eqPair = -1;
		int neqPair = -1;

		bool curRev = false;
		bool curComp = false;

		auto query = [&](int i) -> bool {
			assert(0 <= i && i < B);
			cout << i+1 << '\n' << flush;
			char c; cin >> c;
			if (c == 'N') {
				// ERROR!
				exit(0);
			}
			assert(c == '0' || c == '1');
			return c - '0';
		};

		auto queryPair = [&](int i) {
			assert(0 <= i && i < B/2);
			int q1 = query(i), q2 = query(B-1-i);
			if (curRev) swap(q1, q2);
			if (curComp) q1 ^= 1, q2 ^= 1;
			ans[i] = q1;
			ans[B-1-i] = q2;
			if (q1 == q2) eqPair = i;
			else neqPair = i;
		};

		int curInd = 0;
		auto startBatch = [&]() {
			if (eqPair == -1 && neqPair == -1) {
				curRev = curComp = false;
				queryPair(curInd++);
			} else if (eqPair == -1 || neqPair == -1) {
				int q = max(neqPair, eqPair);
				query(q); // dummy query
				int v = query(q);
				curRev = false;
				curComp = (v != ans[q]);
			} else {
				curComp = query(eqPair) != ans[eqPair];
				curRev = curComp ^ (query(neqPair) != ans[neqPair]);
			}
		};

		while (curInd < B/2) {
			startBatch();
			for (int z = 0; z < 4 && curInd < B/2; z++) {
				queryPair(curInd++);
			}
		}

		if (curRev) {
			reverse(ans.begin(), ans.end());
		}
		if (curComp) {
			for (int i = 0; i < B; i++) ans[i] ^= 1;
		}

		cout << "garbage" << '\n';
		for (int i = 0; i < B; i++) { cout << ans[i]; } cout << '\n' << flush;
		char c; cin >> c;
		if (c == 'N') exit(0);
		assert(c == 'Y');
	}

	return 0;
}
