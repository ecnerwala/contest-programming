#include<bits/stdc++.h>
using namespace std;

const int ANSMOD = int(1e9)+7;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	string S; cin >> S;
	assert(int(S.size()) == N);

	int f = 1;
	for (int v = M; v % 2 == 0; v /= 2) f *= 2;
	for (int v = M; v % 5 == 0; v /= 5) f *= 5;

	vector<int> cnt(M);
	vector<pair<int, int>> bads;

	cnt[0]++;
	int curPref = 0;
	for (int i = 0; i < N; i++) {
		vector<pair<int, int>> nbads;
		for (auto it : bads) {
			cnt[it.first] -= it.second; if (cnt[it.first] < 0) cnt[it.first] += ANSMOD;

			it.first *= 10, it.first %= M;
			
			cnt[it.first] += it.second; if (cnt[it.first] >= ANSMOD) cnt[it.first] -= ANSMOD;
			if (it.first % f != 0) {
				nbads.push_back(it);
			}
		}

		curPref *= 10;
		curPref += (S[i] - '0');
		curPref %= M;

		if (i == N-1) {
			cout << cnt[curPref] << '\n';
			break;
		}

		{
			pair<int, int> it(curPref, cnt[curPref]);
			cnt[it.first] += it.second; if (cnt[it.first] >= ANSMOD) cnt[it.first] -= ANSMOD;
			if (it.first % f != 0) {
				nbads.push_back(it);
			}
		}

		bads = nbads;
	}

	return 0;
}
