#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAXN = 2000;
int N;
ll S[MAXN];

int go() {
	ll E;
	cin >> E >> N;
	for (int i = 0; i < N; i++) {
		cin >> S[i];
	}
	sort(S, S + N);
	int l = 0, r = N-1;
	int res = 0;
	int H = 0;
	while (l <= r) {
		if (E > S[l]) {
			E -= S[l];
			l++;
			H++;
			res = max(res, H);
		} else if (H > 0) {
			H--;
			E += S[r];
			r--;
		} else {
			break;
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
