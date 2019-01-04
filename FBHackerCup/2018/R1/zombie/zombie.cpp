#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int minv(int a, int m) {
	assert(m);
	assert(a > 0);
	a %= m;
	assert(a);
	return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
}

struct num {
	static const int MOD = ll(1e9) + 7;

	int v;

	num() : v(0) {}

	num(int v_) : v(v_) {
		v %= MOD;
		if (v < 0) v += MOD;
	}

	num operator - () const {
		return num{v ? MOD-v : 0};
	}

	num inv() const {
		return num{minv(v, MOD)};
	}

	num& operator += (const num &o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}

	num& operator -= (const num &o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}

	num& operator *= (const num &o) {
		v = int(ll(v) * ll(o.v) % ll(MOD));
		return *this;
	}

	num& operator /= (const num &o) {
		return operator *= (o.inv());
	}

	explicit operator int() const {
		return v;
	}
};
num operator + (const num &a, const num &b) {
	return num(a) += b;
}
num operator - (const num &a, const num &b) {
	return num(a) -= b;
}
num operator * (const num &a, const num &b) {
	return num(a) *= b;
}
num operator / (const num &a, const num &b) {
	return num(a) /= b;
}
ostream& operator << (ostream& out, const num &n) {
	return out << int(n);
}
bool operator == (const num &a, const num &b) {
	return a.v == b.v;
}
bool operator != (const num &a, const num &b) {
	return a.v != b.v;
}

const int MAXN = 4000;
int N;
int A[MAXN][2]; // bounds on fence height

int H[MAXN];

const int MAXM = 4000;
int M;

vector<int> heights;

num dp_extra[MAXM];
num dp_need[MAXM];

void apply_yard(int h) {
	assert(0 <= h && h < M);
	if (h) {
		// everything from need_h to extra_h gets dumped into extra_h
		for (int i = 0; i < h; i++) {
			dp_extra[h] += dp_extra[i];
			dp_extra[i] = 0;
		}
		for (int i = 1; i <= h; i++) {
			dp_extra[h] += dp_need[i];
			dp_need[i] = 0;
		}
	} else {
		// we don't have a zombie
		// previous extra stays extra, previous need stays need
		// previous 0 becomes a need
		dp_need[1] += dp_extra[0];
		dp_extra[0] = 0;
	}
}

void apply_fence(int a, int b) {
	// fence is (a,b]
	assert(a < b);
	num denom = num(b-a).inv();
	// from 0 -> 0
	// we can go from zombie -> no-zombie
	for (int i = 1; i < M; i++) {
		int h = heights[size_t(i)];
		num pmake = num(max(min(h,b)-a, 0)) * denom;
		dp_extra[0] += (1-pmake) * dp_extra[i];
		dp_extra[i] = pmake * dp_extra[i];
	}
	// we can go from need -> bigger need

	// transform to we need something <= i
	for (int i = 2; i < M; i++) {
		dp_need[i] += dp_need[i-1];
	}
	for (int i = 1; i < M; i++) {
		int h = heights[size_t(i)];
		num pmake = num(max(min(h,b)-a, 0)) * denom;
		dp_need[i] *= pmake;
	}

	// transform from we need something <= i
	for (int i = M-1; i > 1; i--) {
		dp_need[i] -= dp_need[i-1];
	}
}

num go() {
	cin >> N >> M;
	assert(1 <= N && N < MAXN);
	assert(1 <= M && M < MAXM);
	for (int i = 1; i < N; i++) {
		cin >> A[i][0] >> A[i][1];
		assert(A[i][0] <= A[i][1]);
		assert(A[i][0] > 0);
		A[i][0] --;
	}
	for (int i = 0; i < N; i++) {
		H[i] = 0;
	}
	for (int j = 0; j < M; j++) {
		int y, h; cin >> y >> h;
		y--;
		assert(0 <= y && y < N);
		assert(h >= 1);
		H[y] = max(H[y], h);
	}
	heights.clear();
	for (int i = 0; i < N; i++) {
		heights.push_back(H[i]);
	}
	sort(heights.begin(), heights.end());
	heights.resize(size_t(unique(heights.begin(), heights.end()) - heights.begin()));

	assert(heights.size());
	assert(heights[0] == 0);
	M = int(heights.size());

	if (M == 1) {
		// we have no zombies
		return 1;
	}
	assert(M >= 2);

	for (int i = 0; i < N; i++) {
		H[i] = int(lower_bound(heights.begin(), heights.end(), H[i]) - heights.begin());
	}

	for (int i = 0; i < M; i++) {
		dp_extra[i] = 0;
	}
	for (int i = 1; i < M; i++) {
		dp_need[i] = 0;
	}
	dp_extra[0] += 1;
	apply_yard(H[0]);

	for (int i = 1; i < N; i++) {
		apply_fence(A[i][0], A[i][1]);
		apply_yard(H[i]);
	}

	assert(dp_extra[0] == 0);

	num res = 0;
	for (int i = 0; i < M; i++) {
		res += dp_extra[i];
	}
	return 1-res;
}

int main() {
	int NUM_CASES; cin >> NUM_CASES;
	for (int case_num = 1; case_num <= NUM_CASES; case_num ++) {
		num res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
