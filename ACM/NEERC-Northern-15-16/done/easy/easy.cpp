#include<bits/stdc++.h>
using namespace std;

int main() {
	if(fopen("easy.in", "r")) {
		freopen("easy.in", "r", stdin);
		freopen("easy.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	string s; cin >> s;
	bool neg = false;
	bool first = true;
	for(char c : s) {
		if(c == '+') {
			neg = false;
			first = true;
		} else if(c == '-') {
			neg = true;
			first = true;
		} else {
			if(neg && !first) {
				cout << '+';
				if(c != '0') neg = false;
			} else {
				first = false;
			}
		}
		cout << c;
	}
	cout << '\n';
}
