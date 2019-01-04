#include<iostream>

using namespace std;

int main() {
	int T; cin >> T;
	for(int t = 0; t < T; t++) {
		int n; cin >> n;
		int v = 0;
		for(int i = 0; i < n; i++) {
			int a, b; cin >> a;
			if(i + 1 < n) cin >> b;
			else b = 0;
			v ^= a - b;
		}
		cout << (v ? "TAK" : "NIE") << '\n';
	}
}
