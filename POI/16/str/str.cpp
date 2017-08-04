#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;

const int INF = int(1e9) + 200;

int N, M;

const int MAXZ = int(1e5) + 200;
int Z;
pii pts[MAXZ]; // sorted by x

// Vector, but static MAX_SIZE
template <typename T, size_t MAX_SIZE> struct arr {
	size_t S;
	T vals[MAX_SIZE];

	arr() : S(0) {}

	size_t size() const { return S; }

	typedef T* iterator;
	iterator begin() { return vals; }
	iterator end() { return vals + S; }
	typedef const T* const_iterator;
	const_iterator cbegin() const { return (const T*) vals; }
	const_iterator cend() const { return (const T*) vals + S; }

	T& operator[](size_t pos) { return vals[pos]; }
	const T& operator[](size_t pos) const { return vals[pos]; }

	void resize(size_t s) { S = s; }
	void resize(iterator end) { S = end - vals; }

	void push_back(const T& t) { vals[S++] = t; }
};

// Frozen set: you can add, the compile, then get value
template <typename T, class Container = std::vector<T> > struct coordset {
	Container coords;
	void add(const T& a) {
		coords.push_back(a);
	}
	void compile() {
		sort(coords.begin(), coords.end());
		coords.resize(unique(coords.begin(), coords.end()));
	}
	// equal range
	pair<int, int> get(const T& a) {
		pair<T*, T*> res = equal_range(coords.begin(), coords.end(), a);
		return pair<int, int>(res.first - coords.begin(), res.second - coords.begin());
	}

	pair<int, int> all() {
		return pair<int, int>(0, coords.size());
	}
};

template <typename T, int L> struct range_count {
	static const int S = (1 << L);
	int seg[2 * S];
	void increment(int a) {
		for (int i = a + S; i > 0; i /= 2) {
			seg[i] ++;
		}
	}
	// [a,b] inclusive
	int query(int a, int b) {
		int res = 0;
		for (int l = a+S, r = b+S; l <= r; l /= 2, r /= 2) {
			if ((l & 1) == 1) {
				res += seg[l];
				l++;
			}
			if ((r & 1) == 0) {
				res += seg[r];
				r--;
			}
		}
		return res;
	}
};

const int MAXP = int(1e5) + 200;
int P;
pii Q[MAXP][2];
int res[MAXP][3];

// [-inf, time), [query.a, query.b)
typedef pair<int, int> pii;
struct seg_event {
	int time;
	pii query;
	int* res_loc;
	int fac;
	bool operator < (const seg_event& o) const {
		return time < o.time;
	}
};

struct AXIS { enum { X, Y, P, N }; };
arr<seg_event, MAXP> evts[4];

int main() {
	scanf("%d %d %d %d", &N, &M, &Z, &P);
	for (int i = 0; i < Z; i++) {
		int x, y;
		scanf("%d %d", &x, &y);
		pts[i] = pii(x, y);
	}

	sort(pts, pts + Z);

	for (int i = 0; i < P; i++) {
		scanf("%d %d %d %d", &Q[i][0].first, &Q[i][0].second, &Q[i][1].first, &Q[i][1].second);
		if (Q[i][0] == Q[i][1]) {
			res[i][0] = res[i][1] = 0;
			res[i][2] = Z;
			continue;
		}
		if (Q[i][0].first == Q[i][1].first) {
			evts[AXIS::Y].push_back(
				seg_event {
					INF,
					pii(),
					&res[i][0],
					1,
				}
			);
		}
	}

	for (int i = 0; i < P; i++) {
		printf("%d %d %d\n", res[i][0], res[i][1], res[i][2]);
	}
	return 0;
}
