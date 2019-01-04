#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

inline ll sq(ll a) {
	return a * a;
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

ll sqsgn(ll a) {
	return sq(a) * sgn(a);
}

struct pt {
	ll x, y, h;
};

bool operator == (const pt &a, const pt &b) {
	return a.x == b.x && a.y == b.y && a.h == b.h;
}

bool operator != (const pt &a, const pt &b) {
	return !(a == b);
}

pt operator + (const pt &a, const pt &b) {
	return pt{a.x + b.x, a.y + b.y, a.h + b.h};
}

pt operator - (const pt &a, const pt &b) {
	return pt{a.x - b.x, a.y - b.y, a.h - b.h};
}

ll cross2d(const pt &a, const pt &b) {
	return a.x * b.y - a.y * b.x;
}

ll area2d(const pt &a, const pt &b, const pt &c) {
	return cross2d(b - a, c - a);
}

int dir2d(const pt &a, const pt &b, const pt &c) {
	return sgn(area2d(a,b,c));
}

ll dot2d(const pt &a, const pt &b) {
	return a.x * b.x + a.y * b.y;
}

const int MAXN = 2000;
int N;

pt P[MAXN];
bool used[MAXN];
int output[MAXN];

void go() {
	memset(used, false, sizeof(used));

	assert(N >= 2);

	{ // tallest point for point 0
		int best = 0;
		for (int i = 1; i < N; i++) {
			if (P[i].h > P[best].h) {
				best = i;
			}
		}
		output[0] = best;
		used[best] = true;
	}

	{
		int best = -1;
		for (int i = 0; i < N; i++) {
			if (used[i]) continue;

			if (best == -1) {
				best = i;
				continue;
			}
			pt bp = P[output[0]] - P[best];
			pt ip = P[output[0]] - P[i];
			assert(bp.h >= 0);
			assert(ip.h >= 0);
			__int128_t a = sq(bp.x) + sq(bp.y);
			__int128_t b = sq(ip.x) + sq(ip.y);
			a *= sq(ip.h);
			b *= sq(bp.h);
			if (a < b) {
				best = i;
			}
		}
		assert(best != -1);
		output[1] = best;
		used[best] = true;
	}

	for (int z = 2; z < N; z++) {
		pt a = P[output[z-2]];
		pt b = P[output[z-1]];

		ll nd = dot2d(a-b, a-b);

		int best = -1;
		for (int i = 0; i < N; i++) {
			if (used[i]) continue;
			if (best == -1) {
				best = i;
				continue;
			}

			if (dir2d(P[best], a, b) != dir2d(P[i], a, b)) {
				// we'll just look for the tallest thing on same side as best
				continue;
			}

			__int128_t vi = (P[i].h - b.h) * nd - (a.h - b.h) * dot2d(a-b, P[i]-b);
			__int128_t vb = (P[best].h - b.h) * nd - (a.h - b.h) * dot2d(a-b, P[best]-b);
			vi *= abs(area2d(P[best], a, b));
			vb *= abs(area2d(P[i], a, b));
			if (vi > vb) {
				best = i;
			}
		}


		output[z] = best;
		used[best] = true;
	}

	reverse(output, output + N);
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> N;
		for (int i = 0; i < N; i++) {
			cin >> P[i].x >> P[i].y >> P[i].h;
		}

		go();

		cout << "Case #" << case_num << ":";
		for (int i = 0; i < N; i++) {
			cout << ' ' << output[i] + 1;
		}
		cout << '\n';
	}

	return 0;
}
