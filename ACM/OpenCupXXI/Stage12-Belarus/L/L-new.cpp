#include <bits/stdc++.h>

namespace machine {
enum REGS {
	R0 = -16,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
};

enum class OPCODE {
	IMM = 0,
	LD,
	ST,
};

struct op_t {
	OPCODE code;
	int arg1;
	int arg2;
};

bool should_simulate = false;
std::vector<op_t> ops;

uint16_t _SIM_REG_BUF[16];
uint16_t *SIM_REG = _SIM_REG_BUF + 16;
uint8_t SIM_MEM[(1<<16)+1];

uint16_t read_sim_mem(int loc) {
	return uint16_t((uint16_t(SIM_MEM[loc + 1]) << 8) | uint16_t(SIM_MEM[loc]));
}

void op_imm(int reg, int val) {
	assert(-16 <= reg && reg < 0);
	assert(0 <= val && val < (1 << 16));
	if (should_simulate) {
		SIM_REG[reg] = uint16_t(val);
	} else {
		ops.push_back({OPCODE::IMM, reg, val});
	}
}
void op_ld(int dest, int src) {
	assert(-16 <= dest && dest < 0);
	assert(-16 <= src && src < 0);
	if (should_simulate) {
		SIM_REG[dest] = read_sim_mem(SIM_REG[src]);
	} else {
		ops.push_back({OPCODE::LD, dest, src});
	}
}
void op_st(int dest, int src) {
	assert(-16 <= dest && dest < 0);
	assert(-16 <= src && src < 0);
	if (should_simulate) {
		SIM_MEM[SIM_REG[dest]] = uint8_t(SIM_REG[src] & 255);
		SIM_MEM[SIM_REG[dest] + 1] = uint8_t(SIM_REG[src] >> 8);
	} else {
		ops.push_back({OPCODE::ST, dest, src});
	}
}
} // namespace machine

namespace my_program {

using namespace machine;

const int M_scratch = 0xf0;
const int R_scratch = R15;
const int R_scratch2 = R14;

void load_reg_addr(int reg, int addr) {
	assert(-16 <= reg && reg < 0);
	assert(0 <= addr && addr < (1 << 16));
	assert(reg != R_scratch);
	op_imm(R_scratch, addr);
	op_ld(reg, R_scratch);
}

void store_addr_reg(int addr, int reg) {
	assert(0 <= addr && addr < (1 << 16));
	assert(-16 <= reg && reg < 0);
	assert(reg != R_scratch);
	op_imm(R_scratch, addr);
	op_st(R_scratch, reg);
}

void store_addr_val(int addr, int val) {
	assert(0 <= addr && addr < (1 << 16));
	assert(0 <= val && val < (1 << 16));
	op_imm(R_scratch, addr);
	op_imm(R_scratch2, val);
	op_st(R_scratch, R_scratch2);
}

const int DECR_TABLE = 0xa0;
const int DECR_CARRY_TABLE = 0xa4;

const int DOUBLE_TABLE = 0xa8;
const int DOUBLE_CARRY_TABLE = 0xac;

const int INCR_TABLE = 0xb0;
const int INCR_CARRY_TABLE = 0xb4;

const int AND_TABLE = 0xc0;

const int NONZERO_TABLE = 0xc8;

const int EXTRACT_TABLE = 0xd0;

void init_tables() {
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < 256; i++) {
			store_addr_val(((DECR_TABLE + z) << 8) + i, (i-z) & 255);
			store_addr_val(((DECR_CARRY_TABLE + z) << 8) + i, (DECR_TABLE) + (i-z < 0));

			store_addr_val(((DOUBLE_TABLE + z) << 8) + i, ((i<<1) + z) & 255);
			store_addr_val(((DOUBLE_CARRY_TABLE + z) << 8) + i, DOUBLE_TABLE + (i >> 7));

			store_addr_val(((INCR_TABLE + z) << 8) + i, (i+z) & 255);
			store_addr_val(((INCR_CARRY_TABLE + z) << 8) + i, INCR_TABLE + (i+z >= 256));
		}
	}

	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
		}
	}
}

void apply_table(int table, int carry_table, int addr, int ctrl) {
}

const int R1_first_run = 0;
const int R1_late_run = 2;

void go() {
	init_tables();
}

} // namespace machine

void print_prog() {
	machine::ops.clear();
	machine::should_simulate = false;
	my_program::go();
	using std::cout;
	cout << machine::ops.size() << '\n';
	for (auto it : machine::ops) {
		switch (it.code) {
			case machine::OPCODE::IMM:
				cout << "imm";
				break;
			case machine::OPCODE::LD:
				cout << "ld";
				break;
			case machine::OPCODE::ST:
				cout << "ld";
				break;
			default:
				assert(false);
		}
		cout << ' ';
		if (it.arg1 < 0) cout << 'r' << it.arg1+16;
		else cout << it.arg1;
		cout << ", ";
		if (it.arg2 < 0) cout << 'r' << it.arg2+16;
		else cout << it.arg2;
		cout << '\n';
	}
}

uint16_t run_inp(std::vector<uint16_t> A) {
	int N = int(A.size());

	memset(machine::_SIM_REG_BUF, 0, sizeof(machine::_SIM_REG_BUF));
	memset(machine::SIM_MEM, 0, sizeof(machine::SIM_MEM));
	machine::should_simulate = true;

	for (int i = 0; i < N; i++) {
		machine::SIM_MEM[2*i] = uint8_t(A[i] & 255);
		machine::SIM_MEM[2*i+1] = uint8_t(A[i] >> 8);
	}
	machine::SIM_REG[machine::R0] = uint16_t(N);

	for (int z = 0; z < 5000; z++) {
		my_program::go();
	}

	return machine::SIM_REG[0];
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	print_prog();

	return 0;
}
