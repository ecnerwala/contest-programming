#include <bits/stdc++.h>

int N_people;
int N_toilet;
int64_t L;
int64_t cur_time;

struct location {
	int id; // person >= 0, negative is toilet
	int dir; // +-1
	int64_t loc_0; // location at time 0
};

int64_t modL(int64_t a) {
	a %= L;
	a += (a < 0) ? L : 0;
	return a;
}

int64_t eval_at(const location &l) {
	return modL(l.loc_0 + l.dir * cur_time) * (N_people+1) + l.dir * ~l.id;
}

struct my_comparator {
	int64_t& base;
	bool operator () (const location& a, const location& b) const {
		int64_t la = eval_at(a);
		int64_t lb = eval_at(b);
		return std::make_pair(la < base, la) < std::make_pair(lb < base, lb);
	}
};

int64_t base_pos;
int64_t base_neg;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> N_people >> N_toilet >> L;
	cur_time = 0;

	std::vector<location> toilets(N_toilet);
	for (int i = 0; i < N_toilet; i++) {
		int64_t x; cin >> x;
		toilets[i].id = ~i;
		toilets[i].loc_0 = x;
		toilets[i].dir = 0;
	}
	std::vector<location> people(N_people);
	std::vector<int64_t> person_ins_time(N_people);
	std::vector<int64_t> person_duration(N_people);
	for (int i = 0; i < N_people; i++) {
		int64_t t_ins, loc_t, duration; char s; cin >> t_ins >> loc_t >> s >> duration;
		person_ins_time[i] = t_ins;
		person_duration[i] = duration;

		assert(s == '+' || s == '-');
		int dir = (s == '+' ? 1 : -1);
		int64_t loc_0 = (loc_t - dir * t_ins) % L;
		loc_0 += (loc_0 < 0) ? L : 0;
		people[i].id = i;
		people[i].loc_0 = loc_0;
		people[i].dir = dir;
	}

	std::set<location, my_comparator> set_pos(my_comparator{base_pos});
	std::set<location, my_comparator> set_neg(my_comparator{base_neg});

	// always tiebreak by earlier person
	std::set<std::pair<int64_t, int>> people_inserts;
	std::set<std::pair<int64_t, int>> toilet_inserts;
	std::set<std::pair<int64_t, std::pair<int, int>>> deletes;

	for (int i = 0; i < N_toilet; i++) {
		set_pos.insert(toilets[i]);
		set_neg.insert(toilets[i]);
	}
	for (int i = 0; i < N_people; i++) {
		people_inserts.insert({person_ins_time[i], i});
	}

	std::vector<std::pair<int64_t, int64_t>> ans(N_people, {-1, -1});

	const int64_t INF = 8e18;

	people_inserts.insert({INF, -1});
	toilet_inserts.insert({INF, -1});
	deletes.insert({INF, {-1, -1}});

	auto advance_time = [&](int64_t nxt_time) -> void {
		cur_time = nxt_time;
		base_pos = (set_pos.empty() ? 0 : eval_at(*set_pos.begin()));
		base_neg = (set_neg.empty() ? 0 : eval_at(*set_neg.begin()));
	};

	auto get_event = [&](const location& a, const location& b) -> std::optional<std::pair<int64_t, std::pair<int, int>>> {
		if (a.dir > b.dir) {
			assert(a.dir - b.dir == 1);
			assert(a.dir != b.dir);
			int64_t la = eval_at(a);
			int64_t lb = eval_at(b);
			assert(la != lb);
			if (la > lb) {
				lb += L * (N_people+1);
			}
			assert(lb > la);
			int64_t delta = lb - la;
			assert(delta > 0);
			return std::pair<int64_t, std::pair<int, int>>{delta / (N_people+1), {std::max(a.id, b.id), ~std::min(a.id, b.id)}};
		} else {
			return std::nullopt;
		}
	};

	auto maybe_delete = [&](const location& a, const location& b) {
		auto evt = get_event(a, b);
		if (evt) {
			deletes.erase(*evt);
		}
	};
	auto maybe_insert = [&](const location& a, const location& b) {
		auto evt = get_event(a, b);
		if (evt) {
			deletes.insert(*evt);
		}
	};

	auto do_insert = [&](std::set<location, my_comparator>& st, location l) {
		cerr << (&st == &set_pos ? "pos" : "neg") << ' ' << "insert " << cur_time << ' ' << l.id << ' ' << l.loc_0 << '\n';
		auto [it, did_insert] = st.insert(l);
		assert(did_insert);
		assert(it != st.end());
		auto pit = it;
		if (pit == st.begin()) {
			pit = st.end();
		}
		--pit;

		auto nit = it;
		++nit;
		if (nit == st.end()) {
			nit = st.begin();
		}

		maybe_delete(*pit, *nit);
		maybe_insert(*pit, *it);
		maybe_insert(*it, *nit);
	};

	auto do_delete = [&](std::set<location, my_comparator>& st, location l) {
		cerr << (&st == &set_pos ? "pos" : "neg") << ' ' << "delete " << cur_time << ' ' << l.id << ' ' << l.loc_0 << '\n';
		auto it = st.find(l);
		assert(it != st.end());
		auto pit = it;
		if (pit == st.begin()) {
			pit = st.end();
		}
		--pit;

		auto nit = it;
		++nit;
		if (nit == st.end()) {
			nit = st.begin();
		}

		maybe_delete(*pit, *it);
		maybe_delete(*it, *nit);
		maybe_insert(*pit, *nit);
		st.erase(it);
	};

	// if many events happen at the same time,
	// the order doesn't matter since toilet positions are distinct and newer people have worse priority
	while (true) {
		auto people_evt = *people_inserts.begin();
		auto toilet_evt = *toilet_inserts.begin();
		auto delete_evt = *deletes.begin();
		int64_t nxt_time = std::min({people_evt.first, toilet_evt.first, delete_evt.first});
		if (nxt_time == INF) break;
		advance_time(nxt_time);
		if (people_evt.first == nxt_time) {
			int i = people_evt.second;
			people_inserts.erase(people_evt);
			do_insert(people[i].dir == +1 ? set_pos : set_neg, people[i]);
		} else if (toilet_evt.first == nxt_time) {
			int j = toilet_evt.second;
			toilet_inserts.erase(toilet_evt);
			do_insert(set_pos, toilets[j]);
			do_insert(set_neg, toilets[j]);
		} else if (delete_evt.first == nxt_time) {
			auto [i, j] = delete_evt.second;
			ans[i] = {toilets[j].loc_0, cur_time};
			do_delete(people[i].dir == +1 ? set_pos : set_neg, people[i]);
			do_delete(set_pos, toilets[j]);
			do_delete(set_neg, toilets[j]);

			toilet_inserts.insert({cur_time + person_duration[i], j});
		} else assert(false);
	}

	for (int i = 0; i < N_people; i++) {
		cout << ans[i].first << ' ' << ans[i].second << '\n';
	}

	return 0;
}
