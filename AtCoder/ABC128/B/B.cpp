#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<pair<pair<string, int>, int>> V(N);
	for (int i = 0; i < N; i++) {
		cin >> V[i].first.first;
		cin >> V[i].first.second;
		V[i].first.second *= -1;
		V[i].second = i+1;
	}
	sort(V.begin(), V.end());
	for (const auto& it : V) cout << it.second << '\n';

	return 0;
}
