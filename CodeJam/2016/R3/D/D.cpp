#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;

int N, L;
string G[MAXN];
string B;

string go() {
	for(int i = 0; i < N; i++) {
		if(G[i] == B) {
			return "IMPOSSIBLE";
		}
	}
	if(L == 1) {
		if(B == "1") return "0 0?";
		else return "1 1?";
	}
	string a = "";
	string b = "";
	for(int i = 0; i < L - 1; i++) {
		a += '?';
	}
	for(int i = 0; i < (L / 2) + 1; i++) {
		b += '1';
		b += '0';
	}
	b += '0';
	b += '?';
	for(int i = 0; i < (L / 2) + 1; i++) {
		b += '1';
		b += '0';
	}
	return a + " " + b;
}
int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> L;
		for(int i = 0; i < N; i++) {
			cin >> G[i];
		}
		cin >> B;

		string res = go();

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
