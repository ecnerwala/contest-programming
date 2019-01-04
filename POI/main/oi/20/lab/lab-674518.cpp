#include<bits/stdc++.h>
using namespace std;

typedef complex<int> pt;
#define X real()
#define Y imag()

// turns
const pt T[2] = {pt(0, 1), pt(0, -1)}; // 1 is right

struct op {
	pt m;
	pt c;

	pt operator () (pt p) {
		return m * p + c;
	}

	op operator * (op a) {
		op res(a);
		res.m *= m;
		res.c *= m;
		res.c += c;
		return res;
	}

	op(pt mult, pt cons) : m(mult), c(cons) {}
};

struct path {
	path *a, *b, *c;
	bool d; // 0 is aLbRc, 1 is aRbLc

	int w, l; // width and length

	// empty path
	path () : a(NULL), b(NULL), c(NULL), d(0), w(0), l(1) { }

	path ( path * x, path * y, path * z, bool dir ) : a(x), b(y), c(z), d(dir) {
		assert(a);
		assert(b);
		assert(c);
		l = a->l + b->w + c->l;
		w = a->w + b->l + c->w;
	}

	void apply(op o, vector<pt> &res) {
		if(!a) {
			res.push_back(o(0));
		} else {
			op ao(1, pt(0, d ? b->l + c->w : 0));
			a->apply(o * ao, res);
			op bo(T[d], d ? pt(a->l, b->l + c->w) : pt(a->l + b->w, a->w));
			b->apply(o * bo, res);
			op co(1, pt(0, d ? 0 : a->w + b->l));
			c->apply(o * co, res);
		}
	}
};

const int MAXN = 3e5;

int N;
char S[MAXN];

int main() {
	scanf("%s", S);
	N = int(strlen(S));
	memcpy(S + N, S, N);

	int sum = 0;
	for(int i = 0; i < N; i++) {
		sum += (S[i] == 'L') ? 1 : -1;
	}
	if(sum != 4) {
		printf("NIE\n");
		return 0;
	}

	int s = 0;
	while(S[s] != 'L') s++;

	assert(s < N);

	for(int i = 0; i < N; i++) {
		S[i] = S[i + s];
	}

	memcpy(S + N, S, N);

	vector<pair<bool, path*> > segs;

	for(int i = 0; i < N; i++) {
		bool d = (S[i] != 'L');
		path* cur = new path();
		while(int(segs.size()) >= 2 && segs.back().first != d) {
			path* prv = segs.back().second;
			segs.pop_back();
			cur = new path (segs.back().second, prv, cur, !d);
			d = segs.back().first;
			segs.pop_back();
		}

		segs.push_back(make_pair(d, cur));
	}

	assert(segs.size() == 4);

	vector<path*> sides;
	for(int i = 0; i < 4; i++) {
		assert(segs[i].first == 0);
		sides.push_back(segs[i].second);
	}

	vector<pt> res;
	sides[0]->apply(op(1, pt(0, -sides[0]->w)), res);
	sides[1]->apply(op(T[0], pt(sides[1]->w + max(sides[0]->l, sides[2]->l), 0)), res);
	sides[2]->apply(op(-1, pt(sides[2]->l, max(sides[1]->l, sides[3]->l) + sides[2]->w)), res);
	sides[3]->apply(op(T[1], pt(-sides[3]->w, sides[3]->l)), res);

	// 1.X, 0.Y
	// 1.X, 2.Y
	for(int i = 0; i < N; i++) {
		int x = (i % 2 ? res[i].X : res[(i + 1) % N].X);
		int y = (i % 2 ? res[(i + 1) % N].Y : res[i].Y);
		printf("%d %d\n", x, y);
	}

	return 0;
}
