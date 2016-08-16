#include<bits/stdc++.h>
using namespace std;

int F, I, T;
int cnt[20];

int main() {
	cin >> F >> I >> T;
	for(int i = 0; i < F; i++) {
		string s; cin >> s;
		for(int j = 0; j < I; j++) {
			if(s[j] == 'Y') cnt[j] ++;
		}
	}
	int res = 0;
	for(int j = 0; j < I; j++) {
		res += (cnt[j] >= T);
	}
	cout << res << '\n';

	return 0;
}
