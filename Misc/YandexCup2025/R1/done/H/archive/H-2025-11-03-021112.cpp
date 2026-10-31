#include <bits/stdc++.h>

const int L = 20;
const int B = 5;
std::vector<std::array<char, L>> cL;
std::vector<int> cL_id;
void precomp() {
	cL_id.assign(1 << L, -1);
	for (int msk = 0; msk < (1 << L); msk++) {
		if (__builtin_popcount(msk) * 2 != L) continue;
		std::array<char, L> cur;
		bool good = true;
		int cnt = 0;
		for (int i = 0; i < L; i++) {
			char c = ((msk >> i) & 1) ? ')' : '(';
			cur[i] = c;
			if (c == '(') cnt++;
			else if (c == ')') cnt--;
			else assert(false);
			if (cnt < 0) { good = false; break; }
		}
		if (!good) continue;
		int id = int(cL.size());
		cL.push_back(cur);
		cL_id[msk] = id;
	}
}

std::string bin_to_paren(std::string S) {
	bool did_extend = int(S.size()) & 1;
	if (did_extend) S += '0';
	int N = int(S.size());
	// flip a prefix of size flip_size
	int flip_size = 0;
	{
		int cnt = 0;
		for (char c : S) cnt += c == '1';
		while (cnt != N / 2) {
			cnt -= S[flip_size] == '1';
			S[flip_size] ^= 1;
			cnt += S[flip_size] == '1';
			flip_size++;
		}
	}
	int rot_size = 0;
	{
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') {
				if (cnt == 0) rot_size = i+1;
				else cnt--;
			}
			else if (S[i] == '0') cnt++;
			else assert(false);
		}
		std::rotate(S.begin(), S.begin() + rot_size, S.end());
		cnt = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') cnt--;
			else if (S[i] == '0') cnt++;
			else assert(false);
			assert(cnt >= 0);
		}
		assert(cnt == 0);
	}
	int64_t val = int64_t(did_extend * N + flip_size) * int64_t(N) + rot_size;
	// in 4, you have 2 choices
	// in 6, you have 5 choices
	// in 8, you have 
	std::string res; res.reserve(N + 70);
	for (char c : S) {
		if (c == '0') res += '(';
		else if (c == '1') res += ')';
	}
	int64_t cur = val;
	for (int z = 0; z < B; z++) {
		int id = int(cur % int(cL.size()));
		for (char c : cL[id]) res += c;
		cur /= int(cL.size());
	}
	assert(cur == 0);
	return res;
}

std::string paren_to_bin(std::string S) {
	assert(int(S.size()) >= 70);
	int64_t val = 0;
	for (int z = 0; z < B; z++) {
		val *= int(cL.size());
		int res = 0;
		for (int i = 0; i < L; i++) { res *= 2; res += S.back() == ')'; S.pop_back(); }
		assert(cL_id[res] != -1);
		val += cL_id[res];
	}
	int N = int(S.size());
	assert(!(N & 1));
	int rot_size = int(val % N);
	int flip_size = int(val / N % N);
	int did_extend = int(val / N / N);
	assert(did_extend <= 1);
	std::rotate(S.begin(), S.end() - rot_size, S.end());
	for (char& c : S) {
		if (c == '(') c = '0';
		else if (c == ')') c = '1';
	}
	for (int i = 0; i < flip_size; i++) S[i] ^= 1;
	if (did_extend) S.pop_back();
	return S;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	precomp();
	cerr << cL.size() << '\n';
	int R; cin >> R;
	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		if (R == 1) {
			cout << bin_to_paren(S) << '\n';
			cerr << S << '\n';
			cerr << bin_to_paren(S) << '\n';
			cerr << paren_to_bin(bin_to_paren(S)) << '\n';
		} else if (R == 2) {
			cout << paren_to_bin(S) << '\n';
		} else assert(false);
	}

	return 0;
}
