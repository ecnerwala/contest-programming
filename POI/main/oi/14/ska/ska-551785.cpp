#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 10;

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

ll peri;
ll cost;

void go() {
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
	int besti, bestj;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			ll v = check(mimi, mami, mima, mama);
			if(v != BAD) {
				if(v < minCost) {
					minCost = v;
					besti = i, bestj = j;
				}
			}
			swap(mami, mama);
		}
		swap(mimi, mima);
	}

	peri = 2 * (mama + mami - mima - mimi);
	cost = minCost;

	if(besti) swap(mimi, mima);
	if(bestj) swap(mami, mama);
	check(mimi, mami, mima, mama);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> X[i] >> Y[i] >> M[i];
	}
	go();
	cout << peri << ' ' << cost << '\n';
	for(int i = 0; i < N; i++) cout << swp[i]; cout << '\n';
	return 0;
}
