#include<iostream>

using namespace std;

const int MAXN = 2e6;

const int INF = 2e9 + 7;
const int BAD = -1;

int N;
int X[MAXN], Y[MAXN];
int M[MAXN];

bool swp[MAXN];

// inclusive
int check(int minx, int maxx, int miny, int maxy) {
	int res = 0;
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

pair<int, int> go() {
	int mimi = INF, mami = -1, mima = INF, mama = -1;

	for(int i = 0; i < N; i++) {
		int a = min(X[i], Y[i]);
		int b = max(X[i], Y[i]);
		if(a < mimi) mimi = a;
		if(a > mami) mami = a;
		if(b < mima) mima = b;
		if(b > mama) mama = b;
	}

	int minCost = INF;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			int v = check(mimi, mami, mima, mama);
			if(v != BAD) {
				if(v < minCost) minCost = v;
			}
			swap(mami, mama);
		}
		swap(mimi, mima);
	}

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			int v = check(mimi, mami, mima, mama);
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
	pair<int, int> res = go();
	cout << res.first << ' ' << res.second << '\n';
	for(int i = 0; i < N; i++) cout << swp[i]; cout << '\n';
}
