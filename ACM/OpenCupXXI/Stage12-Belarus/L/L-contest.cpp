#include <bits/stdc++.h>

using namespace std;

enum op_type {
	IMM = 0,
	LD = 1,
	ST = 2,
};

struct op {
	op_type type;
	int a1;
	int a2;
};

std::vector<op> ops;

constexpr bool SHOULD_SIM = true;
uint16_t SIM_REG[16] = {0,};
uint8_t SIM_MEM[(1 << 16) + 1] = {0,};

void imm(int reg, int v) {
	assert(0 <= reg && reg < 16);
	assert(0 <= v && v < (1 << 16));
	ops.push_back(op{IMM, reg, v});
	if (SHOULD_SIM) {
		SIM_REG[reg] = uint16_t(v);
	}
}
void ld(int dest, int src) {
	assert(0 <= dest && dest < 16);
	assert(0 <= src && src < 16);
	ops.push_back(op{LD, dest, src});
	if (SHOULD_SIM) {
		SIM_REG[dest] = uint16_t((uint16_t(SIM_MEM[SIM_REG[src]+1]) << 8) + (uint16_t(SIM_MEM[SIM_REG[src]])));
	}
}
void st(int dest, int src) {
	assert(0 <= dest && dest < 16);
	assert(0 <= src && src < 16);
	ops.push_back(op{ST, dest, src});
	if (SHOULD_SIM) {
		SIM_MEM[SIM_REG[dest]+1] = uint8_t(SIM_REG[src] >> 8);
		SIM_MEM[SIM_REG[dest]] = uint8_t(SIM_REG[src] & 255);
	}
}

// R14 and R15 are caller save

const int SCRATCH = 0xe0 << 8;
void get_lo(int reg, int hi_msk) {
	assert(0 <= hi_msk && hi_msk < (1 << 8));
	imm(15, SCRATCH);
	st(15, reg);

	imm(15, SCRATCH+1);
	imm(14, hi_msk);
	st(15, 14);

	imm(15, SCRATCH);
	ld(reg, 15);
}
void get_hi(int reg, int hi_msk) {
	assert(0 <= hi_msk && hi_msk < (1 << 8));
	imm(15, SCRATCH);
	st(15, reg);

	imm(15, SCRATCH+2);
	imm(14, hi_msk);
	st(15, 14);

	imm(15, SCRATCH+1);
	ld(reg, 15);
}

void set_msk_reg(int reg_lo, int reg_hi) {
	imm(15, SCRATCH);
	st(15, reg_lo);

	imm(15, SCRATCH+1);
	st(15, reg_hi);

	imm(15, SCRATCH);
	ld(reg_lo, 15);
}

void copy(int rdest, int rsrc) {
	imm(15, SCRATCH);
	st(15, rsrc);
	ld(rdest, 15);
}

// reg must be 0/1
void ternary(int reg, int if_false, int if_true) {
	get_lo(reg, SCRATCH >> 8);

	imm(15, SCRATCH);
	imm(14, if_false);
	st(15, 14);

	imm(15, SCRATCH+1);
	imm(14, if_true);
	st(15, 14);

	ld(reg, reg);
}

void ternary_reg(int reg, int if_false, int if_true) {
	ternary(reg, 0, 2);
	get_lo(reg, SCRATCH >> 8);

	imm(15, SCRATCH);
	st(15, if_false);

	imm(15, SCRATCH+2);
	st(15, if_true);

	ld(reg, reg);
}

void select_table(int reg, int a) {
	ternary(reg, a >> 8, (a >> 8) + 1);
}

const int EXTRACT_TABLE = 0x90 << 8;

const int INCR_TABLE = 0xb0 << 8;
const int INCR_CARRY_TABLE = 0xb3 << 8;

const int DECR_TABLE = 0xb6 << 8;
const int DECR_CARRY_TABLE = 0xb9 << 8;

const int NONZERO_TABLE = 0xc0 << 8;

const int AND_TABLE = 0xc2 << 8;
const int OR_TABLE = 0xc4 << 8;

const int DOUBLE_TABLE = 0xc8 << 8;
const int DOUBLE_CARRY_TABLE = 0xcb << 8;

void init_tables() {
	for (int z = 1; z >= 0; z--) {
		for (int i = 1; i >= 0; i--) {
			imm(15, AND_TABLE + (z << 8) + i);
			imm(14, bool(i && z));
			st(15, 14);

			imm(15, OR_TABLE + (z << 8) + i);
			imm(14, bool(i || z));
			st(15, 14);
		}
	}

	for (int i = 255; i >= 0; i--) {
		imm(15, NONZERO_TABLE + i);
		imm(14, bool(i));
		st(15, 14);
	}

	for (int z = 1; z >= 0; z--) {
		for (int i = 255; i >= 0; i--) {
			imm(15, INCR_TABLE + (z << 8) + i);
			imm(14, i+z);
			st(15, 14);

			imm(15, INCR_CARRY_TABLE + (z << 8) + i);
			imm(14, (INCR_TABLE >> 8) + (i+z >= 256));
			st(15, 14);

			imm(15, DECR_TABLE + (z << 8) + i);
			imm(14, (i-z) & 255);
			st(15, 14);

			imm(15, DECR_CARRY_TABLE + (z << 8) + i);
			imm(14, (DECR_TABLE >> 8) + (i-z < 0));
			st(15, 14);

			imm(15, DOUBLE_TABLE + (z << 8) + i);
			imm(14, ((i << 2) + z) & 255);
			st(15, 14);

			imm(15, DOUBLE_CARRY_TABLE + (z << 8) + i);
			imm(14, (DOUBLE_TABLE >> 8) + (((i << 2) + z) >= 256));
			st(15, 14);
		}
	}

	for (int z = 7; z >= 0; z--) {
		for (int i = 255; i >= 0; i--) {
			imm(15, EXTRACT_TABLE + (z << 8) + i);
			imm(14, (i >> z) & 1);
			st(15, 14);
		}
	}
}

void run_not(int reg) {
	ternary(reg, 1, 0);
}

void run_and(int reg_1, int reg_2) {
	select_table(reg_2, AND_TABLE);
	set_msk_reg(reg_1, reg_2);
	ld(reg_1, reg_1);
}

void run_or(int reg_1, int reg_2) {
	select_table(reg_2, OR_TABLE);
	set_msk_reg(reg_1, reg_2);
	ld(reg_1, reg_1);
}

void nonzero(int reg) {
	get_lo(reg, NONZERO_TABLE >> 8);
	ld(reg, reg);
}

const int RESP_LOC = 0x80 << 8;

// We're going to commandeer R12 and R13 for this

void conditional_incr(int flg) {
	copy(13, flg);
	select_table(13, INCR_CARRY_TABLE);

	imm(15, RESP_LOC);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(13, 12); // now 13 is INCR or INCR+1

	imm(15, RESP_LOC+1);
	ld(12, 15);
	set_msk_reg(12, 13);
	ld(11, 12);

	copy(13, flg);
	select_table(13, INCR_TABLE);

	imm(15, RESP_LOC);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(12, 12);

	imm(15, RESP_LOC);
	st(15, 12);
	imm(15, RESP_LOC+1);
	st(15, 11);
}

void double_resp() {
	imm(13, DOUBLE_CARRY_TABLE >> 8);

	imm(15, RESP_LOC);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(13, 12); // now 13 is DOUBLE or DOUBLE+1

	imm(15, RESP_LOC+1);
	ld(12, 15);
	set_msk_reg(12, 13);
	ld(11, 12);

	imm(13, DOUBLE_TABLE >> 8);

	imm(15, RESP_LOC);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(12, 12);

	imm(15, RESP_LOC);
	st(15, 12);
	imm(15, RESP_LOC+1);
	st(15, 11);
}

const int R0_orig = 0x79 << 8;
const int R1_orig = R0_orig + 10;

const int N = RESP_LOC + 100;

void decr() {
	imm(13, DECR_CARRY_TABLE >> 8);

	imm(15, N);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(13, 12); // now 13 is DECR or DECR+1

	imm(15, N+1);
	ld(12, 15);
	set_msk_reg(12, 13);
	ld(11, 12);

	imm(13, DECR_TABLE >> 8);

	imm(15, N);
	ld(12, 15);

	set_msk_reg(12, 13);
	ld(12, 12);

	imm(15, N);
	st(15, 12);
	imm(15, N+1);
	st(15, 11);
}

void go() {
	// if not R1, then N = R0 and R0 = 1
	copy(10, 1);
	ternary(10, N, N+2);
	get_lo(10, N >> 8);
	st(10, 0);

	imm(15, R0_orig);
	st(15, 0);

	imm(15, R0_orig+2);
	imm(14, 1);
	st(15, 14);

	copy(10, 1);
	ternary(10, R0_orig+2, R0_orig);
	get_lo(10, R0_orig >> 8);
	ld(0, 10);

	imm(1, 1);

	imm(15, R0_orig);
	st(15, 0);

	// if N == 0: N = 1 and mem[0] = 1
	imm(15, N);
	ld(2, 15);
	imm(15, N+1);
	ld(3, 15);

	nonzero(2);
	nonzero(3);
	run_or(2, 3);
	copy(3, 2);

	// R2 and R3 is N > 0
	imm(15, N);
	ld(4, 15);
	imm(5, 1);
	ternary_reg(3, 5, 4);
	imm(15, N);
	st(15, 3);

	imm(15, 0);
	ld(4, 15);
	imm(5, 1);
	ternary_reg(2, 5, 4);
	imm(15, N);
	st(15, 2);

	decr();

	// Take N and double it
	imm(15, N);
	ld(10, 15);
	imm(15, RESP_LOC);
	st(15, 10);
	double_resp();
	imm(15, RESP_LOC);
	ld(10, 15);

	imm(15, RESP_LOC);
	imm(14, 0);
	st(15, 14);

	// now 10 is the index to look at
	ld(9, 10);
	imm(15, R1_orig);
	st(15, 9);

	// R0_orig and R1_orig contain the multiplicands
	// time to start multiplying
	for (int s = 15; s >= 0; s--) {
		for (int i = 0, j = s; i <= s; i++, j--) {
			// now get the ith bit of R0_orig and jth bit of R1_orig
			imm(15, R0_orig + (i >= 8));
			ld(9, 15);
			get_hi(9, (EXTRACT_TABLE >> 8) + (i % 8));
			ld(9, 9);
			imm(15, R1_orig + (j >= 8));
			ld(10, 15);
			get_hi(10, (EXTRACT_TABLE >> 8) + (j % 8));
			ld(10, 10);

			run_and(9, 10);
			conditional_incr(9);
		}

		if (s) double_resp();
	}

	imm(15, RESP_LOC);
	ld(0, 15);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	go();

	cout << ops.size() << '\n';
	for (auto it : ops) {
		switch(it.type) {
			case IMM:
				cout << "imm r" << it.a1 << ", " << it.a2 << '\n';
				break;
			case LD:
				cout << "ld r" << it.a1 << ", r" << it.a2 << '\n';
				break;
			case ST:
				cout << "st r" << it.a1 << ", r" << it.a2 << '\n';
				break;
			default: assert(false);
		}
	}

	return 0;
}
