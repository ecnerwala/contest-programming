#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
char A[MAXN+1];
char C[4] = {'B', 'S', 'C', 0};

int main() {
	int r = scanf("%d\n%s", &N, A);
	assert(r);

	assert(N > 0);
	int res = 0;

	// Special case: 0's don't count, so they can be dupes
	// This only changes things if it's 0, 0, k, so just try it for each color
	for (int x = 0; x < 3; x++) {
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] == C[x]) {
				cnt ++;
				res = max(res, cnt);
			} else {
				cnt = 0;
			}
		}
	}

	for (int z = 0; z < 3; z++) {
		if (A[0] == C[z]) {
			swap(C[z], C[2]);
			break;
		}
	}
	assert(A[0] == C[2]);

	// our initial two points are 0,0 and 1,1, by construction
	// there are a few cases:
	// 0, 0 works
	int diffs[2] = {1, 1};

	// start for state 01 and 10
	int s01 = -1;
	int s10 = -1;
	// start for state ==
	int se = -1;
	int se_A = 0;
	int se_B = 0;
	int se_xA = -1;
	int se_xB = -1;
	for (int i = 1; i < N; i++) {
		if (A[i] == C[0]) {
			diffs[0] --;
		} else if (A[i] == C[1]) {
			diffs[1] --;
		} else if (A[i] == C[2]) {
			diffs[0] ++;
			diffs[1] ++;
		} else assert(false);

		// first, find left endpoints
		if (diffs[0] != 0 && diffs[1] != 0 && diffs[0] - diffs[1] != 0 - 0) {
			// 0, 0 works
			res = max(res, i - (-1));
		} else if (diffs[0] != 1 && diffs[1] != 1 && diffs[0] - diffs[1] != 1 - 1) {
			// 1, 1 works
			res = max(res, i - 0);
		} else if (diffs[0] - diffs[1] == 0) {
			int v = diffs[0];
			if (se != -1) {
				if (v == se_A) {
					if (se_xA != -1) {
						res = max(res, i - se_xA);
					}
				} else if (v == se_B) {
					if (se_xB != -1) {
						res = max(res, i - se_xB);
					}
				} else {
					res = max(res, i - se);
				}
			}
		} else if (diffs[0] == 0 && diffs[1] == 1) {
			if (s01 != -1) {
				res = max(res, i - s01);
			}
		} else if (diffs[0] == 1 && diffs[1] == 0) {
			if (s10 != -1) {
				res = max(res, i - s10);
			}
		} else assert(false);

		// update as left endpoint
		if (s01 == -1 && diffs[0] != 0 && diffs[1] != 1 && diffs[0] - diffs[1] != 0 - 1) {
			s01 = i;
		}
		if (s10 == -1 && diffs[0] != 1 && diffs[1] != 0 && diffs[0] - diffs[1] != 1 - 0) {
			s10 = i;
		}
		if (se == -1 && diffs[0] != diffs[1]) {
			se = i;
			se_A = diffs[0];
			se_xA = -1;
			se_B = diffs[1];
			se_xB = -1;
		}
		if (se != -1 && diffs[0] != diffs[1]) {
			if (se_xA == -1 && diffs[0] != se_A && diffs[1] != se_A) {
				se_xA = i;
			}
			if (se_xB == -1 && diffs[0] != se_B && diffs[1] != se_B) {
				se_xB = i;
			}
		}
	}

	printf("%d\n", res);
	return 0;
}
