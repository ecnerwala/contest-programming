#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
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
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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

using num = modnum<int(1e9)+7>;

const std::array<int, 2> DIR[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

class ThoroughTesting {

public:

	int count(int R, int C, int N, vector <string> board) {
		using state = std::vector<std::array<int, 2>>;
		std::map<state, int> state_id;
		std::vector<state> states;

		// this is pretty sparse, but we'll make the full matrix later
		std::vector<std::vector<int>> adj;

		state start_state(N, {-1, -1});
		for (int x = 0; x < R; x++) {
			for (int y = 0; y < C; y++) {
				char c = board[x][y];
				if ('0' <= c && c <= '9') {
					int i = c - '0';
					start_state[i] = {x, y};
				}
			}
		}

		state_id[start_state] = int(states.size());
		states.push_back(start_state);
		for (int z = 0; z < int(states.size()); z++) {
			adj.emplace_back();
			const auto cur_state = states[z];
			for (int i = 0; i < N; i++) {
				for (auto [dx, dy] : DIR) {
					auto [cx, cy] = states[z][i];
					int nx = cx + dx;
					int ny = cy + dy;
					if (!(0 <= nx && nx < R && 0 <= ny && ny < C)) continue;
					if (board[nx][ny] == 'X') continue;
					if (std::find(cur_state.begin(), cur_state.end(), std::array<int, 2>{nx, ny}) != cur_state.end()) {
						continue;
					}
					auto nxt_state = cur_state;
					nxt_state[i] = {nx, ny};
					if (!state_id.count(nxt_state)) {
						state_id[nxt_state] = int(states.size());
						states.push_back(nxt_state);
					}
					adj[z].push_back(state_id[nxt_state]);
				}
			}
		}

		int V = int(states.size());
		std::vector<std::vector<num>> A(V, std::vector<num>(V));
		num ans = 1;
		for (int i = 0; i < V; i++) {
			int v = int(adj[i].size());
			// first edge is a choice
			for (int z = 1; z <= (i == 0 ? v : v-1); z++) ans *= z;

			for (int j : adj[i]) {
				A[i][i]++;
				A[i][j]--;
			}
		}

		/*
		cerr << V << '\n';
		cerr << "prod_deg " << ans << '\n';

		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				cerr << A[i][j] << ' ';
			}
			cerr << '\n';
		}
		*/

		for (int i = 1; i < V; i++) {
			{
				int j = i;
				while (j < V && A[j][i] == 0) j++;
				if (j == V) {
					ans = 0;
					break;
				}
				std::swap(A[i], A[j]);
			}

			ans *= A[i][i];
			{
				num f = inv(A[i][i]);
				for (int k = i; k < V; k++) {
					A[i][k] *= f;
				}
			}
			for (int j = i+1; j < V; j++) {
				if (A[j][i] == 0) continue;
				num f = A[j][i];
				for (int k = i; k < V; k++) {
					A[j][k] -= A[i][k] * f;
				}
			}
		}
		//cerr << ans << '\n';
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int r = 1; r <= 8; r++) {
		for (int c = 1; c <= 8; c++) {
			int cur = 1;
			for (int n = 1; n <= 8 && n <= r * c; n++) {
				cur *= (r * c);
				if (cur > 1300) break;
				if (cur >= 500) {
					cerr << r << ' ' << c << ' ' << n << ' ' << cur << '\n';
					std::vector<std::string> board(r, std::string(c, '.'));
					for (int i = 0; i < n; i++) {
						board[n/c][n%c] = char('0' + i);
					}
					auto st = clock();
					cerr << ThoroughTesting().count(r, c, n, board) << '\n';
					auto en = clock();
					cerr << "T = " << (double(en - st) / CLOCKS_PER_SEC) << '\n';
				}
			}
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::ThoroughTesting;

// vim:ft=cpp
