#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pt;
#define A first
#define B second

const int MAXN = 4e5;

int N;

ll cross(pt a, pt b) {
	return a.A * b.B - a.B * b.A;
}

ll dot(pt a, pt b) {
	return a.A * b.A + a.B * b.B;
}

pt R[2]; // 2 rays

pt P[MAXN];

int K[MAXN];

int T[MAXN];

bool L; // linear

void compress() {
	vector<ll> xvals, yvals;
	
	for(int i = 0; i < N; i++) {
		xvals.push_back(P[i].A);
		yvals.push_back(P[i].B);
	}

	sort(xvals.begin(), xvals.end());
	sort(yvals.begin(), yvals.end());

	xvals.resize(unique(xvals.begin(), xvals.end()) - xvals.begin());
	yvals.resize(unique(yvals.begin(), yvals.end()) - yvals.begin());

	for(int i = 0; i < N; i++) {
		P[i].A = lower_bound(xvals.begin(), xvals.end(), P[i].A) - xvals.begin():
		P[i].B = lower_bound(yvals.begin(), yvals.end(), P[i].B) - yvals.begin():
	}
}

void go() {
	if(cross(R[0], R[1]) == 0) {
		L = true;

		assert(dot(R[0], R[1]) > 0);

		for(int i = 0; i < N; i++) {
			ll x = dot(R[0], P[i]);
			ll y = cross(R[0], P[i]);
			P[i] = pt(x, y);
		}
	} else {

		L = false;

		if(cross(R[0], R[1]) < 0) {
			swap(R[0], R[1]);
			assert(cross(R[0], R[1]) > 0);
		}

		for(int i = 0; i < N; i++) {
			ll x = cross(P[i], R[1]);
			ll y = cross(R[0], P[i]);
			P[i] = pt(x, y);
		}
	}

	compress();

}


int main() {
	scanf("%d", &N);
	scanf("%lld%lld%lld%lld", &R[0].A, &R[0].B, &R[1].A, &R[1].B);
	for(int i = 0; i < N; i++) {
		scanf("%lld%lld", &P[i].A, &P[i].B);
	}

	for(int i = 0; i < N; i++) {
		scanf("%d", &K[i]);
	}

	go();

	assert(N >= 1);
	printf("%d", T[0] + 1);
	for(int i = 1; i < N; i++) {
		printf(" %d", T[i] + 1);
	}
	printf("\n");

	return 0;
}
