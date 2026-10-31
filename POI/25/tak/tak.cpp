#include <bits/stdc++.h>

struct fraction {
	int num, den;
	fraction() : num(0), den(1) {}
	explicit fraction(int a) : num(a), den(1) {}
	fraction(int a, int b) : num(a), den(b) { assert(den > 0); }

	friend bool operator < (fraction a, fraction b) { return int64_t(a.num) * int64_t(b.den) < int64_t(a.den) * int64_t(b.num); }
	friend bool operator <= (fraction a, fraction b) { return int64_t(a.num) * int64_t(b.den) <= int64_t(a.den) * int64_t(b.num); }

	void reduce() {
		int g = std::gcd(num, den);
		num /= g;
		den /= g;
	}
	
	friend std::ostream& operator << (std::ostream& o, fraction f) { return o << f.num << '/' << f.den; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<pair<int, int>> V_orig(N); for (auto& a : V_orig) cin >> a.first >> a.second;

	vector<pair<int, int>> V(N);
	for (auto& v : V) { int a; cin >> a; a--; v = V_orig[a]; }

	vector<bool> used(N);

	multiset<fraction> lo;
	multiset<fraction> hi;

	lo.insert(fraction(0));

	auto add_pair = [&](int i) -> void {
		if (i == 0 || i == N) return;
		if (used[i]) return;
		used[i] = true;
		int v0 = V[i].first - V[i-1].first;
		int v1 = V[i].second - V[i-1].second;
		// v1 * d + v0 >= 0
		if (v1 > 0) {
			// d >= -v0 / v1
			lo.insert(fraction(-v0, v1));
		} else if (v1 == 0) {
			if (v0 < 0) hi.insert(fraction(-1));
			return;
		} else if (v1 < 0) {
			// v0 / -v1 >= d
			hi.insert(fraction(v0, -v1));
		} else assert(false);
	};

	auto remove_pair = [&](int i) -> void {
		if (i == 0 || i == N) return;
		if (!used[i]) return;
		used[i] = false;
		int v0 = V[i].first - V[i-1].first;
		int v1 = V[i].second - V[i-1].second;
		// v1 * d + v0 >= 0
		if (v1 > 0) {
			// d >= -v0 / v1
			lo.erase(lo.find(fraction(-v0, v1)));
		} else if (v1 == 0) {
			if (v0 < 0) hi.erase(hi.find(fraction(-1)));
			return;
		} else if (v1 < 0) {
			// v0 / -v1 >= d
			hi.erase(hi.find(fraction(v0, -v1)));
		} else assert(false);
	};

	auto ans = [&]() -> void {
		std::optional<fraction> res;

		fraction vlo = *lo.rbegin();
		if (hi.empty()) {
			if (vlo.num == 0) {
				res = fraction(1);
			} else {
				res = vlo;
			}
		} else {
			fraction vhi = *hi.begin();
			if (vhi.num > 0 && vlo <= vhi) {
				res = vhi;
			}
		}

		if (res) {
			res->reduce();
			cout << *res << '\n';
		} else {
			cout << "NIE" << '\n';
		}
	};

	for (int i = 1; i < N; i++) {
		add_pair(i);
	}

	ans();

	int Q; cin >> Q;
	while (Q--) {
		int a, b; cin >> a >> b; a--, b--;
		remove_pair(a);
		remove_pair(a+1);
		remove_pair(b);
		remove_pair(b+1);
		std::swap(V[a], V[b]);
		add_pair(a);
		add_pair(a+1);
		add_pair(b);
		add_pair(b+1);

		ans();
	}

	return 0;
}
