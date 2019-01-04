#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

struct pt {
	int x,y;
	pt& operator += (const pt &o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	pt operator + (const pt &o) const {
		return pt(*this) += o;
	}
	pt& operator -= (const pt &o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}
	pt operator - (const pt &o) const {
		return pt(*this) -= o;
	}
};
bool operator == (const pt &a, const pt &b) {
	return a.x == b.x && a.y == b.y;
}
bool operator != (const pt &a, const pt &b) {
	return !(a == b);
}
bool operator < (const pt &a, const pt &b) {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}
bool operator > (const pt &a, const pt &b) {
	return b < a;
}
bool operator <= (const pt &a, const pt &b) {
	return a.x < b.x || (a.x == b.x && a.y <= b.y);
}
bool operator >= (const pt &a, const pt &b) {
	return b <= a;
}

constexpr pt P[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
struct dir {

	int v;
	dir& operator ++ () {
		v += 1;
		v &= 3;
		return *this;
	}
	dir operator ++ (int) {
		dir res = *this;
		operator++();
		return res;
	}
	dir& operator -- () {
		v += 3;
		v &= 3;
		return *this;
	}
	dir operator -- (int) {
		dir res = *this;
		operator--();
		return res;
	}
	dir operator - () const {
		return dir{v ^ 2};
	}
	operator pt() const {
		return P[v];
	}
};
bool operator == (const dir &a, const dir &b) {
	return a.v == b.v;
}
bool operator < (const dir &a, const dir &b) {
	return a.v < b.v;
}

using seg = pair<pt, dir>;
seg rev(const seg &a) {
	return seg{a.first + pt(a.second), -a.second};
}

ll value(const seg &a) {
	// return y dx
	return a.first.y * pt(a.second).x;
}

class PolylineAreas {

	string expand(const string &prog, size_t &ind) {
		string res;
		int depth = 0;
		while (ind < prog.size()) {
			char c = prog[ind];
			ind ++;
			if (c == ']') {
				if (depth == 0) break;
				depth --;
			} else if ('0' <= c && c <= '9') {
				int cnt = (c - '0');
				while ('0' <= prog[ind] && prog[ind] <= '9') {
					cnt *= 10;
					cnt += (prog[ind] - '0');
					ind++;
				}
				string rep;
				if (prog[ind] == 'F' || prog[ind] == 'R' || prog[ind] == 'L') {
					rep = string{prog[ind]};
					ind ++;
				} else if (prog[ind] == '[') {
					ind ++;
					rep = expand(prog, ind);
				}
				for (int i = 0; i < cnt; i++) {
					res += rep;
				}
			} else if (c == 'F' || c == 'R' || c == 'L') {
				res += c;
			} else if (c == '[') {
				depth ++;
			} else {
				assert(false);
			}
		}
		return res;
	}
	string expand(const string &prog) {
		size_t ind = 0;
		return expand(prog, ind);
	}

	vector<seg> makePolyline(const string &line) {
		vector<seg> res;
		pt cur{0,0};
		dir d{0};
		for (char c : line) {
			if (c == 'F') {
				pt nxt = cur + pt(d);
				res.emplace_back(cur, d);
				res.emplace_back(nxt, -d);
				cur = nxt;
			} else if (c == 'L') {
				d++;
			} else if (c == 'R') {
				d--;
			} else assert(false);
		}
		sort(res.begin(), res.end());
		res.resize(size_t(unique(res.begin(), res.end()) - res.begin()));
		return res;
	}

public:

	vector<ll> findAreas(string polyline) {
		polyline = expand(polyline);
		vector<seg> segments = makePolyline(polyline);
		vector<int> next(segments.size(), -1);
		for (size_t i = 0; i < segments.size(); ) {
			seg st = segments[i];
			i++;
			while (i < segments.size() && segments[i].first == st.first) {
				next[i-1] = int(lower_bound(segments.begin(), segments.end(), rev(segments[i])) - segments.begin());
				i++;
			}
			next[i-1] = int(lower_bound(segments.begin(), segments.end(), rev(st)) - segments.begin());
		}
		for (int i : next) {
			assert(next[i] != -1);
		}
		vector<ll> res;
		for (int i = 0; i < int(segments.size()); i++) {
			if (next[i] == -1) continue;
			ll v = 0;
			for (int j = i; next[j] != -1; ) {
				v += value(segments[j]);
				int n = next[j];
				next[j] = -1;
				j = n;
			}
			v *= -1; // Fix sign
			if (v <= 0) continue;
			res.push_back(v);
		}
		sort(res.begin(), res.end());
		// TODO: first 100 and last 100
		vector<ll> compres;
		for (size_t i = 0; i < res.size(); i++) {
			if (i < 100 || i + 100 >= res.size()) {
				compres.push_back(res[i]);
			}
		}
		assert(compres.size() <= 200);
		assert(compres.size() == min(size_t(200), res.size()));
		return compres;
	}
};

}  // namespace ecnerwala

using ecnerwala::PolylineAreas;

// vim:ft=cpp
