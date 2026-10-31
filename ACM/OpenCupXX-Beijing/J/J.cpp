#include<bits/stdc++.h>
using namespace std;

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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

using hsh = pairnum<modnum<int(1e9)+7>, modnum<int(1e9)+7>>;
const hsh BASE(59, 31897);

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int NLINES; cin >> NLINES;

	vector<string> tokens;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int i = 0; i < NLINES; i++) {
		string line; getline(cin, line);
		istringstream ss(line);
		for (string token; ss >> token;) {
			tokens.push_back(token);
		}
	}

	for (string token : tokens) {
		//cerr << "tok " << token << '\n';
	}

	int NMESS = 0;
	map<string, int> typeIds;
	for (int i = 0; i < int(tokens.size()); i++) {
		if (tokens[i] == "message") {
			typeIds.emplace(tokens[i+1], NMESS++);
		}
	}
	typeIds["double"] = NMESS;
	typeIds["string"] = NMESS+1;

	vector<vector<pair<int, int>>> messageFields;
	messageFields.reserve(NMESS);
	for (int i = 0; i < int(tokens.size()); i++) {
		if (tokens[i] == "message") {
			messageFields.emplace_back();
			assert(tokens[i+2] == "{");
			for (int j = i+3; tokens[j] != "}"; j += 6) {
				string label = tokens[j];
				string type = tokens[j+1];
				assert(tokens[j+3] == "=");
				int num = stoi(tokens[j+4]) << 3;
				assert(tokens[j+5] == ";");
				if (label == "required") {
					num |= 1;
				} else if (label == "optional") {
					num |= 2;
				} else if (label == "repeated") {
					num |= 3;
				} else assert(false);
				assert(typeIds.count(type));
				int typeId = typeIds[type];
				messageFields.back().emplace_back(num, typeId);
			}
			sort(messageFields.back().begin(), messageFields.back().end());
		}
	}

	vector<int> classId(NMESS);
	for (int i = 0; i < NMESS; i++) {
		classId[i] = 0;
	}

	for (int z = 0; z < NMESS; z++) {
		vector<pair<long long, int>> v;
		for (int i = 0; i < NMESS; i++) {
			hsh h = messageFields[i].size();
			for (auto it : messageFields[i]) {
				h *= BASE;
				h += it.first;
				h *= BASE;
				if (it.second >= NMESS) h += it.second;
				else h += classId[it.second];
			}
			v.emplace_back(int(h.u) * (1ll << 32) + int(h.v), i);
		}
		sort(v.begin(), v.end());
		for (int i = 0, ind = 0; i < NMESS; i++) {
			if (i && v[i].first != v[i-1].first) {
				ind++;
			}
			classId[v[i].second] = ind;
		}
	}

	int Q; cin >> Q;
	while (Q--) {
		string t1, t2; cin >> t1 >> t2;
		int v1 = typeIds[t1];
		int v2 = typeIds[t2];
		assert(v1 < NMESS);
		assert(v2 < NMESS);
		bool compat = classId[v1] == classId[v2];
		cout << (compat ? "Wire-format compatible." : "Wire-format incompatible.") << '\n';
	}

	return 0;
}
