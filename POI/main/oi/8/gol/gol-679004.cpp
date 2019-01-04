#include<bits/stdc++.h>
using namespace std;

inline bool isPrime(int a) {
	for(int p = 2; p * p <= a; p++) {
		if(a % p == 0) return false;
	}
	return true;
}

int main() {
	int T; cin >> T;
	while(T--) {
		int N; cin >> N;
		vector<int> res;
		while(N) {
			if(N == 15) {
				N -= 15;
				res.push_back(7);
				res.push_back(5);
				res.push_back(3);
			} else if(N == 20) {
				N -= 20;
				res.push_back(17);
				res.push_back(3);
			} else if(N == 22) {
				N -= 22;
				res.push_back(17);
				res.push_back(5);
			} else if(N == 26) {
				N -= 26;
				res.push_back(19);
				res.push_back(7);
			} else if(isPrime(N)) {
				res.push_back(N);
				break;
			} else {
				assert(N >= 10);
				for(int i = (N + 2) / 2; i <= N; i++) {
					if(isPrime(i)) {
						res.push_back(i);
						N -= i;
						break;
					}
				}
			}
		}
		cout << res.size() << '\n';
		while(!res.empty()) {
			cout << res.back();
			res.pop_back();
			cout << (res.empty() ? '\n' : ' ');
		}
	}
}
