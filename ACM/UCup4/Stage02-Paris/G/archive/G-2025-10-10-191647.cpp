#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<std::string> lines;
	{
		std::string line;
		while (std::getline(std::cin, line)) {
			lines.push_back(line);
		}
	}

	int ans = 0;
	for (int i = 0; i < int(lines.size()); i++) {
		// check if i is a valid starting line
		if (int(lines[i].size()) < 7) continue;
		if ((int(lines[i].size()) - 7) % 4) continue;
		std::string S = lines[i].substr(int(lines[i].size()) - (int(lines[i].size()) - 7) / 4);
		auto cnd = S + ", " + S + " " + S + "ity " + S;
		auto cnd2 = "i said " + cnd;
		int len = 0;
		for (int j = i; j < int(lines.size()) && lines[j] == ((j-i) % 2 == 0 ? cnd : cnd2); j++) {
			len += int(lines[j].size()) + 1;
		}
		ans = std::max(ans, len);
	}
	if (ans == 0) ans = -1;
	cout << ans << '\n';

	return 0;
}
