#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	string S[5];
	for (int i = 0; i < 1024; i++) {
		for (int z = 0; z < 5; z++) {
			S[z] += char('0' + ((i >> z) & 1));
		}
	}

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, B, F; cin >> N >> B >> F;
		assert(B <= 15);

		string Q[5];
		for (int q = 0; q < 5; q++) {
			cout << S[q].substr(0, N) << endl << flush;
			cin >> Q[q];
			if (Q[q] == "-1") exit(0);
			assert(int(Q[q].size()) == N - B);
			Q[q] += S[q].substr(N);
			assert(int(Q[q].size()) == 1024 - B);
		}

		vector<int> bads;
		int nxt = 0;
		for (int i = 0; i < 1024 - B; i++) {
			int v = 0;
			for (int q = 0; q < 5; q++) {
				v |= int(Q[q][i] - '0') << q;
			}
			while ((nxt & 31) != v) {
				bads.push_back(nxt);
				nxt++;
			}
			nxt++;
		}

		while (nxt != 1024) {
			bads.push_back(nxt);
			nxt++;
		}

		assert(int(bads.size()) == B);
		for (int i = 0; i < B; i++) {
			cout << bads[i] << " \n"[i+1==B];
		}
		cout << flush;
		int verdict; cin >> verdict;
		if (verdict != 1) exit(0);
	}

	return 0;
}
