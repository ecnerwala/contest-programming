#include <bits/stdc++.h>

std::vector<std::array<char, 14>> c14;
std::map<std::array<char, 14>, int> c14_id;
void precomp() {
	for (int msk = 0; msk < (1 << 14); msk++) {
		if (__builtin_popcount(msk) * 2 != 14) continue;
		std::array<char, 14> cur;
		bool good = true;
		int cnt = 0;
		for (int i = 0; i < 14; i++) {
			char c = ((msk >> i) & 1) ? '(' : ')';
			cur[i] = c;
			if (c == '(') cnt++;
			else if (c == ')') cnt--;
			else assert(false);
			if (cnt < 0) { good = false; break; }
		}
		if (!good) continue;
		int id = int(c14.size());
		c14.push_back(cur);
		c14_id.insert({cur, id});
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
	for (int z = 0; z < 5; z++) {
		int id = int(cur % int(c14.size()));
		for (char c : c14[id]) res += c;
		cur /= int(c14.size());
	}
	assert(cur == 0);
	return res;
}

std::string paren_to_bin(std::string S) {
	assert(int(S.size()) >= 70);
	int64_t val = 0;
	for (int z = 0; z < 5; z++) {
		val *= int(c14.size());
		std::array<char, 14> res;
		for (int i = 13; i >= 0; i--) { res[i] = S.back(); S.pop_back(); }
		val += c14_id[res];
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
	cerr << c14.size() << '\n';
	int R; cin >> R;
	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		if (R == 1) {
			cout << bin_to_paren(S) << '\n';
		} else if (R == 2) {
			cout << paren_to_bin(S) << '\n';
		} else assert(false);
	}

	return 0;
}
