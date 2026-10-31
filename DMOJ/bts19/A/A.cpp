#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	unordered_map<int, string> subs;
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		subs.emplace(int(s.size()), s);
	}
	string w; cin >> w;
	for (int v = int(w.size()); true; v--) {
		if (subs.count(v)) {
			cout << subs[v] << '\n';
			return 0;
		}
	}

	return 0;
}
