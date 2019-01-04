#include<iostream>

using namespace std;

int main() {
	int T; cin >> T;
	for(int t = 0; t < T; t++) {
		int n; cin >> n;
		int v = 0;
		for(int i = -(n % 2); i < n; i+=2) {
			int a, b;
			if(i < 0) a = 0;
			else cin >> a;
			cin >> b;
			v ^= b - a;
		}
		cout << (v ? "TAK" : "NIE") << '\n';
	}
}
