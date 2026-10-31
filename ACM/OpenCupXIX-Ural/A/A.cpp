#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	map<char, long long> mp;
	for (char c : S) mp[c] ++;
	long long ans = 1;
	for (auto it : mp) {
		ans *= it.second + 1;
	}
	cout << (ans-1) << '\n';

	return 0;
}
