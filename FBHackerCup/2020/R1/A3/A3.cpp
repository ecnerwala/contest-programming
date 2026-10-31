#include<bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		struct rect {
			int64_t l, w, h;
		};
		vector<rect> R(N);
		{
			for (int i = 0; i < K; i++) cin >> R[i].l;
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				R[i].l = int((A * R[i-2].l + B * R[i-1].l + C) % D) + 1;
			}
		}
		{
			for (int i = 0; i < K; i++) cin >> R[i].w;
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				R[i].w = int((A * R[i-2].w + B * R[i-1].w + C) % D) + 1;
			}
		}
		{
			for (int i = 0; i < K; i++) cin >> R[i].h;
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				R[i].h = int((A * R[i-2].h + B * R[i-1].h + C) % D) + 1;
			}
		}

		for (int i = 0; i+1 < N; i++) {
			assert(R[i].h <= R[i+1].h);
		}

		int64_t maxX = 1;
		for (const rect& r : R) {
			maxX = max(maxX, r.l + r.w + 1);
		}

		using num = modnum<int(1e9)+7>;
		num ans = 1;

		num curP = 0;
		set<array<int64_t, 2>> segs;
		// dummy boundaries
		segs.insert({-2, -1});
		segs.insert({maxX + 1, maxX + 2});

		map<int64_t, array<int64_t, 2>> verts;
		verts[-1] = {0, 0};
		verts[maxX+1] = {0, 0};

		for (const rect& r : R) {
			int64_t lo = r.l;
			int64_t hi = r.l + r.w;

			// horizontal component of the boundary
			{
				auto it = segs.lower_bound({lo+1, -1});
				--it;
				if ((*it)[1] < lo) {
					it = segs.insert({lo, lo}).first;
				}

				auto jt = segs.lower_bound({hi+1, -1});
				--jt;
				if ((*jt)[1] < hi) {
					jt = segs.insert({hi, hi}).first;
				}
				// it -> jt inclusive
				int64_t nlo = (*it)[0];
				int64_t nhi = (*jt)[1];
				jt++;

				while (it != jt) {
					curP -= 2 * ((*it)[1] - (*it)[0]);
					it = segs.erase(it);
				}

				curP += 2 * (nhi - nlo);
				segs.insert({nlo, nhi});
			}

			// vertical component of the boundary
			{
				auto it = verts.lower_bound(lo);
				--it;
				auto jt = verts.upper_bound(hi);
				int64_t p = it->second[1];
				int64_t n = jt->second[0];
				it++;
				while (it != jt) {
					curP -= abs(it->second[0] - it->second[1]);
					it = verts.erase(it);
				}

				verts[lo] = {p, r.h};
				curP += abs(r.h - p);

				verts[hi] = {r.h, n};
				curP += abs(r.h - n);
			}

			ans *= curP;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
