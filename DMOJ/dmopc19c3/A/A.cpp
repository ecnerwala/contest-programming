#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	map<int, int> mp;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		mp[a]++;
	}
	int c = 0;
	for (auto it : mp) c = max(c, it.second);
	vector<int> res;
	for (auto it : mp) if (c == it.second) res.push_back(it.first);
	for (int i = 0; i < int(res.size()); i++) {
		cout << res[i] << " \n"[i+1==int(res.size())];
	}

	return 0;
}
