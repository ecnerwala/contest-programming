#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int K, R; cin >> K >> R;
	cout << R + 100 * max(0, 10-K) << '\n';
	return 0;
}
