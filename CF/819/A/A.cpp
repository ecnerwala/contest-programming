#include<bits/stdc++.h>
using namespace std;

int A, B, L, R;
int go() {
	L--;
	int offset = L / (A + B) * (A + B);
	L -= offset;
	R -= offset;
	cerr << R << '\n';
	int len = R - L;
	string s;
	for (int i = 0; i < A; i++) {
		s.push_back(char('a' + i));
	}
	for (int i = 0; i < B; i++) {
		s.push_back(s.back());
	}
	int cnt[30];
	memset(cnt, 0, sizeof(cnt));
	for (int i = B; i < A + B; i++) {
		cnt[s[i] - 'a'] ++;
	}
	for (int i = 0, cur = 0; i < A; i++, cur++) {
		while (cnt[cur]) cur++;
		s.push_back(char('a' + cur));
	}
	for (int i = 0; i < B; i++) {
		s.push_back(s.back());
	}
	if (len >= 2 * (A + B)) {
	} else {
		while (R >= s.size()) s += s;
		s = s.substr(L, len);
	}
	//cerr << s << '\n';
	sort(s.begin(), s.end());
	return unique(s.begin(), s.end()) - s.begin();
}

int main() {
	cin >> A >> B >> L >> R;
	cout << go() << '\n';
	return 0;
}
