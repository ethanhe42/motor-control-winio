#include <windows.h>
#include <iostream>
#include <conio.h>
#include "winio.h"

#pragma comment(lib,"winio.lib")
using namespace std;


int creat_DO(int (&DO_bit)[6])
{
	int temp=0;
  	int i=0;
  	for(i=5;i>0;i--)
   	{
    	temp=(temp+DO_bit[i])*2;
   	}
  	return temp+DO_bit[0];
  }

int creat_DI(int (&DI_bit)[6], int num)
{
  	int i=0;
  	for(i=0;i<6;i++)
   	DI_bit[i]=(num>>i)&0x0001;
  	return 0;
}

void main(void)

{
	
	unsigned short BASE_ADDRESS = 0xE880;
	int OPort=16;
	int iPort=16;
	


// 初始化WinIO 
	if (!InitializeWinIo())
	{
		cout<<"Error In InitializeWinIo!"<<endl;
		exit(1);
	}
//数字量输出

	char c;
	int shutdown= 0;
	int DO_data;
	int DO[6]={0};
	int tmp[6]={0};

	int i;
	int DI_data;
	int DI[6]={0};

	int cnt=0;
	int tmpcnt = cnt;

   while(1)
   {
	DI_data = _inp(BASE_ADDRESS + iPort);
  	creat_DI(DI,DI_data);
  	Sleep(100);
	if (kbhit()){
		c = getch();
		if (c=='\n') {
			for(i=0;i<6;i++) {
				DO[i] = 0;
			}
		}
		shutdown = 1;
	}
	else{
		cnt = 0;
		if (DI[0]){
			for (i = 0; i <6; i++) {
				DO[i] = 0;
			}
		}
		else {
			for (i = 1; i < 6; i++){
				if (DI[i] == 0) {
					cnt += 1 << (5-i);
				}
			}
			if (cnt > 27) {
				DO[5] = 0;
				DO[4] = 1;
				
				for (i = 0; i <4; i++) {
					DO[i] = 0;
				}
				shutdown = 1;
			}
			else{
				DO[4] = 0;
				DO[3] = 1;
				DO[2] =1;
				DO[1] =1;
				if (cnt > 16) {
					DO[2] = 0;
				}
				if (cnt > 10) {
					DO[1] =0;
				}
				DO[0] =0;
			}  
		}

	
	}
  	DO_data=creat_DO(DO);
  	_outp(BASE_ADDRESS + OPort, DO_data);
	if (tmpcnt != cnt){

		cout<<cnt<<endl;	
		tmpcnt = cnt;
	}
	if (shutdown) {
		Beep(800, 3000);
		break;
	}
   }
	_outp(BASE_ADDRESS + OPort, 0);
    ShutdownWinIo();													//关闭WinIO

}