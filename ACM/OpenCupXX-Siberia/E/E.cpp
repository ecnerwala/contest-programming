#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	ll V = 0;
	for (int i = 0; i < N; i++) {
		ll a; cin >> a;
		V = (V / a + 1) * a;
		cout << V << " \n"[i+1==N];
	}

	return 0;
}
