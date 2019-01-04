#include<bits/stdc++.h>
using namespace std;

template <typename T> T gcd(T a, T b) {
	while (a) {
		T t = b % a;
		b = a;
		a = t;
	}
	return b >= 0 ? b : -b;
}

using ll = long long;
struct angle {
	using coord_t = ll;
	using big_t = __int128_t;
	coord_t x, y;
	angle() : x(1), y(0) {}
	angle (coord_t a, coord_t b) : x(a), y(b) {
		assert(x || y);
		coord_t g = gcd(x,y);
		x /= g, y /= g;
	}
	int quadrant() const {
		if (x > 0 && y >= 0) {
			return 0;
		}
		if (x <= 0 && y > 0) {
			return 1;
		}
		if (x < 0 && y <= 0) {
			return 2;
		}
		if (x >= 0 && y < 0) {
			return 3;
		}
		assert(false);
	}
	angle operator - () const {
		angle res;
		res.x = x;
		res.y = -y;
		return res;
	}
	friend angle operator + (const angle &a , const angle &b) {
		return angle(a.x * b.x - a.y * b.y , a.x * b.y + a.y * b.x);
	}
	friend angle operator - (const angle &a , const angle &b) { return a + (-b); }
	angle& operator += (const angle &o) { return *this = *this + o; }
	angle& operator -= (const angle &o) { return *this = *this - o; }
	friend bool operator == (const angle &a, const angle &b) {
		return a.x == b.x && a.y == b.y;
	}
	friend bool operator != (const angle &a, const angle &b) { return !(a == b); }
	friend bool operator < (const angle &a, const angle &b) {
		int qa = a.quadrant();
		int qb = b.quadrant();
		if (qa != qb) return qa < qb;
		// we're in the same quadrant
		// comparisons must use big-ass integers
		return big_t(a.y) * big_t(b.x) < big_t(b.y) * big_t(a.x);
	}
	friend bool operator > (const angle &a, const angle &b) { return b < a; }
	friend bool operator <= (const angle &a, const angle &b) { return !(a > b); }
	friend bool operator >= (const angle &a, const angle &b) { return b <= a; }
	friend std::ostream& operator << (std::ostream& out, const angle &a) {
		return out << '(' << a.x << ',' << a.y << ')';
	}
	explicit operator double () const {
		return atan2(-y, -x) + M_PI;
	}
};
const angle A0(1, 0);
const angle A180(-1, 0);

const int MAXN = 1.1e4;
int N;
pair<int, int> T[MAXN];
angle A[MAXN][3];

const int MAXM = MAXN + 2;
int M;

const int LOGS = 14;
const int S = 1 << LOGS;

struct node {
	double v; // true sum
	double lazy; // lazy for the children
};
node seg[S * 2];
void reset() {
	for (int i = 0; i < 2 * S; i++) {
		seg[i].v = 0;
		seg[i].lazy = 1;
	}
}

void propagate(int i) {
	double l = seg[i].lazy;
	seg[i].lazy = 1;
	if (i < S) {
		seg[2*i+0].v *= l;
		seg[2*i+0].lazy *= l;
		seg[2*i+1].v *= l;
		seg[2*i+1].lazy *= l;
	}
}
void propagate_all(int i) {
	assert(i);
	if (i / 2) propagate_all(i / 2);
	propagate(i);
}

// halve the range, inclusive
// return the sum
double halve(int a, int b) {
	assert(a <= b);
	if (a-1 >= 0) propagate_all(a-1+S);
	if (b+1 < S) propagate_all(b+1+S);
	double res = 0;
	for (int l = a+S, r = b+S; l <= r; l /= 2, r /= 2) {
		assert(l);
		assert(r);
		if (l % 2 == 1) {
			seg[l].lazy *= 0.5;
			seg[l].v *= 0.5;
			res += seg[l].v;
			l++;
		}
		if (r % 2 == 0) {
			seg[r].lazy *= 0.5;
			seg[r].v *= 0.5;
			res += seg[r].v;
			r--;
		}
		assert(l%2==0);
		assert(r%2==1);
	}
	if (a-1 >= 0) {
		for (int l = (a-1+S) / 2; l; l /= 2) {
			seg[l].v = (seg[2*l].v + seg[2*l+1].v) * seg[l].lazy;
		}
	}
	if (b+1 < S) {
		for (int r = (b+1+S) / 2; r; r /= 2) {
			seg[r].v = (seg[2*r].v + seg[2*r+1].v) * seg[r].lazy;
		}
	}
	return res;
}

void update(int a, double v) {
	propagate_all(a+S);
	for (int i = a + S; i; i /= 2) {
		seg[i].v += v;
	}
}

double go() {
	bool covers180 = false;
	angle hi = A180;
	bool covers0 = false;
	angle lo = A0;
	for (int i = 0; i < N; i++) {
		angle diff = A[i][2] - A[i][1];
		assert(diff != A180); // we don't lie on the line
		if (diff < A180) {
			// this is good
		} else {
			swap(A[i][1], A[i][2]);
			diff = -diff;
		}
		// covered area is from A[i][1] to A[i][2]
		assert(diff == A[i][2] - A[i][1]);
		assert(diff < A180);

		A[i][1] -= A[i][0];
		A[i][2] -= A[i][0];

		// now we'll normalize them
		if (A180 - A[i][1] <= diff) {
			// we touch 180
			covers180 = true;
			A[i][2] = -A[i][2];
			if (A[i][2] < A[i][1]) swap(A[i][1], A[i][2]);
			hi = min(hi, A[i][2]);
		} else if (A0 - A[i][1] <= diff) {
			assert(A[i][2] < A180);
			assert(-A[i][1] < A180);
			// we touch 0
			covers0 = true;
			A[i][1] = -A[i][1];
			if (A[i][2] < A[i][1]) swap(A[i][1], A[i][2]);
			lo = max(lo, A[i][1]);
		} else if (A[i][1] > A180 && A[i][2] > A180) {
			A[i][1] = -A[i][1];
			A[i][2] = -A[i][2];
			swap(A[i][1], A[i][2]);
		}

		assert(A0 <= A[i][1] && A[i][1] <= A180);
		assert(A0 <= A[i][2] && A[i][2] <= A180);
		assert(A[i][1] <= A[i][2]);
	}
	if (!covers180 || !covers0) {
		return 1.;
	}
	assert(A0 <= lo && lo < A180);
	assert(A0 < hi && hi <= A180);
	if (hi <= lo) {
		// we're done
		return 0.;
	}
	assert(hi > lo);
	for (int i = 0; i < N; i++) {
		for (int z = 1; z <= 2; z++) {
			if (A[i][z] > hi) A[i][z] = hi;
			else if (A[i][z] < lo) A[i][z] = lo;
		}
	}

	vector<angle> angles;
	angles.push_back(lo);
	angles.push_back(hi);
	for (int i = 0; i < N; i++) {
		angles.push_back(A[i][1]);
	}

	sort(angles.begin(), angles.end());
	angles.resize(size_t(unique(angles.begin(), angles.end()) - angles.begin()));
	assert(!angles.empty());
	assert(angles.front() == lo);
	assert(angles.back() == hi);
	M = int(angles.size()) - 1;
	assert(M >= 1); // hi can't be lo
	for (int i = 0; i < N; i++) {
		int a = int(lower_bound(angles.begin(), angles.end(), A[i][1]) - angles.begin());
		int b = int(upper_bound(angles.begin(), angles.end(), A[i][2]) - angles.begin()) - 1;
		assert(b >= a);
		T[i] = pair<int, int>(a,b);
	}
	sort(T, T + N);

	//cerr << M << '\n';
	//for (int i = 0; i < N; i++) cerr << T[i].first << ' ' << T[i].second << '\n';

	/*
	// we'll solve it with a slow for now
	int res = 0;
	for (int msk = 0; msk < (1 << N); msk++) {
		bool good = true;
		int a[2] = {0,0};
		for (int i = 0; i < N; i++) {
			int z = bool(msk & (1 << i));
			if (T[i].first <= a[z]) {
				a[z] = max(a[z], T[i].second);
			} else {
				good = false;
				break;
			}
		}
		if (good && a[0] == M && a[1] == M) {
			res++;
		}
	}
	return 1 - res / pow(2, N);
	*/

	int R = 0;
	reset();
	update(0, 1);
	for (int i = 0; i < N; i++) {
		assert(T[i].second <= M);
		if (T[i].first > R) {
			return 1;
		}
		// (a,R) => (a, max(T[i].second, R)) or (max(T[i].second, a), R)
		// option 1: you put T[i] on the R side, so all the probability mass halves and R increases
		// option 2: you put T[i] on the smaller side.
		// For all T[i].first <= a <= T[i].second, the mass moves to min(T[i].second, R), otherwise it stays put
		// (All mass < T[i].first dies, but we can just ignore that mass.)
		// Our operation is thus v = halve(T[i].first, min(T[i].second, R)), update(min(T[i].second, R), v), R = max(R, T[i].second)
		double v = halve(T[i].first, min(T[i].second, R));
		update(min(T[i].second, R), v);
		R = max(R, T[i].second);
	}
	return 1 - halve(M, M) * 2;
}

int main() {
	ios_base::sync_with_stdio(0);
	int NUM_CASES; cin >> NUM_CASES;

	for (int case_num = 1; case_num <= NUM_CASES; case_num ++) {
		cin >> N;
		for (int i = 0; i < N; i++) {
			int x,y,dx,dy; cin >> x >> y >> dx >> dy;
			A[i][0] = angle(dx-x, dy-y);
			A[i][1] = angle(0-x,0-y);
			A[i][2] = angle(0-x,1000-y);
		}

		//cerr << "Case #" << case_num << ":" << '\n';
		double res = go();
		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(8) << res << '\n';
	}

	return 0;
}
