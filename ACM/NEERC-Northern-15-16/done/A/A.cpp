#include<bits/stdc++.h>
using namespace std;

int main() {
	if(fopen("alex.in", "r")) {
		freopen("alex.in", "r", stdin);
		freopen("alex.out", "w", stdout);
	}
	long double h, w; cin >> h >> w;
	cout << fixed << setprecision(10) << max(max(min(w, h / 3), min(h, w / 3)), min(h / 2, w / 2)) << '\n';
}
