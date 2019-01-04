#include<bits/stdc++.h>
using namespace std;

const int MAXMOD = 1.1e9;
int MOD;
int A, B;
int THRESH; //thresh

const int MAXL = 1.1e6;
int L;
char W[MAXL];

vector<pair<int, bool>> evts; // false is range end, true is range start

void excludeRange(int a, int b) {
	assert(a <= b);
	if (a == b) return;
	assert(b - a <= MOD);
	if (b - a == MOD) {
		evts.emplace_back(0, true);
		evts.emplace_back(MOD, false);
		return;
	}
	a %= MOD;
	if (a < 0) a += MOD;
	b %= MOD;
	if (b < 0) b += MOD;
	if (b < a) {
		// exclude a..MOD and 0..b
		evts.emplace_back(0, true);
		evts.emplace_back(MOD, false);
	}
	// exclude a..b
	evts.emplace_back(a, true);
	evts.emplace_back(b, false);
}

int main() {
	int r = scanf("%d %d %d %d %d", &MOD, &A, &B, &THRESH, &L);
	assert(r);
	assert(0 < THRESH && THRESH < MOD);
	r = scanf(" %s", W);
	assert(r);
	for (int i = 0, a = 0; i < L; i++, a = (a+A)%MOD) {
		if (W[i] == '1') {
			excludeRange(0 - a, THRESH - a);
		} else {
			excludeRange(THRESH - a, MOD - a);
		}
	}
	// exclude the things going backwards
	for (int i = 1, b = B; i < L; i++) {
		b -= A;
		if (b < 0) b += MOD;
		excludeRange(b, b+1);
	}

	sort(evts.begin(), evts.end());
	int res = 0;
	int cur = 0;
	int cnt = 0;
	for (pair<int, bool> it : evts) {
		if (!cnt) res += it.first - cur;
		cur = it.first;
		if (it.second) cnt++;
		else cnt--;
	}
	if (!cnt) res += MOD - cur;
	printf("%d\n", res);

	return 0;
}
