#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 2e6;

const ll INF = 2e9 + 7;
const ll BAD = -1;

int N;
ll X[MAXN], Y[MAXN];
ll M[MAXN];

bool swp[MAXN];

// inclusive
ll check(ll minx, ll maxx, ll miny, ll maxy) {
	ll res = 0;
	for(int i = 0; i < N; i++) {
		if(minx <= X[i] && X[i] <= maxx && miny <= Y[i] && Y[i] <= maxy) {
			swp[i] = false;
		} else if(minx <= Y[i] && Y[i] <= maxx && miny <= X[i] && X[i] <= maxy) {
			swp[i] = true;
			res += M[i];
		} else {
			return BAD;
		}
	}
	return res;
}

pair<ll, ll> go() {
	ll mimi = INF, mami = -1, mima = INF, mama = -1;

	for(int i = 0; i < N; i++) {
		ll a = min(X[i], Y[i]);
		ll b = max(X[i], Y[i]);
		if(a < mimi) mimi = a;
		if(a > mami) mami = a;
		if(b < mima) mima = b;
		if(b > mama) mama = b;
	}

	ll minCost = INF;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			ll v = check(mimi, mami, mima, mama);
			if(v != BAD) {
				if(v < minCost) minCost = v;
			}
			swap(mami, mama);
		}
		swap(mimi, mima);
	}

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			ll v = check(mimi, mami, mima, mama);
			if(v == minCost) {
				return make_pair(2 * (mama + mami - mima - mimi), minCost);
			}
			swap(mami, mama);
		}
		swap(mimi, mima);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> X[i] >> Y[i] >> M[i];
	}
	pair<ll, ll> res = go();
	cout << res.first << ' ' << res.second << '\n';
	for(int i = 0; i < N; i++) cout << swp[i]; cout << '\n';
	return 0;
}
