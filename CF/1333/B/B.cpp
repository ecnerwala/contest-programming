#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N);
		for (int& a : A) cin >> a;
		bool seen[3] = {false,true,false};
		bool good = true;
		for (int a : A) {
			int b; cin >> b; b -= a;
			good = good && seen[(b>0)+(b>=0)];
			seen[a+1] = true;
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
