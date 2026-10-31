#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD > 0, "Positive MOD only");

private:
	int v;
public:
	modnum() : v(0) {}
	/* IMPLICIT */ modnum(int64_t a) : v(int(a % MOD)) {
		if (v < 0) v += MOD;
	}
	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& o, modnum a) { return o << a.v; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	friend modnum operator + (modnum a, modnum b) { return a += b; }

	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	friend modnum operator - (modnum a, modnum b) { return a -= b; }

	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		/*
		cerr << "Starting case" << '\n';
		for (int a : A) { cerr << a << ' '; } cerr << '\n';
		*/

		using num = modnum<int(1e9)+7>;
		// last char, num reps, second to last was bigger, num reps
		struct dp_t {
			int max_reps = 0;
			bool was_bigger = true;
			std::vector<num> v;
		};
		std::map<int, dp_t> dp;
		dp[-3] = dp_t{1, false, std::vector<num>({num(1)})};
		for (int cur : A) {
			std::map<int, dp_t> ndp;
			for (auto& [last_char, c] : dp) {
				if (cur == last_char) {
					auto& nc = ndp[cur];
					nc.max_reps = std::max(nc.max_reps, c.max_reps + 1);
					if (int(c.v.size()) > c.max_reps) {
						nc.was_bigger = c.was_bigger;
					}

					if (nc.v.size() < c.v.size()+1) nc.v.resize(c.v.size()+1);
					for (int i = 0; i < int(c.v.size()); i++) nc.v[i+1] += c.v[i];

				} else if (abs(cur - last_char) == 2) {
					auto& nc = ndp[cur];
					if (c.was_bigger != (cur > last_char)) {
						num w = std::accumulate(c.v.begin() + c.max_reps, c.v.end(), num(0));
						c.v.resize(c.max_reps);
						c.v.back() += w;
					}

					nc.max_reps = 1;
					nc.was_bigger = last_char > cur;

					if (nc.v.size() < c.v.size()+1) nc.v.resize(c.v.size()+1);
					for (int i = 0; i < int(c.v.size()); i++) nc.v[i+1] += c.v[i];
				} else if (abs(cur - last_char) == 1) {
					if (c.was_bigger != (cur > last_char)) {
						num w = std::accumulate(c.v.begin() + c.max_reps, c.v.end(), num(0));
						c.v.resize(c.max_reps);
						c.v.back() += w;
					}
					{
						auto& nc = ndp[cur];
						nc.max_reps = std::max(nc.max_reps, 1);
						if (nc.v.size() < 1) nc.v.resize(1);
						for (auto w : c.v) nc.v[0] += w;
					}
					{
						assert(ndp.find(last_char) == ndp.end());
						auto& nc = ndp[last_char];
						if (int(c.v.size()) > c.max_reps) {
							nc.was_bigger = c.was_bigger;
						}
						nc.max_reps = c.max_reps;
						nc.v = c.v;
						for (int i = int(nc.v.size()) - 1; i > 0; i--) {
							nc.v[i-1] += nc.v[i];
						}
					}
				} else {
					auto& nc = ndp[cur];
					nc.max_reps = std::max(nc.max_reps, 1);
					if (nc.v.size() < 1) nc.v.resize(1);
					for (auto w : c.v) nc.v[0] += w;
				}
			}
			dp = std::move(ndp);
			/*
			cerr << "inserted " << cur << '\n';
			for (auto [last_char, c] : dp) {
				cerr << last_char << 'x' << c.max_reps << ' ' << (c.was_bigger ? last_char + 2 : last_char - 2) << 'x' << c.v.size() << " => ";
				for (auto w : c.v) { cerr << w << ' '; } cerr << '\n';
			}
			*/
		}

		num ans = 0;
		for (auto [_, c] : dp) {
			for (auto w : c.v) {
				ans += w;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
