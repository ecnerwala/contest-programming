#include<iostream>
#include<algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 20;
int N;
ll K[MAXN];

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> K[i];
	}

	sort(K, K + N);
	N = unique(K, K + N) - K;

	ll val = 0;
	for(int i = 0; i < N; i++) {
		val = max(val, K[i] - val);
	}

	cout << val << '\n';
	return 0;
}
