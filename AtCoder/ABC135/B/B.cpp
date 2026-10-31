#include<bits/stdc++.h>
using namespace std;

const int MAXN = 60;
int N;
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	int bads = 0;
	for (int i = 0; i < N; i++) {
		int v; cin >> v; v--;
		bads += (v != i);
	}
	cout << ((bads == 0 || bads == 2) ? "YES" : "NO") << '\n';

	return 0;
}
