#pragma once

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>


using namespace std;

enum Inst_Name {
	LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU,
	LB, LH, LW, LBU, LHU, SB, SH, SW, ADDI, SLTI, SLTIU, XORI, ORI,
	ANDI, SLLI, SRLI, SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, ZERO
};

//hopefully my file will be accepted 

//------------------数据传输结构体：pass_oj----------------**

class pass_oj
{
public:
	//所有可能用到的数据类型
	Inst_Name inst_name = ZERO;
	int imm = 0u;
	unsigned int opcode = 0u;
	int rd = 0u;
	int rs1 = 0u;
	int rs1_value = 0u;
	int rs2 = 0u;
	int rs2_value = 0u;
	int func3 = 0u;
	int func7 = 0u;
	int shamt = 0u;
	unsigned int instruction = 0u;
	int result = 0u;
	int tmp = 0u;
	int cur_PC = 0u;
	int Pred_PC = 0u;
	int Real_PC = 0u;
	int jump_PC = 0u;
	int no_jump_PC = 0u;

	pass_oj() {};
	~pass_oj() {};
	pass_oj operator=(const pass_oj &oj)
	{
		inst_name = oj.inst_name;
		imm = oj.imm;
		opcode = oj.opcode;
		rd = oj.rd;
		rs1 = oj.rs1;
		rs1_value = oj.rs1_value;
		rs2 = oj.rs2;
		rs2_value = oj.rs2_value;
		func3 = oj.func3;
		func7 = oj.func7;
		shamt = oj.shamt;
		instruction = oj.instruction;
		result = oj.result;
		tmp = oj.tmp;
		Pred_PC = oj.Pred_PC;
		Real_PC = oj.Real_PC;
		jump_PC = oj.jump_PC;
		no_jump_PC = oj.no_jump_PC;
		cur_PC = oj.cur_PC;
		return *this;
	}

};

//创建一个空指令
//读到空指令时返回

void reset(pass_oj &cleaner)
{
	if (!(cleaner.inst_name == ZERO))
	{
		cleaner.inst_name = ZERO;
		cleaner.imm = 0u;
		cleaner.opcode = 0u;
		cleaner.rd = 0u;
		cleaner.rs1 = 0u;
		cleaner.rs1_value = 0u;
		cleaner.rs2 = 0u;
		cleaner.rs2_value = 0u;
		cleaner.func3 = 0u;
		cleaner.func7 = 0u;
		cleaner.shamt = 0u;
		cleaner.instruction = 0u;
		cleaner.result = 0u;
		cleaner.tmp = 0u;
		cleaner.cur_PC = 0u;
		cleaner.Pred_PC = 0u;
		cleaner.Real_PC = 0u;
		cleaner.jump_PC = 0u;
		cleaner.no_jump_PC = 0u;
	};
	return;
}


//----------------------寄存器 类-------------------------**
class TempStorage
{
public:
	int Register[32] = { 0 };
};
TempStorage reg;

int PC = 0;

int counter = 0;
bool flag = 1;//是否出现退出指令

//----------------------内存 类---------------------------**
class InterMem
{
public:
	uint8_t* Memery;
	//输入函数
	void readin()
	{
		char ch1;
		int pos = 0;
		int ins = 0;
		char read[20] = { 0 };
		while (cin >> read)
		{
			if (read[0] == '@')
			{
				pos = 0;
				int len = strlen(read);
				for (int i = 1; i < len; ++i)
				{
					if (i != 1)pos <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						pos += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						pos += (ch1 - 'A') + 10;
					}

				}
				counter = pos;
			}
			else
			{
				ins = 0;
				int len = strlen(read);
				for (int i = 0; i < len; ++i)
				{
					if (i != 0)ins <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						ins += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						ins += (ch1 - 'A') + 10;
					}


				}
				Memery[pos] = ins;
				pos++;
				counter++;
			}
			memset(read, 0, sizeof(read));
		}
		return;

	}
	InterMem()
	{
		Memery = new uint8_t[0x400000];
		memset(Memery, 0, 0x400000 * sizeof(uint8_t));
	}
	~InterMem() {};
};

//------------------------通知信息-------------------------**
class RD_Renew
{
public:
	RD_Renew() {};
	~RD_Renew() {};

	class RD_Forward
	{
	public:
		RD_Forward() {};
		~RD_Forward() {};

		int rd = 0;
		int rd_value = 0;
	};
	RD_Forward cur_RD;
	RD_Renew operator=(const RD_Renew &forward)
	{
		cur_RD.rd = forward.cur_RD.rd;
		cur_RD.rd_value = forward.cur_RD.rd_value;
		return *this;
	}
	//用pass_oj进行赋值
	void assignment(pass_oj &ex_end)
	{
		if (ex_end.rd != 0)
		{
			cur_RD.rd = ex_end.rd;
			cur_RD.rd_value = ex_end.result;
		}
		else
		{
			cur_RD.rd = 0;
			cur_RD.rd_value = 0;
		}
		
		return;
	}
};

RD_Renew EX_Forward;
RD_Renew MA_Forward;
RD_Renew EX_exRenew;
RD_Renew EX_maRenew;

//将通知信息归零
void Initial(RD_Renew &forward)
{
	forward.cur_RD.rd = 0;
	forward.cur_RD.rd_value = 0;
	return;
}
//Ex从诸多信息中进行选择
void Check_And_Renew(pass_oj &id_end)
{ 
	if (EX_exRenew.cur_RD.rd != EX_maRenew.cur_RD.rd)
	{
		
		if (EX_exRenew.cur_RD.rd != 0 && EX_exRenew.cur_RD.rd == id_end.rs1)
		{
			id_end.rs1_value = EX_exRenew.cur_RD.rd_value;
		}
		if (EX_exRenew.cur_RD.rd != 0 && EX_exRenew.cur_RD.rd == id_end.rs2)
		{
			id_end.rs2_value = EX_exRenew.cur_RD.rd_value;
		}

		if (EX_maRenew.cur_RD.rd != 0 && EX_maRenew.cur_RD.rd == id_end.rs1)
		{
			id_end.rs1_value = EX_maRenew.cur_RD.rd_value;
		}
		if (EX_maRenew.cur_RD.rd != 0 && EX_maRenew.cur_RD.rd == id_end.rs2)
		{
			id_end.rs2_value = EX_maRenew.cur_RD.rd_value;
		}
	}
	else if (EX_exRenew.cur_RD.rd != 0 && EX_exRenew.cur_RD.rd == EX_maRenew.cur_RD.rd)
	{
		if (EX_exRenew.cur_RD.rd != 0 && EX_exRenew.cur_RD.rd == id_end.rs1)
		{
			id_end.rs1_value = EX_exRenew.cur_RD.rd_value;
		}
		else if (EX_exRenew.cur_RD.rd != 0 && EX_exRenew.cur_RD.rd == id_end.rs2)
		{
			id_end.rs2_value = EX_exRenew.cur_RD.rd_value;
		}
	}
	return;
}
void update_changes()
{
	EX_exRenew = EX_Forward;
	EX_maRenew = MA_Forward;
	return;
}

//----------------------------分支预测-------------------------**

int Pred_True = 0;
int Pred_False = 0;

vector <pair<uint8_t, vector<uint8_t>>>history(8200, std::make_pair(0, vector<uint8_t>(16, 0)));

int wait_time = 0;
bool Pred_Ans(int cur_PC)
{ 
	bool if_jump = 0;
	uint32_t tmp = history[cur_PC].first;
	switch (history[cur_PC].second[history[cur_PC].first])
	{
	case 0:case 1:
	{
		if_jump = 0;
		break;
	}
	case 2:case 3:
	{
		if_jump = 1;
		break;
	}
	default:break;
	}
	return if_jump;
}

int Guess(int jump, int no_jump,int cur_PC)
{
	return (Pred_Ans(cur_PC))? jump : no_jump;
}

void FeedBack(pass_oj &ex_end,pass_oj &id_end,pass_oj &if_end)
{
	switch (ex_end.inst_name)
	{
	case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
	{
		if (ex_end.Pred_PC != ex_end.Real_PC)
		{
			Pred_False++;
			if (ex_end.Pred_PC == ex_end.jump_PC)
			{
				PC = ex_end.no_jump_PC;
				uint8_t tmp1 = history[ex_end.cur_PC].first,tmp2= history[ex_end.cur_PC].second[tmp1];
				switch (tmp2)
				{
				case 2:tmp2 = 1; break;
				case 3:tmp2 = 2; break;
				}
				history[ex_end.cur_PC].second[tmp1] = tmp2;
				tmp1 = ((tmp1 << 1) & 0b1111);
				history[ex_end.cur_PC].first = tmp1;
			}
			else if (ex_end.Pred_PC == ex_end.no_jump_PC)
			{
				PC = ex_end.jump_PC;
				uint8_t tmp1 = history[ex_end.cur_PC].first, tmp2 = history[ex_end.cur_PC].second[tmp1];
				switch (tmp2)
				{
				case 0:tmp2 = 1; break;
				case 1:tmp2 = 2; break;
				}
				history[ex_end.cur_PC].second[tmp1] = tmp2;
				tmp1 = ((tmp1 << 1) & 0b1111) + 1;
				history[ex_end.cur_PC].first = tmp1;
			}
			reset(id_end);
			reset(if_end);
		}
		else
		{
			Pred_True++;
			if (ex_end.Pred_PC == ex_end.jump_PC)
			{
				uint8_t tmp1 = history[ex_end.cur_PC].first, tmp2 = history[ex_end.cur_PC].second[tmp1];
				switch (tmp2)
				{
				case 2:tmp2 = 3; break;
				case 3:tmp2 = 3; break;
				}
				history[ex_end.cur_PC].second[tmp1] = tmp2;
				tmp1 = ((tmp1 << 1) & 0b1111) + 1;
				history[ex_end.cur_PC].first = tmp1;
			}
			else if (ex_end.Pred_PC == ex_end.no_jump_PC)
			{
				uint8_t tmp1 = history[ex_end.cur_PC].first, tmp2 = history[ex_end.cur_PC].second[tmp1];
				switch (tmp2)
				{
				case 0:tmp2 = 0; break;
				case 1:tmp2 = 0; break;
				}
				history[ex_end.cur_PC].second[tmp1] = tmp2;
				tmp1 = ((tmp1 << 1) & 0b1111);
				history[ex_end.cur_PC].first = tmp1;
			}
		}
		break;
	}
	default:break;
	}
	
	return;
}

//----------------------------------符号扩展函数-------------------------------**
int SignExtended(int data, int bits)//bits表示实际位数而非0-base
{
	if (data&(1 << (bits - 1)))
	{
		data |= (0xffffffff >> bits << bits);
	}
	return data;
}
//-------------------------------   Write_Back   过程------------------------**
class Write_Back
{
public:
	Write_Back() {};
	~Write_Back() {};

	void WB(pass_oj WB_end)
	{
		if (WB_end.inst_name == ZERO)
		{
			return;
		}
		switch (WB_end.inst_name)
		{
		case ADD:case SUB:case SLT:case SLTU:case XOR:
		case OR:case AND:case SLL:case SRL:case SRA:
		case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
		case ANDI:case SLLI:case SRLI:case SRAI:case JALR:
		case LUI:case AUIPC:case JAL:
		{
			if (WB_end.rd != 0)
			{
				reg.Register[WB_end.rd] = WB_end.result;
			}
			break;
		}
		case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
		{
			break;
		}
		case LB:case LH:case LW:case LBU:case LHU:
		{
			if (WB_end.rd != 0)
			{
				reg.Register[WB_end.rd] = WB_end.result;
			}
			break;
		}
		default:break;
		}
		return;
	}
};
//-------------------------------  Memory_Access 过程------------------------**
class Memory_Access
{
public:
	Memory_Access() {};
	~Memory_Access() {};

	pass_oj MA(pass_oj MA_end, InterMem &im)
	{
		if (MA_end.inst_name == ZERO)
		{
			return MA_end;
		}
		switch (MA_end.inst_name)
		{
			case SB:
			{
				if (MA_end.tmp != 0x30004)
					im.Memery[MA_end.tmp] = (MA_end.result & 255);
				else flag = 0;
				Initial(MA_Forward);
				break;
			}
			case SH:
			{
				int tmp1 = MA_end.result;
				im.Memery[MA_end.tmp] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 1] = (tmp1 & 255);
				Initial(MA_Forward);
				
				break;
			}
			case SW:
			{
				int tmp1 = MA_end.result;
				im.Memery[MA_end.tmp] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 1] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 2] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 3] = (tmp1 & 255);
				Initial(MA_Forward);
				break;
			}
			case LB:
			{
				MA_end.result = (int)im.Memery[MA_end.tmp];
				MA_Forward.assignment(MA_end);
				break;
			}
			case LH:
			{
				MA_end.result = (int)((im.Memery[MA_end.tmp + 1] << 8) + im.Memery[MA_end.tmp]);
				MA_Forward.assignment(MA_end);
				break;
			}
			case LW:
			{
				MA_end.result = (int)((im.Memery[MA_end.tmp + 3] << 24) + (im.Memery[MA_end.tmp + 2] << 16) +
					(im.Memery[MA_end.tmp + 1] << 8) + im.Memery[MA_end.tmp]);
				MA_Forward.assignment(MA_end);
				break;
			}
			case LBU:
			{
				MA_end.result =((unsigned int)im.Memery[MA_end.tmp]);
				MA_Forward.assignment(MA_end);
				break;

			}
			case LHU:
			{
				MA_end.result = ((unsigned int)((im.Memery[MA_end.tmp + 1] << 8) + im.Memery[MA_end.tmp]));
				MA_Forward.assignment(MA_end);
				break;
			}
			case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case JALR:case AUIPC:
			{
				Initial(MA_Forward);
				break;
			}
			default: 
			{
				MA_Forward.assignment(MA_end);
				break;
			}
		}
			return MA_end;
	}

};
//-------------------------------    Execution   过程------------------------**
class Execution
{
public:
	Execution() {};
	~Execution() {};

	pass_oj Ex(pass_oj execution_end)
	{
		Check_And_Renew(execution_end);
		switch (execution_end.inst_name)
		{
		//R_Type 10*****
		case ADD:
		{
			execution_end.result = execution_end.rs1_value + execution_end.rs2_value;
			EX_Forward.assignment(execution_end);
			break;
		}
		case SUB:
		{
			execution_end.result = execution_end.rs1_value - execution_end.rs2_value;
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLT:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.rs2_value) ? 1 : 0;
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLTU:
		{
			execution_end.result = ((uint32_t)execution_end.rs1_value < (uint32_t)execution_end.rs2_value) ? 1 : 0;
			EX_Forward.assignment(execution_end);
			break;
		}
		case XOR:
		{
			execution_end.result = (execution_end.rs1_value ^ execution_end.rs2_value);
			EX_Forward.assignment(execution_end);
			break;
		}
		case OR:
		{
			execution_end.result = (execution_end.rs1_value | execution_end.rs2_value);
			EX_Forward.assignment(execution_end);
			break;
		}
		case AND:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.rs2_value);
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLL:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = (execution_end.rs1_value << execution_end.tmp);
			EX_Forward.assignment(execution_end);
			break;
		}
		case SRL:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = ((uint32_t)execution_end.rs1_value >> execution_end.tmp);
			EX_Forward.assignment(execution_end);

			break;
		}
		case SRA:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = SignExtended((execution_end.rs1_value >> execution_end.tmp), 32 - execution_end.tmp);
			EX_Forward.assignment(execution_end);
			break;
		}

		//S_Type 9
		case BEQ:
		{
			execution_end.result = (execution_end.rs1_value == execution_end.rs2_value) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;
		}
		case BNE:
		{
			execution_end.result = (execution_end.rs1_value == execution_end.rs2_value) ?
				(execution_end.no_jump_PC) : (execution_end.jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;
		}
		case BLT:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.rs2_value) ? 
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;
		}
		case BGE:
		{
			execution_end.result = (execution_end.rs1_value >= execution_end.rs2_value) ? 
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;
		}
		case BLTU:
		{
			unsigned int tmp1 = execution_end.rs1_value,
				tmp2 = execution_end.rs2_value;
			execution_end.result = (tmp1 < tmp2) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;
		}
		case BGEU:
		{
			unsigned int tmp1 = execution_end.rs1_value,
				tmp2 = execution_end.rs2_value;
			execution_end.result = (tmp1 >= tmp2) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			Initial(EX_Forward);
			break;

		}
		case SB:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value & 255);
			Initial(EX_Forward);
			break;
		}
		case SH:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value & 0xffff);
			Initial(EX_Forward);
			break;
		}
		case SW:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value );
			Initial(EX_Forward);
			break;
		}
		//I_Type 16
		case JAL:
		{
			execution_end.result = execution_end.cur_PC + 4;
			EX_Forward.assignment(execution_end);

			break;
		}
		case JALR:
		{
			execution_end.result = execution_end.cur_PC + 4;
			Initial(EX_Forward);
			PC = ((execution_end.rs1_value + execution_end.imm) >> 1 << 1);
			break;
		}
		case LB:case LH:case LW:case LBU:case LHU:
		{
			if (execution_end.rd != 0)
				execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			Initial(EX_Forward);
			break;
		}
		case ADDI:
		{
			execution_end.result = (execution_end.rs1_value +execution_end.imm);
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLTI:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.imm)?1:0;
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLTIU:
		{
			execution_end.result = ((uint32_t)execution_end.rs1_value < (uint32_t)execution_end.imm) ? 1 : 0;
			EX_Forward.assignment(execution_end);
			break;
		}
		case XORI:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.imm);
			EX_Forward.assignment(execution_end);
			break;
		}
		case ORI:
		{
			execution_end.result = (execution_end.rs1_value | execution_end.imm);
			EX_Forward.assignment(execution_end);
			break;
		}
		case ANDI:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.imm);
			EX_Forward.assignment(execution_end);
			break;
		}
		case SLLI:
		{
			execution_end.result = (execution_end.rs1_value << (execution_end.shamt&31u));
			EX_Forward.assignment(execution_end);
			break;
		}
		case SRLI:
		{
			uint32_t tmp = (uint32_t)execution_end.rs1_value;
			execution_end.result = (tmp >> (execution_end.shamt & 31u));
			EX_Forward.assignment(execution_end);
			break;
		}
		case SRAI:
		{
			execution_end.result = SignExtended(execution_end.rs1_value >> (execution_end.shamt & 31u), 32-(execution_end.shamt & 31u));
			EX_Forward.assignment(execution_end);
			break;
		}
		//U_Type 2******
		case LUI:
		{
			execution_end.result = (execution_end.imm );
			EX_Forward.assignment(execution_end);
			break;
		}
		case AUIPC:
		{
			if (execution_end.rd != 0)
			{
				PC = execution_end.cur_PC + (execution_end.imm << 12);
				execution_end.result = PC;
				Initial(EX_Forward);
				//EX_Forward.assignment(execution_end);
			}

		}
		}

		
		return execution_end;
	}

};
//-------------------------------     Decode     过程------------------------**
class Instruction_Decode
{
public:
	Instruction_Decode() {};
	~Instruction_Decode() {};

	unsigned int instruction = 0;
	
	pass_oj Decode(pass_oj decode_end)
	{
		if (decode_end.inst_name == ZERO)
		{
			return decode_end;
		}

		switch (decode_end.inst_name)
		{
		case ADD:case SUB:case SLL:case SLT:case SLTU:
		case XOR:case OR:case AND:case SRL:case SRA:
		case BEQ:case BNE:case BLT:case BGE:
		case BLTU:case BGEU:
		case SB:case SH:case SW:
			decode_end.rs1_value = reg.Register[decode_end.rs1];
			decode_end.rs2_value = reg.Register[decode_end.rs2];
			break;

		case JALR:case LB:case LH:case LW:case LBU:case LHU:
		case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
		case SLLI:case SRLI:case SRAI:
				decode_end.rs1_value = reg.Register[decode_end.rs1];
			break;
		default :break;
		}
		return decode_end;
	}

};
//-------------------------------      Fetch     过程------------------------**
class Instruction_Fetch
{
public:
	unsigned int instruction = 0;

	Instruction_Fetch() {};
	~Instruction_Fetch() {};

	void Fetch(InterMem &im)
	{
		instruction = (im.Memery[PC + 3] << 24) + (im.Memery[PC + 2] << 16)
				+ (im.Memery[PC + 1] << 8) + im.Memery[PC];
		PC += 4;
		return;
	}

	pass_oj R_()
	{
		unsigned int tmp = instruction;

		unsigned int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int rd = (tmp & 31);
		tmp >>= 5;

		unsigned int func3 = (tmp & 7);
		tmp >>= 3;

		unsigned int rs1 = (tmp & 31);
		tmp >>= 5;

		unsigned int rs2 = (tmp & 31);
		tmp >>= 5;

		unsigned int func7 = (tmp & 127);

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		fetch_end.rd = rd;
		fetch_end.rs1 = rs1;
		fetch_end.rs2 = rs2;

		if (func3 == 0)
		{
			if (func7 == 0)fetch_end.inst_name = ADD;
			else fetch_end.inst_name = SUB;
		}
		else if (func3 == 1)fetch_end.inst_name = SLL;
		else if (func3 == 2)fetch_end.inst_name = SLT;
		else if (func3 == 3)fetch_end.inst_name = SLTU;
		else if (func3 == 4)fetch_end.inst_name = XOR;
		else if (func3 == 5)
		{
			if (func7 == 0)fetch_end.inst_name = SRL;
			else fetch_end.inst_name = SRA;
		}
		else if (func3 == 6)fetch_end.inst_name = OR;
		else if (func3 == 7)fetch_end.inst_name = AND;
		return fetch_end;
	}
	pass_oj I_()
	{
		unsigned int tmp = instruction;

		int opcode = (tmp & 127);
		tmp = (tmp >> 7);
		pass_oj fetch_end ;
		fetch_end.cur_PC = PC - 4;

		if (opcode == 3)
		{
			int rd = (tmp & 31);
			tmp >>= 5;

		    int func3 = (tmp & 7);
			tmp >>= 3;

			unsigned int rs1 = (tmp & 31);
			tmp >>= 5;

			int imm1 = (tmp&(4095));
			int imm = SignExtended(imm1, 12);
			
			fetch_end.rd = rd;
			fetch_end.rs1 = rs1;
			fetch_end.imm = imm;

			wait_time = 2;

			switch (func3)
			{
			case 0:fetch_end.inst_name = LB; break;
			case 1:fetch_end.inst_name = LH; break;
			case 2:fetch_end.inst_name = LW; break;
			case 4:fetch_end.inst_name = LBU; break;
			case 5:fetch_end.inst_name = LHU; break;
			}
		}
		else if (opcode == 19)
		{
			int rd = (tmp & 31);
			tmp >>= 5;

			int func3 = (tmp & 7);
			tmp >>= 3;

			int rs1 = (tmp & 31);
			tmp >>= 5;

			if (func3 == 0)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ADDI;
			}
			else if (func3 == 2)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = SLTI;
			}
			else if (func3 == 3)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = SLTIU;
			}
			else if (func3 == 4)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = XORI;
			}
			else if (func3 == 6)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ORI;
			}
			else if (func3 == 7)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ANDI;
			}
			else
			{
				int shamt1 = (tmp & 31);
				int shamt = SignExtended(shamt1, 5);
				tmp >>= 5;
				int func7 = (tmp & 127);
				if (func3 == 1)
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SLLI;
				}
				else if (func7 == 0)
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SRLI;
				}
				else
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SRAI;
				}

			}

		}
		else if (opcode == 111)
		{
			int rd = (tmp & 31);
			tmp >>= 5;
			int imm1 = (tmp & 255);
			tmp >>= 8;
			int imm2 = (tmp & 1);
			tmp >>= 1;
			int imm3 = (tmp & 1023);
			tmp >>= 10;
			int imm4 = (tmp & 1);
			int imm0 = 0;
			imm0 += (imm4 << 20);
			imm0 += (imm3 << 1);
			imm0 += (imm2 << 11);
			imm0 += (imm1 << 12);
			int imm = SignExtended(imm0, 21);

			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = JAL;

			PC = fetch_end.cur_PC + fetch_end.imm;
		}
		else
		{
			int rd = (tmp & 31);
			tmp >>= 5;
			tmp >>= 3;
			int rs1 = (tmp & 31);
			tmp >>= 5;
			int imm1 = (tmp & 4095);
			int imm = SignExtended(imm1, 12);

			wait_time = 4;
			fetch_end.rd = rd;
			fetch_end.rs1 = rs1;
			fetch_end.imm = imm;
			
			fetch_end.inst_name = JALR;
		}
		return fetch_end;
	}
	pass_oj U_()
	{
		unsigned int tmp = instruction;
		int opcode = (tmp & 127);
		tmp >>= 7;

		int rd = (tmp & 31);
		tmp >>= 5;

		int imm = (((tmp & 0xfffff) << 12)&(0xffffffff >> 12 << 12));

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		if (opcode == 55)
		{
			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = LUI;
		}
		else
		{
			wait_time = 4;

			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = AUIPC;
		}
		return fetch_end;

	};
	pass_oj S_()
	{
		int tmp = instruction;
		int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int imm1 = (tmp & 31);
		tmp >>= 5;

		int func3 = (tmp & 7);
		tmp >>= 3;

		int rs1 = (tmp & 31);
		tmp >>= 5;

		int rs2 = (tmp & 31);
		tmp >>= 5;

		unsigned int imm2 = (tmp & 127);

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		if (opcode == 99)
		{
			int imm0 = 0;
			imm0 += ((imm1 & 1) << 11);
			imm1 >>= 1;
			imm0 += ((imm1 & 15) << 1);
			imm0 += ((imm2 & 63) << 5);
			imm2 >>= 6;
			imm0 += ((imm2 & 1) << 12);
			imm0 = (imm0&(0xffffffff >> 1 << 1));
			int imm = SignExtended(imm0, 13);

			fetch_end.rs1 = rs1;
			fetch_end.rs2 = rs2;
			fetch_end.imm = imm;

			//分支预测
			fetch_end.jump_PC = fetch_end.cur_PC + fetch_end.imm;
			fetch_end.no_jump_PC = fetch_end.cur_PC + 4;
			PC = Guess(fetch_end.jump_PC, fetch_end.no_jump_PC, fetch_end.cur_PC);
			fetch_end.Pred_PC = PC;

			switch (func3)
			{
			case 0:fetch_end.inst_name = BEQ; break;
			case 1:fetch_end.inst_name = BNE; break;
			case 4:fetch_end.inst_name = BLT; break;
			case 5:fetch_end.inst_name = BGE; break;
			case 6:fetch_end.inst_name = BLTU; break;
			case 7:fetch_end.inst_name = BGEU; break;
			}
			
		}
		else
		{
			int imm0 = 0;
			imm0 += (imm1 & 31);
			imm0 += ((imm2 & 127) << 5);
			int imm = SignExtended(imm0, 12);

			fetch_end.rs1 = rs1;
			fetch_end.rs2 = rs2;
			fetch_end.imm = imm;

			switch (func3)
			{
			case 0:fetch_end.inst_name = SB; break;
			case 1:fetch_end.inst_name = SH; break;
			case 2:fetch_end.inst_name = SW; break;
			}
		}
		return fetch_end;
	};

	pass_oj fetcher(InterMem &im)
	{
		pass_oj fetch_end;
		Fetch(im);
		fetch_end.opcode = (instruction & 127);

		switch (fetch_end.opcode)
		{
		case 51:fetch_end = R_(); break;
		case 111:case 103:case 3:case 19:fetch_end = I_(); break;
		case 99:case 35:fetch_end = S_(); break;
		case 55:case 23:fetch_end = U_(); break;
		}	
		return fetch_end;
	}
};
