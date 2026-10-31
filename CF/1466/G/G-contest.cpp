#include <bits/stdc++.h>

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

using num = modnum<int(1e9)+7>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	string S0, T; cin >> S0 >> T;
	assert(int(T.size()) == N);

	vector<array<int, 26>> trie; trie.reserve(int(1e6) + 20);
	trie.push_back(array<int, 26>()); trie.back().fill(-1);
	vector<bool> is_needed; is_needed.reserve(int(1e6) + 20);
	is_needed.push_back(false);

	int max_len = 0;

	vector<vector<pair<int, int>>> queries(N+1);
	for (int q = 0; q < Q; q++) {
		int k; string w; cin >> k >> w;
		max_len = max(int(w.size()), max_len);
		int cur = 0;
		for (char c : w) {
			c -= 'a';
			if (trie[cur][c] != -1) {
				cur = trie[cur][c];
			} else {
				int next_idx = int(trie.size());
				trie[cur][c] = next_idx;
				cur = next_idx;
				trie.push_back(array<int, 26>()); trie.back().fill(-1);
				is_needed.push_back(false);
			}
		}
		is_needed[cur] = true;
		queries[k].push_back({cur, q});
	}

	vector<int> query_failure; query_failure.reserve(trie.size());
	query_failure.push_back(-1);
	vector<int> max_query(trie.size());
	vector<int> idx(trie.size(), -1);
	{
		vector<int> failure(trie.size());
		vector<array<int, 26>> bfs_trie(trie.size());
		vector<int> bfs; bfs.reserve(trie.size());
		failure[0] = -1;
		bfs.push_back(0);
		for (int z = 0; z < int(bfs.size()); z++) {
			int cur = bfs[z];
			if (is_needed[cur]) {
				idx[cur] = max_query[z] = int(query_failure.size());
				query_failure.push_back(max_query[failure[z]]);
			} else {
				max_query[z] = z ? max_query[failure[z]] : 0;
			}
			for (int c = 0; c < 26; c++) {
				if (trie[cur][c] == -1) {
					bfs_trie[z][c] = z ? bfs_trie[failure[z]][c] : 0;
				} else {
					int nxt = trie[cur][c];
					int nz = int(bfs.size());
					failure[nz] = z ? bfs_trie[failure[z]][c] : 0;
					bfs_trie[z][c] = nz;
					bfs.push_back(nxt);
				}
			}
		}
		assert(bfs.size() == trie.size());
		trie = std::move(bfs_trie);
	}

	auto get_cnts = [&](const string& sa) -> vector<num> {
		vector<num> res(query_failure.size(), 0);
		{
			int cur = 0;
			++res[max_query[cur]];
			for (char c : sa) {
				c -= 'a';
				cur = trie[cur][c];
				++res[max_query[cur]];
			}
		}
		for (int z = int(query_failure.size())-1; z > 0; z--) {
			res[query_failure[z]] += res[z];
		}
		return res;
	};

	vector<num> ans(Q);

	int K = 0;
	string SK = S0;
	while (K < N && int(SK.size()) < max_len) {
		auto cnts = get_cnts(SK);
		for (auto [n, q] : queries[K]) {
			assert(is_needed[n]);
			assert(idx[n] != -1);
			n = idx[n];
			ans[q] = cnts[n];
		}
		SK = SK + T[K] + SK;
		K++;
	}

	vector<num> base_cnts = get_cnts(SK);
	array<vector<num>, 26> letter_cnts;
	int st = 0;
	for (char c : SK) {
		st = trie[st][c-'a'];
	}

	for (int c = 0; c < 26; c++) {
		vector<num> res(trie.size(), 0);
		{
			int a = trie[st][c];
			int b = 0;
			++res[max_query[a]];
			--res[max_query[b]];
			for (char v : SK) {
				v -= 'a';
				a = trie[a][v];
				b = trie[b][v];
				if (a == b) break;
				++res[max_query[a]];
				--res[max_query[b]];
			}
		}
		for (int z = int(query_failure.size())-1; z > 0; z--) {
			res[query_failure[z]] += res[z];
		}
		letter_cnts[c] = std::move(res);
	}
	num base_weight = 1;
	array<num, 26> letter_weights; letter_weights.fill(num(0));
	while (true) {
		for (auto [n, q] : queries[K]) {
			assert(is_needed[n]);
			assert(idx[n] != -1);
			n = idx[n];
			ans[q] = 0;
			ans[q] += base_weight * base_cnts[n];
			for (int c = 0; c < 26; c++) {
				ans[q] += letter_weights[c] * letter_cnts[c][n];
			}
		}
		if (K == N) break;
		base_weight *= 2;
		for (int c = 0; c < 26; c++) letter_weights[c] *= 2;
		letter_weights[T[K] - 'a']++;
		K++;
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
