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
	
	freopen("lvalue2.data", "r", stdin);
	im.readin();
	int cnt = 1;
	pass_oj if_end, id_end, ex_end, ma_end;

	
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
			if_end = my_fet.fetcher(im);
		
		if (wait_time > 0)wait_time--;
		else wait_time = 0;

		Initial(EX_exRenew);
		Initial(EX_maRenew);
		update_changes();
		Initial(EX_Forward);
		Initial(MA_Forward);
		
		FeedBack(ex_end, id_end, if_end);

		cnt++;

		if (!flag)
		{
			cout << (reg.Register[10] & 255u);
			break;
		}
	}
	
	if ((Pred_False + Pred_True) != 0)cout << "\n" << ((double)Pred_True / (Pred_False + Pred_True) * 100) << "%" << "\n";
	cout << Pred_True << " " << Pred_False+Pred_True;
	return 0;
}
