#include<bits/stdc++.h>
using namespace std;

int n, m, q;
int a[20], b[20];
int nxt[4000000];
char val[4000000];

char str[4000000];

bool check(int i, int j) {
	for(int v = 0; str[v]; v++) {
		char c = str[v];
		if(val[i * m + j] != c) return false;
		int ni = i + a[c - '0'], nj = j + b[c - '0'];
		if(ni < 0 || ni >= n || nj < 0 || nj >= m) ni = i, nj = j;
		i = ni, j = nj;
	}
	return true;
}

bool check() {
	for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) if(check(i, j)) return true;
	return false;
}

int main() {
	cin >> n >> m >> q;
	for(int i = 0; i < n; i++) cin >> (val + (m * i));
	for(int i = 0; i < 10; i++) cin >> a[i] >> b[i];
	while(q--) {
		cin >> str;
		cout << (check() ? "YES" : "NO") << endl;
	}
	return 0;
}
