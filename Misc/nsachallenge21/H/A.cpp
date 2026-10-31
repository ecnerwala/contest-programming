#include <bits/stdc++.h>

const std::string ALPHABET = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz9876543210*@";

std::string decode_crazy64(const std::string& s) {
	std::string cur_alphabet = ALPHABET;
	assert(s.size() % 4 == 0);
	std::string r; r.reserve(s.size() / 4 * 3);
	for (int i = 0; i < int(s.size()); i += 4) {
		auto from_char = [&](char c) -> uint32_t {
			if (c == '!') return 0;
			auto res = uint32_t(std::find(cur_alphabet.begin(), cur_alphabet.end(), c) - cur_alphabet.begin());
			assert(res < cur_alphabet.size());
			return res;
		};
		uint32_t v = (from_char(s[i]) << 18) | (from_char(s[i+1]) << 12) | (from_char(s[i+2]) << 6) | (from_char(s[i+3]) << 0);
		r += char((v >> 16) & 255);
		if (s[i+2] != '!') r += char((v >> 8) & 255);
		if (s[i+3] != '!') r += char((v >> 0) & 255);
		int shift = v & 63;
		if (v & 1) {
			std::rotate(cur_alphabet.begin(), cur_alphabet.begin() + shift, cur_alphabet.end());
		} else {
			std::rotate(cur_alphabet.begin(), cur_alphabet.end() - shift, cur_alphabet.end());
		}
	}
	return r;
}

std::string encode_crazy64(std::string s) {
	int num_pad = 0;
	while (s.size() % 3) s += '\0', num_pad++;
	std::string cur_alphabet = ALPHABET;
	std::string r; r.reserve(s.size() / 3 * 4);
	for (int i = 0; i < int(s.size()); i += 3) {
		auto from_char = [&](char c) -> uint32_t {
			return uint32_t(uint8_t(c));
		};
		uint32_t v = (from_char(s[i]) << 16) | (from_char(s[i+1]) << 8) | (from_char(s[i+2]) << 0);
		r += cur_alphabet[(v >> 18) & 63];
		r += cur_alphabet[(v >> 12) & 63];
		r += cur_alphabet[(v >> 6) & 63];
		r += cur_alphabet[(v >> 0) & 63];
		int shift = v & 63;
		if (v & 1) {
			std::rotate(cur_alphabet.begin(), cur_alphabet.begin() + shift, cur_alphabet.end());
		} else {
			std::rotate(cur_alphabet.begin(), cur_alphabet.end() - shift, cur_alphabet.end());
		}
	}
	for (int z = 0; z < num_pad; z++) r[int(r.size())-1-z] = '!';
	return r;
}

std::string encode_hex(const std::string& s) {
	std::ostringstream o;
	for (char c : s) {
		o << std::hex << std::setw(2) << std::setfill('0') << uint32_t(uint8_t(c));
	}
	return o.str();
}

std::string decode_hex(const std::string& s) {
	assert(s.size() % 2 == 0);
	std::string r; r.reserve(s.size()/2);
	for (int i = 0; i < int(s.size()); i += 2) {
		r += char(std::stoi(s.substr(i, 2), nullptr, 16));
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string op; cin >> op;
	string s; cin >> s;
	if (op == "DECODE") {
		cout << encode_hex(decode_crazy64(s)) << '\n';
	} else if (op == "ENCODE") {
		cout << encode_crazy64(decode_hex(s)) << '\n';
	} else assert(false);

	return 0;
}
