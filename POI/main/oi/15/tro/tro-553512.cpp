#include<iostream>
#include<algorithm>
#include<complex>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;
typedef pair<double, pii> pt;

#define A first
#define B second

const double PI = atan2(0., -1.);
const double TAU = 2 * PI;

ll cross(pii a, pii b) {
	return a.A * b.B - a.B * b.A;
}

const int MAXN = 2e5;
int N;
ll X[MAXN], Y[MAXN];

const int MAXS = MAXN;
int S;
pt pts[MAXS * 2];

ll sumArea() {
	sort(pts, pts + S);
	for(int i = 0; i < S; i++) {
		pts[S + i] = pts[i];
		pts[S + i].A += TAU;
	}
	pii sumpts(0, 0);
	for(int i = 0; i < S; i++) {
		sumpts.A += pts[i].B.A;
		sumpts.B += pts[i].B.B;
	}
	ll res = 0;
	for(int j = S, i = 0; j < S * 2; j++) {
		while(pts[i].A + PI < pts[j].A) {
			sumpts.A -= pts[i].B.A;
			sumpts.B -= pts[i].B.B;
			i++;
		}
		res += cross(sumpts, pts[j].B);
		sumpts.A += pts[j].B.A;
		sumpts.B += pts[j].B.B;
	}
	cerr << res << '\n';
	return res;
}

ll go() {
	ll res = 0;
	S = N - 1;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(j == i) continue;
			pii val = pii(X[j] - X[i], Y[j] - Y[i]);
			pts[j - (j > i)] = pt(atan2(val.B, val.A), val);
		}
		res += sumArea();
	}
	cerr << res << '\n';
	assert(res % 3 == 0);
	res /= 3;
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> X[i] >> Y[i];
	}
	ll res = go();
	cout << res / 2 << '.' << ((res % 2) ? 5 : 0) << '\n';
	return 0;
}
