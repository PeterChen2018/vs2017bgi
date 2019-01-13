// RESET
//    94.01  配合 UTC 修改
//    94.11  增加 14ABK
//    96.07  英吋/公厘 選擇
//    98.02  增加 14AHK
//


#include "graphics.h"

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "reset.h"
#include "english.h"


void inputxy(int x, int y, char *str1 );
void gotoxy(int x, int y) ; 
void clrscr();



CFONT  cfont;  // 中文字型的開啟情形



typedef struct
      {
	int   CHINA;		  // =1 為中文畫面  =0 為英文畫面
	int   DIGIT_NUM;	  // 數位板的編號  =0 無  =2 CALCOMP
	int   DIGIT_type;	  // 數位板的輸出方式  0= 無  1=RUN 2=POINT 3=TRACK 4=LINE
	int   DIGIT_format;	  // 數位板的輸出格式 1=Binary5 2=Binary8 3=UIOFmm 4=ASCIImm
	int   DIGIT_COM;	  // 數位板的COM 號碼
	int   DIGIT_COM_IO;	  // 數位板的IO起始位置
	float DIGIT_scale;	  // 數位板輸出尺寸/實際尺寸
	long  DIGIT_baud;	  // 傳輸速率
	int   DIGIT_parity;	  // 同位元 None, Odd, Even
	int   DIGIT_data;	  // 資料長度
	int   DIGIT_stop;	  // 停止位元數
	int   DIGIT_retry;	  // 重讀次數
	int   PMAC_type;	  // =0 PMAC  =1 UTC  =2 SMCC
	int   PMAC_COM; 	  // PMAC的COM 號碼
	int   PMAC_COM_IO;	  // PMAC的IO起始位置
	long  PMAC_baud;	  // 傳輸速率
	int   PMAC_parity;	  // 同位元 None, Odd, Even
	int   PMAC_data;	  // 資料長度
	int   PMAC_stop;	  // 停止位元數
	int   PMAC_retry;	  // 重讀次數

	int   MACHINE_type;	  // 機器型號 0=通用  1=14A(線刀)  2=14AK  3=14AVK  4=14AVHK  5=14ABK 6=14AHK
	float BladeWide;	  // 帶刀寬度
	float WireDia;		  // 線刀直徑
	int   AxisUse;		  // 使用的軸數
	long  CutSpeed; 	  // 預設的切割速度
	long  RotSpeed; 	  // 預設的轉刀速度
	int   CutType;		  // 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
	char  Cut_LU[10];	  // 刀左上 的 指令  14A P1007	14AK P1000  14AVK P1005
	char  Cut_U[10];	  // 刀  上 的 指令	 P1006	     P1001	  P1006
	char  Cut_RU[10];	  // 刀右上 的 指令	 P1005	     P1002	  P1007
	char  Cut_L[10];	  // 刀左   的 指令	 P1004	     P1003	  P1003
	char  Cut_R[10];	  // 刀右   的 指令	 P1003	     P1004	  P1004
	char  Cut_LD[10];	  // 刀左下 的 指令	 P1002	     P1005	  P1000
	char  Cut_D[10];	  // 刀  下 的 指令	 P1001	     P1006	  P1001
	char  Cut_RD[10];	  // 刀右下 的 指令	 P1000	     P1007	  P1002
	int   UnitMode; 	  // 1=公制 mm	2=英制inch
	int   Reserve02;
	int   Reserve03;
	int   Reserve04;
      } DATA;

DATA  Data;			  // 目前設備的規劃現況

int DATA_default();		  // 取得預設值
int showscr();			  // 顯示 設定畫面
int bartext(int nitem, int num);
int setcurso(int mode); 	  // 將游標隱藏或顯現


#define item  7        // 共有多少選項


int  pos[item][2]      //  201807 各選項的座標 
       = {  118,55  ,118,67  ,118,84  ,102,164  ,102,178   ,401,55  ,401,67  };

//int  pos[item][2]      // 各選項的座標
//       = {  14,4,  14,5,  14,6,  12,11,  12,12,  52,4,	52,5  };


int  nmin[item] = {  0,  1,  1,  1,  0,  1, 0 };    // 設定範圍最小值
int  nmax[item] = {  1,  6,  2,  4,  3,  4, 1 };    // 201807  設定範圍最大值

// int  nmax[item] = {  1,  5,  2,  4,  3,  4, 1 };    // 設定範圍最大值

int  pset[item];   // 目前之選項值


int main()
{
  FILE	*fin, *fout;
  int	FileFlag;
  char	Dkeyin[2];
  char	tmpstr[256];
  int	i, key, nitem;
  
  int first = 1 ;
  
  
  initwindow( 640 ,480 ,"Chart" );
  // initwindow( 800 ,600 ,"Chart" );
  
  
  
  if ((cfont.Handl=open("SKFONT16.DAT",0x8000))<0) // 開啟中文字型檔案, 二進位模式
  { printf("Can't open font file!"); system("pause"); return(2); }
  
  
  cfont.Wmode   = COPY_PUT; // cprstr 印字時之方式, 有 COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT 五種方式
  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
  cfont.Cx  = 16;		       // 橫向量
  cfont.Cy  =  0;		       // 縱向量
  cfont.Cw  =  2;		       // 字寬 1=8點 2=16點
  
  
  
  
  
  
   
  if ((fin=fopen("SKDATA02.DAT","rb")) != NULL)
    {
      if (fread(&Data, sizeof(DATA), 1, fin) == 0)  DATA_default();
      FileFlag = 1;
      fclose(fin);
    }
  else	{ DATA_default();  // 取得預設值
	  FileFlag = 0;
	}
  
  //printf("FileFlag=%d \n", FileFlag);
  //system("pause") ;
  //clrscr();
  
  setcurso(0);	   // 隱藏游標
  showscr();	   // 顯示 設定畫面
  
  //system("pause") ;
  
  // **** 增加選項時, 這裡亦須修改 ****
  pset[0] = Data.CHINA; 	   // 語言
  pset[1] = Data.MACHINE_type;	   // 機種
  pset[2] = Data.UnitMode;	   // 單位  1=mm 2=in
  pset[3] = Data.DIGIT_COM;	   // 數位板 COM 埠
  pset[4] = Data.DIGIT_NUM;	   // 數位板 機種
  pset[5] = Data.PMAC_COM;	   // 控制器 COM 埠
  pset[6] = Data.PMAC_type;	   // 控制器 型號  0=PMAC 1=UTC 2=SMCC
  
  
  for (i=0; i<item; i++)   bartext(i, pset[i]);
  nitem = 0;	   // 預設選項號碼
  
  //textattr(BLACK+(LIGHTGRAY<<4));
  
  setcolor(RED);
  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
  bartext( nitem, pset[nitem] );
  setcolor(WHITE);
  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
 
  
  int keepgoing = 1; 
  char c;
  while ( keepgoing )
  {
   
   	if (kbhit()) {
		// key = getch();
		c = (char) getch( );
    	if ( c == 27 )	 // ESC 則跳離
    	{
  	 		keepgoing = 0;
    	}
   	}
   	
	//delay(500);	printf("x= %d y=%d c=%d \n" ,mousex() ,mousey() ,c ); if (kbhit()) printf("kbhit \n" );
	
    switch (c)
	{
	  case	 0 :		 // 功能鍵
	    
	    // printf("function key %d \n" ,c );
	    
	    if( first == 0 ) c = (char) getch();
	    if( first == 1 ) {
	    	first = 0 ;
	    	c = 32 ;
		}
	    
	    // printf("fk getch %d \n" ,c );
	    
	    switch ( c )
	    {   case  60 :	 // F02 鍵 save and exit
		     int Ferror;
		     switch (Data.MACHINE_type) 	     // 機種
		     {  case 1 :  // SA-14A
			      Data.BladeWide = 0;	// 帶刀寬度
			      Data.WireDia   = 1.2;	// 線刀直徑
			      Data.AxisUse  = 2;		// 使用的軸數
			      Data.CutSpeed =  3000;	// 預設的切割速度
			      Data.RotSpeed = 12000;	// 預設的轉刀速度 (不使用)
			      Data.CutType  = 0;		// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
			      if (Data.PMAC_type == 0)	// PMAC
 				  { strcpy(Data.Cut_LU, "P1007");    // 刀左上 的 指令   P1000     14AVK P1005
				    strcpy(Data.Cut_U,  "P1006");    // 刀  上 的 指令   P1001           P1006
				    strcpy(Data.Cut_RU, "P1005");    // 刀右上 的 指令   P1002           P1007
				    strcpy(Data.Cut_L,  "P1004");    // 刀左   的 指令   P1003           P1003
				    strcpy(Data.Cut_R,  "P1003");    // 刀右   的 指令   P1004           P1004
				    strcpy(Data.Cut_LD, "P1002");    // 刀左下 的 指令   P1005           P1000
				    strcpy(Data.Cut_D,  "P1001");    // 刀  下 的 指令   P1006           P1001
				    strcpy(Data.Cut_RD, "P1000");    // 刀右下 的 指令   P1007           P1002
				  }
			      else  // UTC  AK type
				  { strcpy(Data.Cut_LU, "P1000");
				    strcpy(Data.Cut_U,  "P1001");
				    strcpy(Data.Cut_RU, "P1002");
				    strcpy(Data.Cut_L,  "P1003");
				    strcpy(Data.Cut_R,  "P1004");
				    strcpy(Data.Cut_LD, "P1005");
				    strcpy(Data.Cut_D,  "P1006");
				    strcpy(Data.Cut_RD, "P1007");
				  }
			      break;
			    case 2 :  // SA-14AK
			      Data.BladeWide = 4.5;	// 帶刀寬度
			      Data.WireDia   = 0;	// 線刀直徑
			      Data.AxisUse  = 3;		// 使用的軸數
			      Data.CutSpeed =  4000;	// 預設的切割速度
			      Data.RotSpeed = 12000;	// 預設的轉刀速度
			      Data.CutType  = 2;	// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
			      strcpy(Data.Cut_LU, "P1000");    // 刀左上 的 指令   P1000     14AVK P1005
			      strcpy(Data.Cut_U,  "P1001");    // 刀  上 的 指令   P1001           P1006
			      strcpy(Data.Cut_RU, "P1002");    // 刀右上 的 指令   P1002           P1007
			      strcpy(Data.Cut_L,  "P1003");    // 刀左   的 指令   P1003           P1003
			      strcpy(Data.Cut_R,  "P1004");    // 刀右   的 指令   P1004           P1004
			      strcpy(Data.Cut_LD, "P1005");    // 刀左下 的 指令   P1005           P1000
			      strcpy(Data.Cut_D,  "P1006");    // 刀  下 的 指令   P1006           P1001
			      strcpy(Data.Cut_RD, "P1007");    // 刀右下 的 指令   P1007           P1002
			      break;
			    case 3 :  // SA-14AVK
			      Data.BladeWide = 3.6;	// 帶刀寬度
			      Data.WireDia   = 0;	// 線刀直徑
			      Data.AxisUse  = 4;		// 使用的軸數
			      Data.CutSpeed = 15000;	// 預設的切割速度
			      Data.RotSpeed = 15000;	// 預設的轉刀速度
			      Data.CutType  = 2;	// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
			      strcpy(Data.Cut_LU, "P1005");    // 刀左上 的 指令   14AVK P1005
			      strcpy(Data.Cut_U,  "P1006");    // 刀  上 的 指令         P1006
			      strcpy(Data.Cut_RU, "P1007");    // 刀右上 的 指令         P1007
			      strcpy(Data.Cut_L,  "P1003");    // 刀左   的 指令         P1003
			      strcpy(Data.Cut_R,  "P1004");    // 刀右   的 指令         P1004
			      strcpy(Data.Cut_LD, "P1000");    // 刀左下 的 指令         P1000
			      strcpy(Data.Cut_D,  "P1001");    // 刀  下 的 指令         P1001
			      strcpy(Data.Cut_RD, "P1002");    // 刀右下 的 指令         P1002
			      break;
			    case 4 :  // SA-14AVHK
			      Data.BladeWide = 3.6;	// 帶刀寬度
			      Data.WireDia   = 0;	// 線刀直徑
			      Data.AxisUse  = 8;		// 使用的軸數
			      Data.CutSpeed = 15000;	// 預設的切割速度
			      Data.RotSpeed = 15000;	// 預設的轉刀速度
			      Data.CutType  = 4;		// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
			      strcpy(Data.Cut_LU, "P1005");    // 刀左上 的 指令   14AVK P1005
			      strcpy(Data.Cut_U,  "P1006");    // 刀  上 的 指令         P1006
			      strcpy(Data.Cut_RU, "P1007");    // 刀右上 的 指令         P1007
			      strcpy(Data.Cut_L,  "P1003");    // 刀左   的 指令         P1003
			      strcpy(Data.Cut_R,  "P1004");    // 刀右   的 指令         P1004
			      strcpy(Data.Cut_LD, "P1000");    // 刀左下 的 指令         P1000
			      strcpy(Data.Cut_D,  "P1001");    // 刀  下 的 指令         P1001
			      strcpy(Data.Cut_RD, "P1002");    // 刀右下 的 指令         P1002
			      break;
			    case 5 :  // SA-14ABK
			      Data.BladeWide = 0;	// 帶刀寬度
			      Data.WireDia   = 0;	// 線刀直徑
			      Data.AxisUse  = 3;		// 使用的軸數
			      Data.CutSpeed =  4000;	// 預設的切割速度
			      Data.RotSpeed = 12000;	// 預設的轉刀速度
			      Data.CutType  = 5;		// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 5=14ABK	99=線,帶刀通用
			      strcpy(Data.Cut_LU, "P1000");    // 刀左上 的 指令   P1000     14AVK P1005
			      strcpy(Data.Cut_U,  "P1001");    // 刀  上 的 指令   P1001           P1006
			      strcpy(Data.Cut_RU, "P1002");    // 刀右上 的 指令   P1002           P1007
			      strcpy(Data.Cut_L,  "P1003");    // 刀左   的 指令   P1003           P1003
			      strcpy(Data.Cut_R,  "P1004");    // 刀右   的 指令   P1004           P1004
			      strcpy(Data.Cut_LD, "P1005");    // 刀左下 的 指令   P1005           P1000
			      strcpy(Data.Cut_D,  "P1006");    // 刀  下 的 指令   P1006           P1001
			      strcpy(Data.Cut_RD, "P1007");    // 刀右下 的 指令   P1007           P1002
			      break;
			    case 6 :  // SA-14AHK
			      Data.BladeWide = 3.6;	// 帶刀寬度
			      Data.WireDia   = 0;	// 線刀直徑
			      Data.AxisUse  = 6;		// 使用的軸數
			      Data.CutSpeed = 15000;	// 預設的切割速度
			      Data.RotSpeed = 15000;	// 預設的轉刀速度
			      Data.CutType  = 4;		// 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 5=14ABK	99=線,帶刀通用
			      strcpy(Data.Cut_LU, "P1000");    // 刀左上 的 指令   
			      strcpy(Data.Cut_U,  "P1001");    // 刀  上 的 指令   
			      strcpy(Data.Cut_RU, "P1002");    // 刀右上 的 指令   
			      strcpy(Data.Cut_L,  "P1003");    // 刀左   的 指令   
			      strcpy(Data.Cut_R,  "P1004");    // 刀右   的 指令   
			      strcpy(Data.Cut_LD, "P1005");    // 刀左下 的 指令   
			      strcpy(Data.Cut_D,  "P1006");    // 刀  下 的 指令   
			      strcpy(Data.Cut_RD, "P1007");    // 刀右下 的 指令   
			      break;    			      
		     }
             
		     //normvideo();
		     
			 cleardevice();
		     
		     // gotoxy(26,23);  printf("                        ");
		     // gotoxy(15,23);  printf("Pass Word : "); 
		     // gets( tmpstr );
		     // if (strncmp(tmpstr, "SUNKIST", 12) == 0) 
		     
		     
		     //cprstr( 118 ,95 , "Pass Word : " ); 
			 //inputxy( 230 , 95, tmpstr ) ; 
		     //cprstr( 230 , 115 , tmpstr ); 
			 
  			int x1, y1 ;
  			char str1[21];
  			char str2[80];
  			 
	    	x1 = 118 ; 
   	    	y1 = 95 ;
   	    	strcpy(str2, "Pass Word : ") ;
            cprstr( x1,y1 , "%s", str2 ); 
            tmpstr[0] = 0;
            TextEdit( x1 + strlen(str2) * 8 , y1 , 8, 7, 0, tmpstr);   // 編輯文字
           	
			 
		     if (strncmp(tmpstr, "SUNKIST", 6) == 0)   // 密碼正確
		     {
			 	if (FileFlag == 1)  // 檔案存在
			    	fout = fopen("SKDATA02.DAT","r+b");
			 	else
			    	fout = fopen("SKDATA02.DAT","w+b");
			 	fwrite(&Data, sizeof(DATA), 1, fout);	  // 寫入資料
			 	fclose(fout);
			 	
			 	setcurso(1);
			 	//normvideo();
			 	clrscr();
			 	return(0);
		     }
		     else
		     {
			   	cprstr( 118 ,95 , "Pass Word Error! .............." );  // gotoxy(15,23);  printf("Pass Word Error!....... ");
			   	delay(3000);
			   	showscr();	   // 顯示 設定畫面
  				for (i=0; i<item; i++)   bartext(i, pset[i]);
  				nitem = 0;	   // 預設選項號碼
  				setcolor(RED);
				cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
  				bartext( nitem, pset[nitem] );
  				setcolor(WHITE);
  				cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
  				
		     }
		     
		     break;
		     
		    case  72 :			      // 上移鍵
		      //normvideo();
		      bartext(nitem, pset[nitem]);
		      if (nitem > 0)   nitem--;
		      else  nitem = item -1;	      // 循環至最後一項
		      
			  //textattr(BLACK+(LIGHTGRAY<<4));
  			  setcolor(RED);
			  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			  cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
  			  bartext( nitem, pset[nitem] );
  			  setcolor(WHITE);
			  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			  cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
		      // bartext(nitem, pset[nitem]);
		      
		      break;
		    
		 case  80 :			      // 下移鍵
		    
		    //normvideo();
		    bartext(nitem, pset[nitem]);
		    if (nitem < item-1)   nitem++;
		    else  nitem = 0;		      // 循環至最前一項
		    
			//textattr(BLACK+(LIGHTGRAY<<4));
		    setcolor(RED);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
		    bartext(nitem, pset[nitem]);
		    setcolor(WHITE);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
		    
		    break;
		    
		 case  73 :			      // Pg Up鍵
		    if (pset[nitem] > nmin[nitem])  pset[nitem]--;
		    else   pset[nitem] = nmax[nitem]; // 循環成最大值
		    
		    // printf("nitem=%d pset[nitem]=%d  \n " ,nitem ,pset[nitem] );
	        setcolor(RED);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
		    bartext( nitem ,pset[nitem] );
		    setcolor(WHITE);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
		    
		    
		    break;
		    
		 case  81 :			      // Pg Dn鍵
		    if (pset[nitem] < nmax[nitem])   pset[nitem]++;
		    else   pset[nitem] = nmin[nitem]; // 循環成最小值
		    
		    // printf("nitem=%d pset[nitem]=%d  \n " ,nitem ,pset[nitem] );
		    setcolor(RED);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
		    bartext( nitem ,pset[nitem] );
		    setcolor(WHITE);
		    cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  			cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
		    
		    break;
		    
		 case  71 :			      // Home 鍵
		    if (nitem != 0)
		      { 
			  	// normvideo();
				bartext(nitem, pset[nitem]);
				nitem = 0;
				//textattr(BLACK+(LIGHTGRAY<<4));
  				setcolor(RED);
		    	cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
  				bartext( nitem, pset[nitem] );
  				setcolor(WHITE);
		    	cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
				//bartext(nitem, pset[nitem]);
				
				
		      }
		    break;
		    
		 case  79 :			      // End  鍵
		    if (nitem != item-1)
		      { // normvideo();
			    bartext(nitem, pset[nitem]);
			    nitem = item -1;
			    //textattr(BLACK+(LIGHTGRAY<<4));
  				setcolor(RED);
		    	cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 2 ;  // 文字背景顏色	 2 綠  0 黑    
  				bartext( nitem, pset[nitem] );
  				setcolor(WHITE);
		    	cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  				cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
			    //bartext(nitem, pset[nitem]);
		      }
		    break;
	    }
	    break;
	  default :
	    break;
	}
  }
  setcurso(1);
  
  
  
  close(cfont.Handl);	      // 關閉字型檔案
  closegraph();
  
  return(0);
}

int bartext(int nitem, int num)
 {
   
   // gotoxy( pos[nitem][0] ,pos[nitem][1] );       // 將游標移至目前的選項位置
   // cprstr( 0,0*y+z ,"  SUNKIST  CAD/CAM  SETUP  MENU       Write by W. M. Chen 2000-2007 ");
   
   switch (nitem)
     { case 0 :
	    Data.CHINA = num;
	    switch (num)		      // 語言
	      { 
		    case 0 :  
				//outtextxy( 118 ,55 , "ENGLISH" );  
				cprstr( 118 ,55 , "ENGLISH" );  
				break;
		    case 1 :  
				//outtextxy( 118 ,55 , "CHINESE" );  
				cprstr( 118 ,55 , "CHINESE" );  
				break;
	      }
	    break;
       case 1 :
	    Data.MACHINE_type = num;
	    switch (num)		      // 機種
	      { 	
		  case 1 :  
		  		//outtextxy( 118 ,67 , "SA-14A    " );  // printf("SA-14A    ");
		  		cprstr( 118 ,67 , "SA-14A     " );  
			  	break;
		  case 2 :  
		  		cprstr( 118 ,67 , "SA-14AK    " );  // printf("SA-14AK   ");
			  	break;
		  case 3 :  
		  		cprstr( 118 ,67 , "SA-14AVK   " );  // printf("SA-14AVK  ");
			  	break;
		  case 4 :  
		  		cprstr( 118 ,67 , "SA-14AVHK  " );  // printf("SA-14AVHK ");
			  	break;
		  case 5 :  
		  		cprstr( 118 ,67 , "SA-14ABK   " );  // printf("SA-14ABK  ");
			  	break;
		  case 6 :  
		  		cprstr( 118 ,67 , "SA-14AHK   " );  // printf("SA-14AHK  ");
			  	break;	  
	      }
	    break;
       case 2 :
	    Data.UnitMode = num;
	    switch (num)		      // 單位
	      { 	case 1 : 
		  			cprstr( 118 ,84 , "mm      " ); // printf("mm     ");
			  		break;
				case 2 :  
					cprstr( 118 ,84 , "inch    " ); // printf("inch   ");
			  		break;
	      }
	    break;
       case 3 : 			      // 數位板 的 COM 號碼
	    Data.DIGIT_COM = num;
	    switch (num)
	      { case 1 :
		     cprstr( 78 ,165  , "COM 1 " ); // printf("COM 1 ");
		     Data.DIGIT_COM_IO = 0x3F8;      // 數位板的IO起始位置
		     //gotoxy(19, 14);
		     cprstr( 168 ,211 , "0x3F8 " ); // printf("0x3F8 ");
		     break;
		case 2 :
		     cprstr( 78 ,165  , "COM 2 " ); // printf("COM 2 ");
		     Data.DIGIT_COM_IO = 0x2F8;
		     //gotoxy(19, 14);
		     cprstr( 168 ,211 , "0x2F8 " ); //printf("0x2F8 ");
		     break;
		case 3 :
		     cprstr( 78 ,165  , "COM 3 " ); // printf("COM 3 ");
		     Data.DIGIT_COM_IO = 0x3E8;
		     //gotoxy(19, 14);
		     cprstr( 168 ,211 , "0x3E8 " ); //printf("0x3E8 ");
		     break;
		case 4 :
		     cprstr( 78 ,165  , "COM 4 " ); // printf("COM 4 ");
		     Data.DIGIT_COM_IO = 0x2E8;
		     //gotoxy(19, 14);
		     cprstr( 168 ,211 , "0x2E8 " ); //printf("0x2E8 ");
		     break;
	      }
	    break;
       case 4 : 			      // 數位板 的 型式
	    Data.DIGIT_NUM = num;
	    switch (num)
	      { case 0 :
		     cprstr( 78 ,182 , "None                    " ); //printf("None                        ");
		     Data.DIGIT_baud = 9600;	 // 傳輸速率
		     cprstr( 168 ,228 , "           " ); //gotoxy(19, 15);   printf("        ");
		     Data.DIGIT_parity = 1;	 // 同位元 0=None 1=Odd 2=Even
		     cprstr( 168 ,245 , "           " ); //gotoxy(19, 16);   printf("   ");
		     Data.DIGIT_data = 7;	 // 資料長度
		     cprstr( 168 ,262 , "           " ); //gotoxy(19, 17);   printf("   ");
		     Data.DIGIT_stop = 1;	 // 停止位元數
		     cprstr( 168 ,276 , "           " ); //gotoxy(19, 18);   printf("   ");
		     Data.DIGIT_scale = 1;	 // 數位板輸出尺寸/實際尺寸
		     cprstr( 168 ,291 , "           " ); //gotoxy(19, 19);   printf("        ");
		     Data.DIGIT_format = 0;	 // 數位板的輸出格式 1=Binary5 2=Binary8 3=UIOFmm 4=ASCIImm
		     cprstr( 168 ,307 , "           " ); //gotoxy(19, 20);   printf("        ");
		     Data.DIGIT_type = 0;	 // 數位板的輸出方式  0= 無  1=RUN 2=POINT 3=TRACK 4=LINE
		     cprstr( 168 ,324 , "           " ); //gotoxy(19, 21);   printf("        ");
		     Data.DIGIT_retry = 20;	 // 重讀次數
		     break;
		case 1 :
		     cprstr( 78 ,182 , "Summagrid IV--UIOF <mm> " ); //printf("Summagrid IV -- UIOF <mm>   ");
		     Data.DIGIT_baud = 9600;		 // 傳輸速率
		     cprstr( 168 ,228 , "9600       " ); //gotoxy(19, 15);   printf("9600    ");
		     Data.DIGIT_parity = 2;		 // 同位元 0=None 1=Odd 2=Even
		     cprstr( 168 ,245 , "2          " ); //gotoxy(19, 16);   printf("2  ");
		     Data.DIGIT_data = 7;		 // 資料長度
		     cprstr( 168 ,262 , "7          " ); //gotoxy(19, 17);   printf("7  ");
		     Data.DIGIT_stop = 1;		 // 停止位元數
		     cprstr( 168 ,276 , "1          " ); //gotoxy(19, 18);   printf("1  ");
		     Data.DIGIT_scale = 10;		 // 輸出尺寸/實際尺寸
		     cprstr( 168 ,291 , "10   lpm   " ); //gotoxy(19, 19);   printf("10   lpm");
		     Data.DIGIT_format = 3;		 // 輸出格式  3=UIOFmm
		     cprstr( 168 ,307 , "UIOF mm    " ); //gotoxy(19, 20);   printf("UIOF mm ");
		     Data.DIGIT_type = 2;		 // 輸出方式  2=POINT
		     cprstr( 168 ,324 , "POINT      " ); //gotoxy(19, 21);   printf("POINT   ");
		     Data.DIGIT_retry = 20;		 // 重讀次數
		     break;
		case 2 :
		     cprstr( 78 ,182 , "Calcomp -- ASCII <mm>   " ); //printf("Calcomp     -- ASCII <mm>   ");
		     Data.DIGIT_baud = 9600;		 // 傳輸速率
		     cprstr( 168 ,228 , "9600       " ); //gotoxy(19, 15);   printf("9600    ");
		     Data.DIGIT_parity = 2;		 // 同位元 0=None 1=Odd 2=Even
		     cprstr( 168 ,245 , "2          " ); //gotoxy(19, 16);   printf("2  ");
		     Data.DIGIT_data = 7;		 // 資料長度
		     cprstr( 168 ,262 , "7          " ); //gotoxy(19, 17);   printf("7  ");
		     Data.DIGIT_stop = 1;		 // 停止位元數
		     cprstr( 168 ,276 , "1          " ); //gotoxy(19, 18);   printf("1  ");
		     Data.DIGIT_scale = 200 / 25.4;	 // 輸出尺寸/實際尺寸
		     cprstr( 168 ,291 , "200  lpi   " ); //gotoxy(19, 19);   printf("200  lpi");
		     Data.DIGIT_format = 4;		 // 輸出格式  4=ASCIImm
		     cprstr( 168 ,307 , "ASCII mm   " ); //gotoxy(19, 20);   printf("ASCII mm");
		     Data.DIGIT_type = 2;		 // 輸出方式  2=POINT
		     cprstr( 168 ,324 , "POINT      " ); //gotoxy(19, 21);   printf("POINT   ");
		     Data.DIGIT_retry = 20;		 // 重讀次數
		     break;
		case 3 :
		     cprstr( 78 ,182 , "Wintime--Binary<5 bytes>" ); //printf("Wintime -- Binary <5 bytes> ");
		     Data.DIGIT_baud = 9600;		 // 傳輸速率
		     cprstr( 168 ,228 , "9600       " ); //gotoxy(19, 15);   printf("9600    ");
		     Data.DIGIT_parity = 0;		 // 同位元 0=None 1=Odd 2=Even
		     cprstr( 168 ,245 , "0          " ); //gotoxy(19, 16);   printf("0  ");
		     Data.DIGIT_data = 8;		 // 資料長度
		     cprstr( 168 ,262 , "8          " ); //gotoxy(19, 17);   printf("8  ");
		     Data.DIGIT_stop = 1;		 // 停止位元數
		     cprstr( 168 ,276 , "1          " ); //gotoxy(19, 18);   printf("1  ");
		     Data.DIGIT_scale = 500 / 25.4;	 // 輸出尺寸/實際尺寸
		     cprstr( 168 ,291 , "500  lpi   " ); //gotoxy(19, 19);   printf("500  lpi");
		     Data.DIGIT_format = 1;		 // 輸出格式  1=Binary 5
		     cprstr( 168 ,307 , "Binary 5   " ); //gotoxy(19, 20);   printf("Binary 5");
		     Data.DIGIT_type = 1;		 // 輸出方式  1=RUN
		     cprstr( 168 ,324 , "RUN        " ); //gotoxy(19, 21);   printf("RUN     ");
		     Data.DIGIT_retry = 20;		 // 重讀次數
		     break;
	      }
	    break;
       case 5 : 			      // PMAC	的 COM 號碼
	    Data.PMAC_COM = num;
	    switch (num)
	      { case 1 :
		     cprstr( 401 ,55 , "COM 1 " ); // printf("COM 1 ");
		     Data.PMAC_COM_IO = 0x3F8;
		     cprstr( 435 ,99 , "0x3F8 " ); // gotoxy(56, 7);   printf("0x3F8 ");
		     break;
		case 2 :
		     cprstr( 401 ,55 , "COM 2 " ); // printf("COM 2 ");
		     Data.PMAC_COM_IO = 0x2F8;
		     cprstr( 435 ,99 , "0x2F8 " ); // gotoxy(56, 7);   printf("0x2F8 ");
		     break;
		case 3 :
		     cprstr( 401 ,55 , "COM 3 " ); // printf("COM 3 ");
		     Data.PMAC_COM_IO = 0x3E8;
		     cprstr( 435 ,99 , "0x3E8 " ); // gotoxy(56, 7);   printf("0x3E8 ");
		     break;
		case 4 :
		     cprstr( 401 ,55 , "COM 4 " ); // printf("COM 4 ");
		     Data.PMAC_COM_IO = 0x2E8;
		     cprstr( 435 ,99 , "0x2E8 " ); // gotoxy(56, 7);   printf("0x2E8 ");
		     break;
	      }
	    break;
       case 6 :      // 控制器 的 型號
	    Data.PMAC_type = num;
	    switch (num)
	      { case 0 :   // PMAC
		     cprstr( 401 ,67 , "PMAC   " ); // printf("PMAC  ");
		     break;
		case 1 :   // UTC
		     cprstr( 401 ,67 , "UTC    " ); // printf("UTC   ");
		     break;
		case 2 :
		     cprstr( 401 ,67 , "SMCC   " ); // printf("SMCC  ");
		     break;
	      }
	    break;
     }
   return(0);
 }

 
 
 
 
 

int setcurso(int mode)		     // 將游標隱藏或顯現
{
  //union REGS reg;
  //reg.h.ah = 0x01;
  //if (mode == 0)  reg.x.cx = 0xffff;
  //else	   reg.x.cx = 0x0b0c;
  //int86(0x10,&reg,&reg);
  return(0);
}

int DATA_default()   // 取得預設值
{
  Data.CHINA = 0;		  // =1 為中文畫面  =0 為英文畫面
  Data.DIGIT_NUM = 2;		  // 數位板的編號  =0 無  =2 CALCOMP
  Data.DIGIT_type = 2;		  // 數位板的輸出方式  0= 無  1=RUN 2=POINT 3=TRACK 4=LINE
  Data.DIGIT_format = 4;	  // 數位板的輸出格式 1=Binary5 2=Binary8 3=UIOFmm 4=ASCIImm
  Data.DIGIT_COM = 1;		  // 數位板的COM 號碼
  Data.DIGIT_COM_IO = 0x3F8;	  // 數位板的IO起始位置
  Data.DIGIT_scale = 200 / 25.4;  // 數位板輸出尺寸/實際尺寸
  Data.DIGIT_baud = 9600;	  // 傳輸速率
  Data.DIGIT_parity = 2;	  // 同位元 None, Odd, Even
  Data.DIGIT_data = 7;		  // 資料長度
  Data.DIGIT_stop = 1;		  // 停止位元數
  Data.DIGIT_retry = 20;	  // 重讀次數
  Data.PMAC_type = 0;		  // =0 PMAC  =1 UTC  =2 SMCC
  Data.PMAC_COM = 2;		  // PMAC的COM 號碼
  Data.PMAC_COM_IO = 0x2F8;	  // PMAC的IO起始位置
  Data.PMAC_baud = 38400;	  // 傳輸速率
  Data.PMAC_parity = 0; 	  // 同位元 None, Odd, Even
  Data.PMAC_data = 8;		  // 資料長度
  Data.PMAC_stop = 1;		  // 停止位元數
  Data.PMAC_retry = 20; 	  // 重讀次數

  Data.MACHINE_type = 2;	  // 機器型號 0=保留  1=14A(線刀)  2=14AK
				  //	      3=14AVK  4=14AVHK    5=14ABK
  Data.BladeWide = 4.5; 	  // 帶刀寬度
  Data.WireDia	 = 0;		  // 線刀直徑
  Data.AxisUse	= 3;		  // 使用的軸數
  Data.CutSpeed =  4000;	  // 預設的切割速度
  Data.RotSpeed = 12000;	  // 預設的轉刀速度
  Data.CutType	= 2;		  // 0=WIRE 1=BLADE(TIP) 2=BLADE(MID)
				  // 4=直橫綜合 99=線,帶刀通用
  strcpy(Data.Cut_LU, "P1000");  		 // 刀左上 的 指令   
  strcpy(Data.Cut_U,  "P1001");   		// 刀  上 的 指令   
  strcpy(Data.Cut_RU, "P1002");   	// 刀右上 的 指令   
  strcpy(Data.Cut_L,  "P1003");   		// 刀左   的 指令   
  strcpy(Data.Cut_R,  "P1004");   		// 刀右   的 指令   
  strcpy(Data.Cut_LD, "P1005");   		// 刀左下 的 指令   
  strcpy(Data.Cut_D,  "P1006");   		// 刀  下 的 指令   
  strcpy(Data.Cut_RD, "P1007");   	// 刀右下 的 指令   
  Data.UnitMode  = 1;
  Data.Reserve02 = 0;
  Data.Reserve03 = 0;
  Data.Reserve04 = 0;

  return(1);
}

void clrscr()
{
    int i, j ;
    /*
    for( i=0 ; i<=10; i++) {
	  gotoxy(0, i);
	  printf("                            " );
    }
    gotoxy(0, 0);
    */
}

void gotoxy(int x, int y)
{
	/*
    COORD pots;
    pots.X = x;
    pots.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pots);
    */
}

int showscr()	   // 顯示 設定畫面
{
  cleardevice();
  setcolor(WHITE);
  //settextstyle(SANS_SERIF_FONT ,HORIZ_DIR ,1 );
  //settextstyle( DEFAULT_FONT ,HORIZ_DIR , 2 );
  
  //outtextxy(195,451,"Smiling Face Animation");
  //clrscr();
  
  int y = 16 ;
  int z = 6 ;
  
  
  cfont.Color   = 15 ;   // 文字顏色  15 亮白  4 紅 
  cfont.BColor  = 0 ;  // 文字背景顏色	 2 綠  0 黑    
  
  cprstr( 0,0*y+z ,"  SUNKIST  CAD/CAM  SETUP  MENU                             2000-2018 ");
  cprstr( 0,1*y+z ,"                                                                      ");
  cprstr( 0,2*y+z ,"============ << Common >> =================== << Controller >> =======");
  cprstr(0,3*y+z ,"| Language :                     | Port   :                          |");
  cprstr(0,4*y+z ,"| Machine  :                     | Type   :                          |");
  cprstr(0,5*y+z ,"| Unit     :                     |                                   |");
  cprstr(0,6*y+z ,"|                                | Adress     :                      |");
  cprstr(0,7*y+z ,"|                                | Baud Rate  : 38400                |");
  cprstr(0,8*y+z ,"|                                | Parity     : 0                    |");
  cprstr(0,9*y+z ,"|===========<< Digitizer >> =====| Data Bits  : 8                    |");
  cprstr(0,10*y+z,"| Port :                         | Stop Bits  : 1                    |");
  cprstr(0,11*y+z,"| Type :                         |                                   |");
  cprstr(0,12*y+z,"|                                |===================================|");
  cprstr(0,13*y+z,"| Adress        :                |                                   |");
  cprstr(0,14*y+z,"| Baud Rate     :                | <UP> <DOWN>    : MOVE             |");
  cprstr(0,15*y+z,"| Parity        :                | <Pg-Up> <Pg-Dn>: CHANGE           |");
  cprstr(0,16*y+z,"| Data Bits     :                |                                   |");
  cprstr(0,17*y+z,"| Stop Bits     :                |                                   |");
  cprstr(0,18*y+z,"| Resolution    :                | <ESC>  : Quit                     |");
  cprstr(0,19*y+z,"| Report Format :                | <F02>  : Save and Quit            |");
  cprstr(0,20*y+z,"| Operatig Mode :                |                                   |");
  cprstr(0,21*y+z,"======================================================================");
  
  
  /*
  outtextxy( 0,0*y+z ,"  SUNKIST  CAD/CAM  SETUP  MENU       Write by W. M. Chen 2000-2007 ");
  outtextxy( 0,1*y+z ,"                                                                      ");
  outtextxy( 0,2*y+z ,"============ << Common >> =================== << Controller >> =======");
  outtextxy(0,3*y+z ,"| Language :                     | Port   :                          |");
  outtextxy(0,4*y+z ,"| Machine  :                     | Type   :                          |");
  outtextxy(0,5*y+z ,"| Unit     :                     |                                   |");
  outtextxy(0,6*y+z ,"|                                | Adress     :                      |");
  outtextxy(0,7*y+z ,"|                                | Baud Rate  : 38400                |");
  outtextxy(0,8*y+z ,"|                                | Parity     : 0                    |");
  outtextxy(0,9*y+z ,"|===========<< Digitizer >> =====| Data Bits  : 8                    |");
  outtextxy(0,10*y+z,"| Port :                         | Stop Bits  : 1                    |");
  outtextxy(0,11*y+z,"| Type :                         |                                   |");
  outtextxy(0,12*y+z,"|                                |===================================|");
  outtextxy(0,13*y+z,"| Adress        :                |                                   |");
  outtextxy(0,14*y+z,"| Baud Rate     :                | <UP> <DOWN>    : MOVE             |");
  outtextxy(0,15*y+z,"| Parity        :                | <Pg-Up> <Pg-Dn>: CHANGE           |");
  outtextxy(0,16*y+z,"| Data Bits     :                |                                   |");
  outtextxy(0,17*y+z,"| Stop Bits     :                |                                   |");
  outtextxy(0,18*y+z,"| Resolution    :                | <ESC>  : Quit                     |");
  outtextxy(0,19*y+z,"| Report Format :                | <F02>  : Save and Quit            |");
  outtextxy(0,20*y+z,"| Operatig Mode :                |                                   |");
  outtextxy(0,21*y+z,"======================================================================");
  */
  
  
  return(0);
}

void inputxy(int x, int y, char *str1 )
{
    char string1[20];
    char i1;
  	int i2 = 0 ;
  	
	outtextxy( x , y , "                     " );
    for(int i = 0; i < 19; i++) string1[i] = '\0' ; // clear
  	do {
  		i1=getch();
  		//putchar(i1);
  		string1[i2] = i1;
  		outtextxy( x , y , string1);
  		i2++;
  	} while (i1 != 13 && i2<=19 );
   	outtextxy( x , y , string1);
   	for(int i = 0; i < 19; i++) {
	   *(str1+i) = string1[i] ;
    }
}


