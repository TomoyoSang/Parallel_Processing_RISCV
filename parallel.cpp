#include "parallel.h"
using namespace std;
InterMem im;
Instruction_Fetch my_fet;
Instruction_Decode my_dec;
Execution my_ex;
Memory_Access my_ma;
Write_Back my_wb;


int main()
{
	char Name[38][10];
	{
		strcpy(Name[0], "LUI"); strcpy(Name[1], "AUIPC");
		strcpy(Name[2], "JAL"); strcpy(Name[3], "JALR");
		strcpy(Name[4], "BEQ"); strcpy(Name[5], "BNE");
		strcpy(Name[6], "BLT"); strcpy(Name[7], "BGE");
		strcpy(Name[8], "BLTU"); strcpy(Name[9], "BGEU");
		strcpy(Name[10], "LB"); strcpy(Name[11], "LH");
		strcpy(Name[12], "LW"); strcpy(Name[13], "LBU");
		strcpy(Name[14], "LHU"); strcpy(Name[15], "SB");
		strcpy(Name[16], "SH"); strcpy(Name[17], "SW");
		strcpy(Name[18], "ADDI"); strcpy(Name[19], "SLTI");
		strcpy(Name[20], "SLTIU"); strcpy(Name[21], "XORI");
		strcpy(Name[22], "ORI"); strcpy(Name[23], "ANDI");
		strcpy(Name[24], "SLLI"); strcpy(Name[25], "SRLI");
		strcpy(Name[26], "SRAI"); strcpy(Name[27], "ADD");
		strcpy(Name[28], "SUB"); strcpy(Name[29], "SLL");
		strcpy(Name[30], "SLT"); strcpy(Name[31], "SLTU");
		strcpy(Name[32], "XOR"); strcpy(Name[33], "SRL");
		strcpy(Name[34], "SRA"); strcpy(Name[35], "OR");
		strcpy(Name[36], "AND"); strcpy(Name[37], "ZERO");
	}

	//freopen("pi.data", "r", stdin);
	im.readin();
	int cnt = 1;
	//½»½Ó½á¹¹
	pass_oj if_end, id_end, ex_end, ma_end;

	//Îå¼¶Á÷Ë®
	while (1)
	{

		my_wb.WB(ma_end);
		reset(ma_end);

		ma_end = my_ma.MA(ex_end, im);
		reset(ex_end);

		ex_end = my_ex.Ex(id_end);
		reset(id_end);

		id_end = my_dec.Decode(if_end);
		reset(if_end);

		if (wait_time <= 0)
		{
			if_end = my_fet.fetcher(im);
		}
		if (wait_time > 0)wait_time--;
		else wait_time = 0;

		Initial(EX_exRenew);
		Initial(EX_maRenew);
		update_changes();
		Initial(EX_Forward);
		Initial(MA_Forward);
		
		//FeedBack(if_jump, ex_end, id_end, if_end);

		//cout << cnt << " " << PC << " ";
		//cout << endl;
		cnt++;
		//cout <<"EX: " <<EX_exRenew.cur_RD.rd << " " << EX_exRenew.cur_RD.rd_value << '\n';
		//cout << "MA: "<<EX_maRenew.cur_RD.rd << " " << EX_maRenew.cur_RD.rd_value << '\n';
		//cout << cnt << "*******" <<"PC: "<<PC<< endl;
		//cout << "rd:  " << RD.cur_RD.rd << "    rd_value:   " << RD.cur_RD.rd_value << endl;
		//cout << "Name: "<<Name[if_end.inst_name]<< "   " << "cur_PC:"<<if_end.cur_PC << "   ***IF "<<endl;
		//cout << "Name: " << Name[id_end.inst_name] << "   " << "cur_PC:" << id_end.cur_PC << "   ***ID " << endl;
		//cout << "Name: " << Name[ex_end.inst_name] << "   " << "cur_PC:" << ex_end.cur_PC << "   ***EX " << endl;
		//cout << "Name: " << Name[ma_end.inst_name] << "   " << "cur_PC:" << ma_end.cur_PC << "   ***MA " << endl;



		//for (int i = 1; i <= 31; ++i)
		//	cout << reg.Register[i] << " ";



		/*if (if_end.inst_name != ZERO)cout << Name[if_end.inst_name];
		else if (id_end.inst_name != ZERO)cout << Name[id_end.inst_name];
		else if (ex_end.inst_name != ZERO)cout << Name[ex_end.inst_name];
		else if (ma_end.inst_name != ZERO)
		{
			cout << Name[ma_end.inst_name]; cnt++;
	    }*/
		//cout << "\n";

		if (!flag)
		{
			cout << (reg.Register[10] & 255u);
			break;
		}
	}

	return 0;
}
