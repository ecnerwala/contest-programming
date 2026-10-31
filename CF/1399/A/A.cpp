#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		set<int> A;
		while (N--) {
			int v; cin >> v;
			A.insert(v);
		}
		cout << (int(A.size()) == *A.rbegin() - *A.begin() + 1 ? "YES" : "NO") << '\n';
	}

	return 0;
}
