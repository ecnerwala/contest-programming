#include<bits/stdc++.h>
using namespace std;

bool adj[30][30];

bool go(string a, string b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < int(a.size()); i++) {
		if (!adj[a[i] - 'a'][b[i] - 'a']) {
			return false;
		}
	}
	return true;
}

int main() {
	int m, n; cin >> m >> n;
	for (int i = 0; i < m; i++) {
		char a, b; cin >> a >> b;
		adj[a - 'a'][b - 'a'] = true;
	}
	for (int k = 0; k < 26; k++) {
		adj[k][k] = true;
	}
	for (int k = 0; k < 26; k++) {
		for (int i = 0; i < 26; i++) {
			for (int j = 0; j < 26; j++) {
				if (adj[i][k] && adj[k][j]) adj[i][j] = true;
			}
		}
	}
	for (int i = 0 ; i< n; i++) {
		string a, b; cin >> a >> b;
		cout << (go(a,b) ? "yes" : "no") << '\n';
	}
	return 0;
}
