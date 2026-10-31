#include<bits/stdc++.h>
using namespace std;

map<pair<int, int>, int> mp;
int N;
string S;

bool isPal(string s) {
	return s == string(s.rbegin(), s.rend());
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S; N = int(S.size());
	int i = 0, j = N-1;
	while (i <= j && S[i] == S[j]) {
		i++, j--;
	}
	if (i > j) {
		cout << "YES" << '\n';
		cout << i << '\n';
		exit(0);
	}
	if (isPal(S.substr(i, j-i))) {
		cout << "YES" << '\n';
		cout << j+1 << '\n';
	} else if (isPal(S.substr(i+1, j-i))){
		cout << "YES" << '\n';
		cout << i+1 << '\n';
	} else {
		cout << "NO" << '\n';
	}

	return 0;
}
