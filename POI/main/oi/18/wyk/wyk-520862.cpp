#include<iostream>
#include<iomanip>
#include<vector>
#include<complex>
#include<cassert>
#include<cstdlib>
#include<ctime>

using namespace std;

typedef complex<double> pt;
typedef pair<pt, double> circ;
const double EPS = 1e-9;

const int MAXN = 200000;
const double MAXX = 1e6;

int N, M; 
pt P[MAXN];

double D;
int S;
pt Q[MAXN];

template<class T> void shuffle(T *a, int n) {
	for(int i = 0; i < n; i++) {
		int x = rand() % n;
		int y = rand() % n;
		swap(a[x], a[y]);
	}
}

bool in(pt p, circ c) {
	return abs(c.first - p) <= c.second;
}

circ circumcircle(vector<pt> v) {
	if(v.size() == 0) {
		return circ(0, 0);
	} else if(v.size() == 1) {
		return circ(v[0], 0);
	} else if(v.size() == 2) {
		return circ((v[0] + v[1]) / 2., abs(v[0] - v[1]) / 2);
	} else {
		assert(v.size() == 3);
		pt c = norm(v[0]) * (v[2] - v[1]) + norm(v[1]) * (v[0] - v[2]) + norm(v[2]) * (v[1] - v[0]);
		pt div = (v[0] * conj(v[1]) - v[1] * conj(v[0])) + (v[1] * conj(v[2]) - v[2] * conj(v[1])) + (v[2] * conj(v[0]) - v[0] * conj(v[2]));
		//cerr << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
		assert(div != 0.);
		c /= div;
		double r = abs(v[0] - c);
		//cerr << c << ' ' << r << '\n';
		assert(abs(abs(v[1] - c) - r) <= EPS && abs(abs(v[2] - c) - r) <= EPS);
		return circ(c, r);
	}
}

pt K[MAXN];
vector<pt> r;
circ sed(int n) {
	if(!n || r.size() == 3) {
		return circumcircle(r);
	}

	n--;
	pt p = K[n];

	circ res = sed(n);

	if(!in(p, res)) {
		r.push_back(p);

		res = sed(n);

		r.pop_back();
	}

	return res;
}

circ getSed(int a, int len) {
	for(int i = 0; i < len; i++) {
		K[i] = P[a + i];
	}
	shuffle(K, len);

	assert(r.empty());
	//r.clear();

	return sed(len);
}

bool check(double d) {
	D = d;

	int cur = 0;
	for(S = 0; S < M; S++) {
		if(cur == N) break;

		int min = 0, max = 1;

		while(getSed(cur, max).second <= D) {
			min = max;
			max *= 2;
			if(cur + max > N) {
				max = N - cur + 1;
				break;
			}
		}

		while(max - min > 1) {
			int mid = (min + max) / 2;
			if(getSed(cur, mid).second <= D) {
				min = mid;
			} else {
				max = mid;
			}
		}

		Q[S] = getSed(cur, min).first;
		cur += min;
	}

	return cur == N;
}

void go() {
	if(check(0.0)) return;
	double min = 0, max = MAXX * 4;
	while(max - min > EPS) {
		double mid = (min + max) / 2;
		if(check(mid)) max = mid;
		else min = mid;
	}
	check(max);
}

int main() {
	ios_base::sync_with_stdio(0);

	cout << fixed << setprecision(15);

	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> P[i].real() >> P[i].imag();
	}

	go();

	cout << D << '\n';
	cout << S << '\n';
	for(int i = 0; i < S; i++) {
		cout << Q[i].real() << ' ' << Q[i].imag() << '\n';
	}
}
