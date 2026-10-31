#include <bits/stdc++.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#include "geodeb.h"
#pragma GCC diagnostic pop

using namespace std;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

int signum(int a) { return (a > 0) - (a < 0); }

using P = Point<int64_t>;

int T, D, W, G;
int64_t D2;
std::vector<std::pair<int, int>> accel_ranges(W);

int get_accel(int w) {
	auto it = std::lower_bound(accel_ranges.begin(), accel_ranges.end(), w, [](const auto& a, int b) {
		return a.first < b;
	});
	if (it == accel_ranges.end()) return 0;
	else return it->second;
}

struct gift_t {
	std::string name;

	int score;
	int weight;
	P pos;
};
std::vector<gift_t> gifts;

std::string TEST_ID;

struct state_t {
	P pos = P(0, 0);
	P vel = P(0, 0);
	int num_carrots = 0;
	int weight = 0;
	int score = 0;
	int timestamp = 0;
	std::vector<bool> gift_loaded;
	std::vector<bool> gift_delivered;
	std::vector<string> ops;

	state_t() {
		gift_loaded.resize(G);
		gift_delivered.resize(G);
	}

	void LoadCarrots(int i) {
		assert(i >= 0);
		assert(pos.dist2() <= D2);
		if (i == 0) return;
		ops.push_back("LoadCarrots " + std::to_string(i));
		num_carrots += i;
		weight += i;
	}
	void LoadCarrotsTo(int i) {
		if (num_carrots >= i) return;
		LoadCarrots(i - num_carrots);
	}

	void LoadGift(int idx) {
		assert(!gift_loaded[idx] && !gift_delivered[idx]);
		assert(pos.dist2() <= D2);
		ops.push_back("LoadGift " + gifts[idx].name);
		gift_loaded[idx] = true;
		weight += gifts[idx].weight;
	}
	void DeliverGift(int idx) {
		assert(gift_loaded[idx] && !gift_delivered[idx]);
		assert((pos - gifts[idx].pos).dist2() <= D2);
		ops.push_back("DeliverGift " + gifts[idx].name);
		gift_loaded[idx] = false;
		weight -= gifts[idx].weight;
		score += gifts[idx].score;
		gift_delivered[idx] = true;
	}

	int max_accel() const {
		return get_accel(weight);
	}

	void AccelX(int64_t dx) {
		if (dx == 0) return;
		assert(num_carrots > 0);
		if (dx > 0) ops.push_back("AccRight " + std::to_string(dx));
		else if (dx < 0) ops.push_back("AccLeft " + std::to_string(-dx));
		num_carrots--;
		weight--;
		vel.x += dx;
	}
	void AccelY(int64_t dy) {
		if (dy == 0) return;
		assert(num_carrots > 0);
		if (dy > 0) ops.push_back("AccUp " + std::to_string(dy));
		else if (dy < 0) ops.push_back("AccDown " + std::to_string(-dy));
		num_carrots--;
		weight--;
		vel.y += dy;
	}
	void Accel(P a) {
		assert(a.x == 0 || a.y == 0);
		if (a.x == 0) AccelY(a.y);
		else AccelX(a.x);
	}

	void Float() {
		ops.push_back("Float 1");
		pos = pos + vel;
		timestamp++;
	}
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> T >> D >> W >> G; D2 = D*D;
	accel_ranges.resize(W);
	for (auto& [l, a] : accel_ranges) {
		cin >> l >> a;
	}
	gifts.resize(G);
	for (auto& g : gifts) {
		cin >> g.name >> g.score >> g.weight >> g.pos.x >> g.pos.y;
	}
	if (T == 15) TEST_ID = "A";
	else if (T == 450) TEST_ID = "B";
	else if (T == 5000) TEST_ID = "C";
	else if (T == 10000) TEST_ID = "D";
	else if (T == 1200) TEST_ID = "E";
	else if (T == 600) TEST_ID = "F";
	else TEST_ID = "X";

	GD_INIT(TEST_ID + "_gd.html");
	GD_LOG_TO_STDERR(false);

	int max_weight = 0;
	int max_score = 0;
	int64_t max_pos = 0;
	for (const auto& g : gifts) {
		max_weight = std::max(max_weight, g.weight);
		max_score = std::max(max_score, g.score);
		max_pos = std::max(max_pos, std::abs(g.pos.x));
		max_pos = std::max(max_pos, std::abs(g.pos.y));
	}

	for (const auto& g : gifts) {
		if (g.pos.dist2() <= D2) continue;
		GD_POINT(g.pos.x, g.pos.y);
		GD_CIRCLE(g.pos.x, g.pos.y, D);
		GD_CIRCLE(g.pos.x, g.pos.y, double(g.score) / max_score * (max_pos * 0.005) , "red");
		GD_CIRCLE(g.pos.x, g.pos.y, double(g.weight) / max_weight * (max_pos * 0.002) , "blue");
	}

	GD_POINT(0., 0.);
	for (auto [_, a] : accel_ranges) {
		GD_CIRCLE(0., 0., a, "green");
	}

	state_t global_state;

	for (int i = 0; i < int(gifts.size()); i++) {
		const auto& g = gifts[i];
		if (g.pos.dist2() <= D2) {
			global_state.LoadGift(i);
			global_state.DeliverGift(i);
			assert(global_state.gift_delivered[i]);
		}
	}

	std::vector<P> dirs({P(120, 250), P(244, 244), P(235, 82), P(204, -114), P(250, -260), P(40, -250), P(-30, -260), P(-250, -250), P(-274, -134), P(-285, 50), P(-265, 270), P(-100, 260)});
	assert(dirs.size() == 12);
	std::vector<std::vector<int>> path_presents(dirs.size());
	for (int i = 0; i < G; i++) {
		if (global_state.gift_delivered[i]) continue;
		const auto& g = gifts[i];
		double best = 1e99;
		int best_dir = -1;
		for (int z = 0; z < int(dirs.size()); z++) {
			if (g.pos.dot(dirs[z]) > 0) {
				double v = std::abs(g.pos.cross(dirs[z]) / dirs[z].dist());
				if (v < best) {
					best = v;
					best_dir = z;
				}
			}
		}
		path_presents[best_dir].push_back(i);
		GD_POINT(g.pos, (best_dir & 1) ? "red" : "blue");
	}

	for (int z = 0; z < 12; z++) {
		std::sort(path_presents[z].begin(), path_presents[z].end(), [&](int a, int b) {
			return gifts[a].pos.dist2() < gifts[b].pos.dist2();
		});
	}

	auto load_presents = [&](state_t& state, std::vector<int> presents) -> void {
		state.LoadCarrotsTo(100);
		for (int i : presents) {
			state.LoadGift(i);
		}
	};

	auto do_lineup = [&](state_t& state, P target_pos, P target_vel, int timesteps) -> bool {
		int64_t max_accel = state.max_accel();
		while (state.vel.x != 0 || state.vel.y != 0) {
			if (std::abs(state.vel.x) > std::abs(state.vel.y)) {
				state.AccelX(-std::min(std::abs(state.vel.x), max_accel) * signum(state.vel.x));
			} else {
				state.AccelY(-std::min(std::abs(state.vel.y), max_accel) * signum(state.vel.y));
			}
			state.Float();
		}
		while (state.vel.x != target_vel.x || state.vel.y != target_vel.y) {
			P dvel = target_vel - state.vel;
			P dpos = target_pos - state.pos;
		}
	};

	auto deliver_line = [&](state_t& state, P dest, std::vector<int> presents, int decel_after) -> void {
		assert(state.pos.dist2() <= D2);
		load_presents(state, presents);
		std::reverse(presents.begin(), presents.end());
		while (!presents.empty()) {
			if ((state.pos - gifts[presents.back()].pos).dist2() <= D2) {
				state.DeliverGift(presents.back());
				presents.pop_back();
			} else {
				// if possible, deccelerate first
				int max_accel = state.max_accel();
				// we want to get to dir, so we should change our angle to match it
				//dir - state.pos;
				//-signum(state.vel.x);
				//-signum(state.vel.y);
				state.Float();
			}
		}
	};

	global_state.LoadCarrotsTo(100);
	for (int i = 0; i < 2; i++) {
		global_state.AccelX(-4);
		global_state.Float();
		global_state.AccelY(-4);
		global_state.Float();
		global_state.AccelY(-4);
		global_state.Float();
		global_state.AccelX(-4);
		global_state.Float();
	}
	for (int i = 0; i < 2; i++) {
		global_state.AccelX(4);
		global_state.Float();
		global_state.AccelY(4);
		global_state.Float();
		global_state.AccelY(4);
		global_state.Float();
		global_state.AccelX(4);
		global_state.Float();
	}

	cerr << global_state.pos << ' ' << global_state.vel << '\n';

	for (int i = 0; i < 3; i++) {
		global_state.AccelX(3);
		global_state.Float();
		global_state.AccelY(3);
		global_state.Float();
		global_state.AccelY(3);
		global_state.Float();
		global_state.AccelX(3);
		global_state.Float();
	}

	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';
	deliver_line(global_state, dirs[1], path_presents[1], 2);
	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';
	for (int i = 0; i < 5; i++) {
		global_state.AccelX(-4);
		global_state.Float();
		global_state.AccelY(-4);
		global_state.Float();
		global_state.AccelY(-4);
		global_state.Float();
		global_state.AccelX(-4);
		global_state.Float();
	}
	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';
	deliver_line(global_state, dirs[7], path_presents[7], 2);
	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';
	global_state.AccelX(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();

	global_state.AccelX(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();

	global_state.AccelX(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();

	global_state.AccelX(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();
	global_state.AccelX(4); global_state.Float();

	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();
	global_state.AccelY(4); global_state.Float();

	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();
	global_state.Float();

	cerr << global_state.pos << ' ' << global_state.vel << ' ' << global_state.timestamp << '\n';

	cout << global_state.ops.size() << '\n';
	for (auto op : global_state.ops) {
		cout << op << '\n';
	}

	return 0;
}
