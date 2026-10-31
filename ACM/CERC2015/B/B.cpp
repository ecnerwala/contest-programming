#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	string s; getline(cin, s);
	istringstream stream(s);
	vector<int> lens;
	while (stream >> s) {
		lens.push_back(int(s.size())+1);
	}
	//for (auto l : lens) cerr << l << '\n';
	int totLen = 0;
	for (auto l : lens) {
		totLen += l;
	}

	vector<int> nxtPref(totLen+1);
	int pref = 0;
	vector<int> prefLen(totLen+1);
	for (auto l : lens) {
		prefLen[pref] = l;
		for (int i = 0; i < l; i++) {
			nxtPref[pref+i] = pref;
		}
		pref += l;
	}
	nxtPref[totLen] = totLen;

	int a, b; cin >> a >> b;
	a++, b++;
	for (int m = a; m <= b; m++) {
		int cur = 0;
		int ans = 0;
		while (cur < totLen) {
			ans += prefLen[cur];
			cur = nxtPref[min(totLen, cur + m)];
		}
		cout << ans-1 << '\n';
	}

	return 0;
}
