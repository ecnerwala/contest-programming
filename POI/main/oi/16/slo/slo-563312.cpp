#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 20;
const ll MAXW = 1e4;

int N;
ll W[MAXN];
int A[MAXN];
int B[MAXN];
int P[MAXN];

bool vis[MAXN];

int go() {
	for(int i = 0; i < N; i++) {
		P[A[i]] = B[i];
	}

	int mi = W[0];
	for(int i = 1; i < N; i++) {
		if(W[i] < mi)  mi = W[i];
	}

	ll res = 0;

	for(int i = 0; i < N; i++) {
		if(vis[i]) continue;
		ll sum = 0;
		ll cnt = 0;
		ll low = MAXW;
		for(int cur = P[i]; true; cur = P[cur]) {
			vis[cur] = true;
			if(W[cur] < low) low = W[cur];
			cnt++;
			sum += W[cur];
			if(cur == i) break;
		}
		if(cnt == 1) {
			continue;
		}

		ll c1 = 0, c2 = low + mi;
		c1 += sum - low + (cnt - 1) * low;
		sum += mi;
		cnt++;
		low = mi;
		c2 += sum - low + (cnt - 1) * low;
		res += min(c1, c2);
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> W[i];
	}

	for(int i = 0; i < N; i++) {
		cin >> A[i]; A[i] --;
	}
	for(int i = 0; i < N; i++) {
		cin >> B[i]; B[i] --;
	}

	ll res = go();

	cout << res << '\n';
	return 0;
}
