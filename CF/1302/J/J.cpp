#include <bits/stdc++.h>
using namespace std;

int main() {

	string S; cin >> S;

	vector<int> v0(101, 0);
	vector<int> v1(101);
	for (int i = 1; i <= 100; i++) v1[i] = (S[i-1] - '0');

	bool x;
	auto check_fn = [&](auto fn) { return fn(v0) == fn(v1); };
#define CHECK(expr) {x = check_fn([](const vector<int>& d) -> bool { return (expr); });}
	auto rotate_zero = [&](int i, int v) { v0[i] += v, v0[i] %= 10; if (x) { v1[i] += v, v1[i] %= 10; } };
	auto rotate_nonzero = [&](int i, int v) { if (!x) { v1[i] += v, v1[i] %= 10; } };

CHECK(d[39] % 2 == 1)
	rotate_zero(39, 9);
	rotate_nonzero(37, 1);
CHECK(d[24] % 2 == 1)
	rotate_zero(24, 1);
	rotate_nonzero(76, 3);
CHECK(d[13] + d[91] > 10)
	rotate_zero(14, 6);
	rotate_nonzero(34, 8);
CHECK(d[87] % 2 == 1)
	rotate_zero(87, 7);
	rotate_nonzero(22, 9);
CHECK(d[79] > d[15])
	rotate_zero(74, 7);
	rotate_nonzero(84, 6);
CHECK(d[26] + d[66] > 9)
	rotate_zero(31, 7);
	rotate_nonzero(95, 4);
CHECK(d[53] + d[1] > 8)
	rotate_nonzero(66, 1);
	rotate_zero(94, 6);
CHECK(d[41] > d[29])
	rotate_nonzero(67, 5);
	rotate_zero(41, 9);
CHECK(d[79] + d[20] > 10)
	rotate_zero(18, 2);
	rotate_nonzero(72, 9);
CHECK(d[14] + d[24] > 10)
	rotate_zero(64, 2);
	rotate_nonzero(84, 2);
CHECK(d[16] > d[34])
	rotate_zero(81, 5);
	rotate_nonzero(15, 2);
CHECK(d[48] + d[65] > 9)
	rotate_nonzero(57, 2);
	rotate_zero(28, 5);
CHECK(d[81] % 2 == 1)
	rotate_nonzero(81, 5);
	rotate_zero(25, 4);
CHECK(d[70] % 2 == 1)
	rotate_zero(70, 9);
	rotate_nonzero(93, 3);
CHECK(d[92] + d[49] > 9)
	rotate_nonzero(81, 2);
	rotate_zero(42, 3);
CHECK(d[96] > d[20])
	rotate_nonzero(45, 4);
	rotate_zero(45, 1);
CHECK(d[91] > d[21])
	rotate_zero(60, 3);
	rotate_nonzero(72, 1);
CHECK(d[89] > d[7])
	rotate_nonzero(98, 9);
	rotate_zero(52, 7);
CHECK(d[38] > d[97])
	rotate_zero(92, 6);
	rotate_nonzero(35, 4);
CHECK(d[96] > d[99])
	rotate_nonzero(42, 4);
	rotate_zero(40, 9);
CHECK(d[86] % 2 == 1)
	rotate_nonzero(86, 1);
	rotate_zero(14, 3);
CHECK(d[23] % 2 == 1)
	rotate_nonzero(23, 5);
	rotate_zero(55, 9);
CHECK(d[79] % 2 == 1)
	rotate_nonzero(79, 1);
	rotate_zero(29, 8);
CHECK(d[4] > d[91])
	rotate_nonzero(98, 8);
	rotate_zero(69, 4);
CHECK(d[93] > d[24])
	rotate_nonzero(75, 9);
	rotate_zero(95, 3);
CHECK(d[32] + d[50] > 10)
	rotate_zero(91, 3);
	rotate_nonzero(1, 5);
CHECK(d[81] > d[31])
	rotate_nonzero(86, 7);
	rotate_zero(67, 5);
CHECK(d[83] > d[86])
	rotate_zero(48, 7);
	rotate_nonzero(2, 6);
CHECK(d[20] > d[88])
	rotate_nonzero(9, 2);
	rotate_zero(99, 4);
CHECK(d[14] % 2 == 1)
	rotate_nonzero(14, 5);
	rotate_zero(97, 7);
CHECK(d[38] > d[14])
	rotate_zero(48, 2);
	rotate_nonzero(81, 5);
CHECK(d[92] > d[74])
	rotate_nonzero(92, 1);
	rotate_zero(50, 9);
CHECK(d[76] > d[89])
	rotate_zero(68, 6);
	rotate_nonzero(69, 5);
CHECK(d[2] > d[28])
	rotate_nonzero(75, 1);
	rotate_zero(89, 1);
CHECK(d[67] % 2 == 1)
	rotate_nonzero(67, 9);
	rotate_zero(49, 1);
CHECK(d[23] % 2 == 1)
	rotate_zero(23, 1);
	rotate_nonzero(59, 3);
CHECK(d[81] % 2 == 1)
	rotate_zero(81, 9);
	rotate_nonzero(9, 4);
CHECK(d[92] + d[82] > 9)
	rotate_zero(81, 2);
	rotate_nonzero(91, 5);
CHECK(d[42] + d[48] > 9)
	rotate_zero(35, 8);
	rotate_nonzero(59, 6);
CHECK(d[55] % 2 == 1)
	rotate_zero(55, 9);
	rotate_nonzero(61, 6);
CHECK(d[83] % 2 == 1)
	rotate_zero(83, 5);
	rotate_nonzero(85, 4);
CHECK(d[96] % 2 == 1)
	rotate_nonzero(96, 1);
	rotate_zero(72, 4);
CHECK(d[17] % 2 == 1)
	rotate_nonzero(17, 1);
	rotate_zero(28, 3);
CHECK(d[85] > d[74])
	rotate_zero(37, 3);
	rotate_nonzero(10, 3);
CHECK(d[50] + d[67] > 9)
	rotate_nonzero(85, 9);
	rotate_zero(42, 4);
CHECK(d[11] + d[43] > 10)
	rotate_zero(56, 7);
	rotate_nonzero(50, 7);
CHECK(d[95] + d[64] > 9)
	rotate_zero(95, 4);
	rotate_nonzero(95, 9);
CHECK(d[21] + d[16] > 9)
	rotate_nonzero(87, 3);
	rotate_zero(30, 1);
CHECK(d[91] % 2 == 1)
	rotate_zero(91, 1);
	rotate_nonzero(77, 1);
CHECK(d[95] > d[82])
	rotate_nonzero(53, 2);
	rotate_zero(100, 5);
CHECK(d[88] + d[66] > 10)
	rotate_zero(34, 4);
	rotate_nonzero(57, 4);
CHECK(d[73] > d[84])
	rotate_zero(52, 3);
	rotate_nonzero(42, 9);
CHECK(d[66] > d[38])
	rotate_zero(94, 7);
	rotate_nonzero(78, 7);
CHECK(d[23] > d[12])
	rotate_zero(78, 2);
	rotate_nonzero(62, 8);
CHECK(d[13] > d[9])
	rotate_nonzero(42, 7);
	rotate_zero(1, 9);
CHECK(d[43] > d[29])
	rotate_nonzero(20, 2);
	rotate_zero(47, 2);
CHECK(d[100] + d[51] > 8)
	rotate_zero(10, 6);
	rotate_nonzero(89, 1);
CHECK(d[19] > d[37])
	rotate_zero(26, 7);
	rotate_nonzero(30, 8);
CHECK(d[73] > d[25])
	rotate_zero(77, 3);
	rotate_nonzero(41, 1);
CHECK(d[67] + d[96] > 10)
	rotate_zero(47, 6);
	rotate_nonzero(33, 5);
CHECK(d[11] > d[10])
	rotate_zero(33, 3);
	rotate_nonzero(4, 3);
CHECK(d[85] % 2 == 1)
	rotate_zero(85, 7);
	rotate_nonzero(37, 9);
CHECK(d[14] % 2 == 1)
	rotate_zero(14, 1);
	rotate_nonzero(28, 4);
CHECK(d[30] + d[18] > 8)
	rotate_zero(93, 5);
	rotate_nonzero(68, 1);
CHECK(d[54] + d[72] > 8)
	rotate_nonzero(88, 8);
	rotate_zero(25, 8);
CHECK(d[72] % 2 == 1)
	rotate_zero(72, 5);
	rotate_nonzero(10, 3);
CHECK(d[15] % 2 == 1)
	rotate_nonzero(15, 3);
	rotate_zero(68, 1);
CHECK(d[81] + d[31] > 9)
	rotate_nonzero(2, 5);
	rotate_zero(35, 1);
CHECK(d[57] % 2 == 1)
	rotate_nonzero(57, 1);
	rotate_zero(25, 9);
CHECK(d[75] + d[51] > 9)
	rotate_nonzero(73, 8);
	rotate_zero(49, 1);
CHECK(d[81] + d[61] > 10)
	rotate_zero(61, 3);
	rotate_nonzero(88, 1);
CHECK(d[60] % 2 == 1)
	rotate_zero(60, 1);
	rotate_nonzero(31, 2);
CHECK(d[93] % 2 == 1)
	rotate_nonzero(93, 5);
	rotate_zero(50, 1);
CHECK(d[19] + d[82] > 9)
	rotate_zero(48, 7);
	rotate_nonzero(88, 8);
CHECK(d[45] % 2 == 1)
	rotate_nonzero(45, 7);
	rotate_zero(100, 1);
CHECK(d[46] > d[71])
	rotate_zero(28, 8);
	rotate_nonzero(37, 6);
CHECK(d[79] % 2 == 1)
	rotate_zero(79, 5);
	rotate_nonzero(10, 1);
CHECK(d[19] > d[95])
	rotate_zero(76, 9);
	rotate_nonzero(95, 8);
CHECK(d[49] % 2 == 1)
	rotate_nonzero(49, 5);
	rotate_zero(66, 3);
CHECK(d[62] % 2 == 1)
	rotate_zero(62, 1);
	rotate_nonzero(26, 8);
CHECK(d[67] > d[33])
	rotate_nonzero(27, 8);
	rotate_zero(96, 2);
CHECK(d[73] + d[15] > 8)
	rotate_zero(98, 6);
	rotate_nonzero(11, 6);
CHECK(d[63] > d[42])
	rotate_zero(66, 1);
	rotate_nonzero(58, 2);
CHECK(d[41] % 2 == 1)
	rotate_nonzero(41, 9);
	rotate_zero(99, 5);
CHECK(d[93] % 2 == 1)
	rotate_zero(93, 5);
	rotate_nonzero(53, 1);
CHECK(d[46] % 2 == 1)
	rotate_zero(46, 3);
	rotate_nonzero(64, 4);
CHECK(d[99] + d[64] > 10)
	rotate_zero(72, 9);
	rotate_nonzero(51, 5);
CHECK(d[75] > d[23])
	rotate_nonzero(89, 2);
	rotate_zero(76, 7);
CHECK(d[6] % 2 == 1)
	rotate_zero(6, 1);
	rotate_nonzero(44, 6);
CHECK(d[58] % 2 == 1)
	rotate_nonzero(58, 3);
	rotate_zero(49, 9);
CHECK(d[5] > d[13])
	rotate_zero(46, 9);
	rotate_nonzero(21, 7);
CHECK(d[44] + d[94] > 9)
	rotate_zero(36, 4);
	rotate_nonzero(15, 3);
CHECK(d[52] + d[43] > 8)
	rotate_zero(29, 8);
	rotate_nonzero(72, 6);
CHECK(d[87] + d[48] > 9)
	rotate_nonzero(61, 8);
	rotate_zero(14, 3);
CHECK(d[81] % 2 == 1)
	rotate_nonzero(81, 7);
	rotate_zero(64, 2);
CHECK(d[88] % 2 == 1)
	rotate_nonzero(88, 7);
	rotate_zero(53, 9);
CHECK(d[86] + d[78] > 10)
	rotate_nonzero(96, 7);
	rotate_zero(79, 1);
CHECK(d[20] % 2 == 1)
	rotate_nonzero(20, 7);
	rotate_zero(2, 7);
CHECK(d[77] > d[80])
	rotate_zero(60, 5);
	rotate_nonzero(38, 8);
CHECK(d[65] % 2 == 1)
	rotate_zero(65, 1);
	rotate_nonzero(85, 3);

	for (int i = 1; i <= 100; i++) { cout << v1[i]; } cout << '\n';
}
