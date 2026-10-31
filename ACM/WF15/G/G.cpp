#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int C; cin >> C;
	while (C--) {
		int L, V1, V2, T, S; cin >> L >> V1 >> V2 >> T >> S;
		int totPieces = (V2 - V1) / T;

		int curDepth = 0;
		int curRem = 1;
		for (int k = 1; k < totPieces; k++) {
			int nDepth = L / S / (V2 - T * k);
			curRem <<= nDepth - curDepth;
			curDepth = nDepth;
			if (!curRem) {
				curDepth = -1;
				break;
			}
			curRem--;
			if (curRem >= totPieces - k) {
				break;
			}
		}
		if (curDepth == -1) {
			cout << "impossible" << '\n';
		} else {
			cout << curDepth << '\n';
		}
	}

	return 0;
}
