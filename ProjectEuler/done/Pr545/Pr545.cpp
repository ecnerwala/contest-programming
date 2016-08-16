#include<bits/stdc++.h>
using namespace std;

const int K = 20010; // 2 * 3 * 5 * 23 * 29
const int M = 308; // min value which could work, lcm of primes - 1

const int V = 1e9;
const int MAXV = 1e9 + 20;

bool isPrime[MAXV];
bool isGood[MAXV];

int main() {
	for(int i = 1; i <= V; i++) {
		isGood[i] = (i % M == 0);
		isPrime[i] = true;
	}
	isGood[1] = isPrime[1] = false;
	int cnt = 0;
	for(int i = 1; i <= V; i++) {
		if(isPrime[i]) {
			for(int j = i + i; j <= V; j += i) {
				isPrime[j] = false;
			}
			if(K % i != 0) {
				for(int j = i - 1; j <= V; j += i - 1) {
					isGood[j] = false;
				}
			}
		}
		if(isGood[i] && !isPrime[i+1]) {
			cnt++;
			if(cnt == 1) assert(i == 308);
			if(cnt == 10) assert(i == 96404);
			if(cnt == 100000) {
				cout << i << '\n';
				return 0;
			}
		}
	}
	cerr << "Only " << cnt << " found" << '\n';
	return 0;
}
