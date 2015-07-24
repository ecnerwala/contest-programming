#include<bits/stdc++.h>
using namespace std;

int N, K;

inline bool vowel(char c) {
	return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y';
}

bool rhyme() {
	string a, b;
	getline(cin, a);
	getline(cin, b);
	int cnt = 0;
	for(int i = 0; i < int(a.size()); i++) {
		cnt += vowel(a[i]);
	}
	for(int i = 0; i < int(b.size()); i++) {
		cnt -= vowel(b[i]);
	}
	if(cnt) return false;
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	for(int i = 0, j = 0, k = K; k; ) {
		if(i == int(a.size())) return false;
		else if(j == int(b.size())) return false;
		else if(a[i] == ' ') i++;
		else if(b[j] == ' ') j++;
		else if(a[i] != b[j]) return false;
		else i++, j++, k--;
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	string s;
	getline(cin, s);
	int res = 0;
	while(N--) {
		res += rhyme();
	}
	cout << res << '\n';
	return 0;
}
