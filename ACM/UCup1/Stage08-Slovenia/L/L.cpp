#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> deck(98);
	for (auto& v : deck) cin >> v;
	auto deck_it = deck.begin();

	std::array<std::vector<int>, 4> piles{
		std::vector<int>({1}),
		std::vector<int>({1}),
		std::vector<int>({100}),
		std::vector<int>({100}),
	};

	std::vector<int> hand;
	hand.reserve(8);
	auto draw_card = [&]() -> void {
		if (deck_it == deck.end()) return;
		hand.push_back(*(deck_it++));
	};
	for (int z = 0; z < 8; z++) draw_card();

	for (int iter = 0; !hand.empty(); iter++) {
		for (int z = 0; z < 2; z++) {
			int best_abs = 200;
			int best_i = -1;
			int best_p = -1;

			for (int i = 0; i < int(hand.size()); i++) {
				for (int p = 0; p < int(piles.size()); p++) {
					int d = hand[i] - piles[p].back();
					if (p >= 2) d = -d;
					if (d == -10) {
						best_abs = d;
						best_i = i;
						best_p = p;
						goto found;
					}
				}
			}

			for (int i = 0; i < int(hand.size()); i++) {
				for (int p = 0; p < int(piles.size()); p++) {
					int d = hand[i] - piles[p].back();
					if (p >= 2) d = -d;
					if (d > 0 && d < best_abs) {
						best_abs = d;
						best_i = i;
						best_p = p;
					}
				}
			}
			if (best_abs == 200) goto done;
found:
			piles[best_p].push_back(hand[best_i]);
			hand.erase(hand.begin() + best_i);
		}
		draw_card();
		draw_card();
	}


done:;

	for (int z = 0; z < 4; z++) {
		for (int i = 0; i < int(piles[z].size()); i++) {
			cout << piles[z][i] << " \n"[i+1==int(piles[z].size())];
		}
	}

	if (hand.empty()) cout << '\n';
	for (int i = 0; i < int(hand.size()); i++) {
		cout << hand[i] << " \n"[i+1==int(hand.size())];
	}

	deck.erase(deck.begin(), deck_it);
	if (deck.empty()) cout << '\n';
	for (int i = 0; i < int(deck.size()); i++) {
		cout << deck[i] << " \n"[i+1==int(deck.size())];
	}

	return 0;
}
