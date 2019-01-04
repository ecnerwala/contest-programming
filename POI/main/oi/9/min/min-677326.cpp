#include<bits/stdc++.h>

using namespace std;
int main() {
	int n; cin >> n;
	int cnt = 0;
	char c; cin >> c;
	assert(c == '-');
	cout << '-';
	for(int i = 1; i < n - 1; i++) {
		char v; cin >> v;
		if(v != c) {
			cout << '('; cnt++;
			c = v;
		}
		cout << '-';
	}
	while(cnt --) cout << ')';
	cout << '\n';
}
