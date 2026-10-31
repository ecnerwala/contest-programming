#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;

	std::array<std::array<int, 3>, 3> cnt;
	for (int z = 0; z < 3; z++) cnt[z].fill(0);
	for (int i = 0; i < 3*N; i++) {
		cnt[i/N][S[i]-'A']++;
	}

	for (int x = 0; x <= N; x++) {
		int nABC = x;
		int nACB = cnt[0][0] - nABC;
		int nCBA = cnt[1][1] - nABC;
		int nBAC = cnt[2][2] - nABC;

		int nBCA = cnt[0][1] - nBAC;
		int nCAB = cnt[0][2] - nCBA;

		if (std::min({nABC, nACB, nCBA, nBAC, nBCA, nCAB}) < 0) continue;

		for (int i = 0; i < N; i++) {
			if (S[i] == 'A') {
				if (nABC > 0) nABC--, cout << 1;
				else if (nACB > 0) nACB--, cout << 2;
				else assert(false);
			} else if (S[i] == 'B') {
				if (nBCA > 0) nBCA--, cout << 3;
				else if (nBAC > 0) nBAC--, cout << 4;
				else assert(false);
			} else if (S[i] == 'C') {
				if (nCAB > 0) nCAB--, cout << 5;
				else if (nCBA > 0) nCBA--, cout << 6;
				else assert(false);
			} else assert(false);
		}

		nABC = x;
		nACB = cnt[0][0] - nABC;
		nCBA = cnt[1][1] - nABC;
		nBAC = cnt[2][2] - nABC;
		nBCA = cnt[0][1] - nBAC;
		nCAB = cnt[0][2] - nCBA;

		for (int i = N; i < 2*N; i++) {
			if (S[i] == 'A') {
				if (nCAB > 0) nCAB--, cout << 5;
				else if (nBAC > 0) nBAC--, cout << 4;
				else assert(false);
			} else if (S[i] == 'B') {
				if (nABC > 0) nABC--, cout << 1;
				else if (nCBA > 0) nCBA--, cout << 6;
				else assert(false);
			} else if (S[i] == 'C') {
				if (nBCA > 0) nBCA--, cout << 3;
				else if (nACB > 0) nACB--, cout << 2;
				else assert(false);
			} else assert(false);
		}

		nABC = x;
		nACB = cnt[0][0] - nABC;
		nCBA = cnt[1][1] - nABC;
		nBAC = cnt[2][2] - nABC;
		nBCA = cnt[0][1] - nBAC;
		nCAB = cnt[0][2] - nCBA;

		for (int i = 2*N; i < 3*N; i++) {
			if (S[i] == 'A') {
				if (nBCA > 0) nBCA--, cout << 3;
				else if (nCBA > 0) nCBA--, cout << 6;
				else assert(false);
			} else if (S[i] == 'B') {
				if (nCAB > 0) nCAB--, cout << 5;
				else if (nACB > 0) nACB--, cout << 2;
				else assert(false);
			} else if (S[i] == 'C') {
				if (nABC > 0) nABC--, cout << 1;
				else if (nBAC > 0) nBAC--, cout << 4;
				else assert(false);
			} else assert(false);
		}
		exit(0);
	}
	assert(false);

	return 0;
}
