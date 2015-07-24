#include<bits/stdc++.h>
using namespace std;

char sub[26] = {
	'a', // 'a'
	'b', // 'b'
	'c', // 'c'
	'd', // 'd'
	'e', // 'e'
	'f', // 'f'
	'g', // 'g'
	'h', // 'h'
	'i', // 'i'
	'j', // 'j'
	'k', // 'k'
	'l', // 'l'
	'm', // 'm'
	'n', // 'n'
	't', // 'o'
	'p', // 'p'
	'q', // 'q'
	'r', // 'r'
	's', // 's'
	'o', // 't'
	'u', // 'u'
	'v', // 'v'
	'w', // 'w'
	'x', // 'x'
	'y', // 'y'
	'z'  // 'z'
};

int cnt[200];

int main() {
	freopen("cipher.txt", "r", stdin);
	freopen("cipher.out", "w", stdout);

	while(cin) {
		string s;
		getline(cin, s);
		for(char c : s) {
			if('a' <= c && c <= 'z') {
				cnt[c - 'a'] ++;
				c = sub[c - 'a'];
			} else if('A' <= c && c <= 'Z') {
				cnt[c - 'A'] ++;
				c = sub[c - 'A'] + 'A' - 'a';
			}
			cout << c;
		}
		cout << '\n';
	}

	for(char i = 'a'; i <= 'z'; i++) {
		cerr << i << ' ' << cnt[i - 'a'] << '\n';
	}
	return 0;
}
