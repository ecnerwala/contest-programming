#include<bits/stdc++.h>
using namespace std;

typedef double ld;

const ld EPS = 1e-18;
const ld INF = 1e20;

const int MAXN = 400;

int N;

ld cost[MAXN][MAXN];
ld profit[MAXN][MAXN];

ld dist[MAXN][MAXN];

// returns true if you can make > avg
// i.e. sum profit > sum cost * avg
// i.e 0 > sum cost * avg - profit
bool good(ld avg) {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			dist[i][j] = cost[i][j] * avg - profit[i][j];
		}
	}
	for(int k = 0; k < N; k++) {
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
			if(dist[i][i] < -EPS) return true;
		}
	}
	return false;
}

ld go() {
	ld mi = 0; // you can make > mi
	ld ma = INF; // you can make <= ma
	for(int i = 0; i < 100; i++) {
		ld md = (mi + ma) / 2;
		if(good(md)) mi = md;
		else ma = md;
	}
	return ma;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin >> N;
  for(int i = 0; i < N; i++) {
	  for(int j = 0; j < N; j++) {
		  if(j == i) {
			  cost[i][j] = 0;
			  profit[i][j] = 0;
		  } else {
			  cin >> cost[i][j] >> profit[i][j];
		  }
	  }
  }
  printf("%.4lf\n", go());
  return 0;
}
