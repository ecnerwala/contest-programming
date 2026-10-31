#include <bits/stdc++.h>

const std::array<int, 91> LEVEL_EXP_PREF = {
	0, 0, 1000, 2325, 4025, 6175, 8800, 11950, 15675, 20025, 25025, 30725, 37175, 44400, 52450, 61375, 71200, 81950, 93675, 106400, 120175, 135050, 151850, 169850, 189100, 209650, 231525, 254775, 279425, 305525, 333100, 362200, 392850, 425100, 458975, 494525, 531775, 570750, 611500, 654075, 698500, 744800, 795425, 848125, 902900, 959800, 1018875, 1080150, 1143675, 1209475, 1277600, 1348075, 1424575, 1503625, 1585275, 1669550, 1756500, 1846150, 1938550, 2033725, 2131725, 2232600, 2341550, 2453600, 2568775, 2687100, 2808625, 2933400, 3061475, 3192875, 3327650, 3465825, 3614525, 3766900, 3922975, 4082800, 4246400, 4413825, 4585125, 4760350, 4939525, 5122700, 5338925, 5581950, 5855050, 6161850, 6506450, 6893400, 7327825, 7815450, 8362650
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_CASES; cin >> N_CASES;
	while (N_CASES--) {

		int cur_a, cur_l; std::array<int, 3> cur_t;
		cin >> cur_a >> cur_l; for (auto& i : cur_t) cin >> i;

		int tgt_a, tgt_l; std::array<int, 3> tgt_t;
		cin >> tgt_a >> tgt_l; for (auto& i : tgt_t) cin >> i;

		int num_coins = 0, num_special = 0, num_boss = 0, num_weekly_boss = 0, num_crown = 0;

		std::array<int, 3> num_exp_mat{0, 0, 0};
		int num_gem_silver = 0, num_gem_fragment = 0, num_gem_chunk = 0, num_gem_gemstone = 0;
		int num_mob_common = 0, num_mob_rare = 0, num_mob_epic = 0;
		int num_book_teach = 0, num_book_guide = 0, num_book_phylo = 0;

		const std::array<int, 7> stops{20, 40, 50, 60, 70, 80, 90};
		while (cur_l < tgt_l) {
			int nxt_l = std::min(tgt_l, *std::upper_bound(stops.begin(), stops.end(), cur_l));
			int exp_amt = LEVEL_EXP_PREF[nxt_l] - LEVEL_EXP_PREF[cur_l];
			num_coins += exp_amt / 5;

			exp_amt = (exp_amt + 999) / 1000;
			num_exp_mat[2] += exp_amt / 20;
			exp_amt %= 20;
			num_exp_mat[1] += exp_amt / 5;
			num_exp_mat[0] += exp_amt % 5;

			cur_l = nxt_l;
		}

		for (; cur_a < tgt_a; cur_a ++) {
			if (cur_a == 0) {
				num_gem_silver += 1;
				num_boss += 0;
				num_mob_common += 3;
				num_special += 3;
				num_coins += 20000;
			} else if (cur_a == 1) {
				num_gem_fragment += 3;
				num_boss += 2;
				num_mob_common += 15;
				num_special += 10;
				num_coins += 40000;
			} else if (cur_a == 2) {
				num_gem_fragment += 6;
				num_boss += 4;
				num_mob_rare += 12;
				num_special += 20;
				num_coins += 60000;
			} else if (cur_a == 3) {
				num_gem_chunk += 3;
				num_boss += 8;
				num_mob_rare += 18;
				num_special += 30;
				num_coins += 80000;
			} else if (cur_a == 4) {
				num_gem_chunk += 6;
				num_boss += 12;
				num_mob_epic += 12;
				num_special += 45;
				num_coins += 100000;
			} else if (cur_a == 5) {
				num_gem_gemstone += 6;
				num_boss += 20;
				num_mob_epic += 24;
				num_special += 60;
				num_coins += 120000;
			} else assert(false);
		}

		for (int z = 0; z < 3; z++) {
			for (; cur_t[z] < tgt_t[z]; cur_t[z]++) {
				int lvl = cur_t[z];
				if (lvl == 1) {
					num_coins += 12500;
					num_mob_common += 6;
					num_book_teach += 3;
				} else if (lvl == 2) {
					num_coins += 17500;
					num_mob_rare += 3;
					num_book_guide += 2;
				} else if (lvl == 3) {
					num_coins += 25000;
					num_mob_rare += 4;
					num_book_guide += 4;
				} else if (lvl == 4) {
					num_coins += 30000;
					num_mob_rare += 6;
					num_book_guide += 6;
				} else if (lvl == 5) {
					num_coins += 37500;
					num_mob_rare += 9;
					num_book_guide += 9;
				} else if (lvl == 6) {
					num_coins += 120000;
					num_mob_epic += 4;
					num_book_phylo += 4;
					num_weekly_boss += 1;
				} else if (lvl == 7) {
					num_coins += 260000;
					num_mob_epic += 6;
					num_book_phylo += 6;
					num_weekly_boss += 1;
				} else if (lvl == 8) {
					num_coins += 450000;
					num_mob_epic += 9;
					num_book_phylo += 12;
					num_weekly_boss += 2;
				} else if (lvl == 9) {
					num_coins += 700000;
					num_mob_epic += 12;
					num_book_phylo += 16;
					num_weekly_boss += 2;
					num_crown += 1;
				} else assert(false);
			}
		}

		cout << num_coins << ' ' << num_special << ' ' << num_boss << ' ' << num_weekly_boss << ' ' << num_crown << '\n';
		cout << num_exp_mat[0] << ' ' << num_exp_mat[1] << ' ' << num_exp_mat[2] << '\n';
		cout << num_gem_silver << ' ' << num_gem_fragment << ' ' << num_gem_chunk << ' ' << num_gem_gemstone << '\n';
		cout << num_mob_common << ' ' << num_mob_rare << ' ' << num_mob_epic << '\n';
		cout << num_book_teach << ' ' << num_book_guide << ' ' << num_book_phylo << '\n';
	}

	return 0;
}
