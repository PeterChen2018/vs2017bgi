
#define ARC_div        72	// 畫弧時, 以幾條直線繪出。 (=36 表一圓以36條直線繪出, 每隔10度)
				// ARC_div= 720 有問題 ??????
#define UnitAngDif     0.00001

typedef unsigned char Byte;
typedef unsigned int  Word;


typedef struct
  {
    int   G;	   // =1 G1(直線) =2 G2(順時針弧) G3 G4 G22 G23 G96 G97 G98 G99
    float X;	   // X 方向的相對座標值   (G4暫停的時間值(秒)正值
    float Y;	   // Y 方向的相對座標值
    float I;	   // 弧心距起點的 X 相對位置 ( CNC 檔使用 )
    float J;	   // 弧心距起點的 Y 相對位置 ( CNC 檔使用 )
    float Z;	   // G2 G3 弧的半徑
    long  F;	   // 速度  mm/minute  須為正值 =0與前面相同
    int   B;	   // G22 G23 重覆的次數
    int   C;	   // G22 G23 G98 的副程式名稱
  } G_CODE;

typedef struct EntDT
  {
    struct EntDT  *Pptr;     // 上一個結構的指標位址
    struct EntDT  *Nptr;     // 下一個結構的指標位址
    G_CODE Gcode;
  } G_DATA;


typedef struct		   // 圖素資料的結構
      { char  type;	   // =1 LINE  =2 ARC
	char  status;	   // 現況 (保留)
	float x1;	   // 起始點 (X 值)
	float y1;	   // 起始點 (Y 值)
	float x2;	   // LINE 的終點, ARC 的中點
	float y2;	   //
	float x3;	   // ARC 的終點 (X 值)
	float y3;	   //
      } ENT_DATA;

typedef struct
      { int status;	   /* 現況 0正常 1有兩點重覆 2三點垂直 3 三點水平  */
	int dire;	   /* 弧的方向 =1 順時針			   */
	int half;	   /* =1 弧角度超過 180 度			   */
	float cenx;	   /* 弧心 X 座標				   */
	float ceny;	   /* 弧心 Y 座標				   */
	float radius1;	   /* 半徑					   */
	float ang1;	   /* 起始角度	(degree)			   */
	float ang2;	   /* 終止角度	(degree)			   */
      } ARC_DATA;	   /* 弧  資料的結構				   */

typedef struct
      { int num;	   /* 目前已輸入了幾點 (共三點) 	*/
	float x[3];
	float y[3];
      } ARC_P;		   /* 目前輸入的狀況			*/

typedef struct
      {
	int x;		   /* 滑鼠目前座標			*/
	int y;
	int xp; 	   /* 滑鼠上一次座標			*/
	int yp;
	int shape;	   /* 目前滑鼠游標的型式, 9999 為未定義 */
	int Mx1;	   /* 滑鼠移動範圍的最小水平座標	*/
	int My1;	   /* 滑鼠移動範圍的最小垂直座標	*/
	int Mx2;	   /* 滑鼠移動範圍的最大水平座標	*/
	int My2;	   /* 滑鼠移動範圍的最大垂直座標	*/
      } MOUSE;		   /* 目前滑鼠的狀況			*/

typedef struct
      {
	int  Handl;		/* 字型檔案代表碼		*/
	int  Wmode;		/* cprstr 印字時之方式, 有 COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT 五種方式  */
	int  Color;		/* 文字顏色  0黑  1藍  2綠  3靛  4紅  5洋紅  6棕  7白	*/
				/* (cprstr 印字時之顏色)		    14黃 15亮白 */
				/* 四個色面 HRGB (亮紅綠藍) color=9(1001)為亮藍, 餘類推 */
	int  BColor;		/* 文字背景顏色 		*/
	int  Cx;		/* 橫向量			*/
	int  Cy;		/* 縱向量			*/
	int  Cw;		/* 字寬 1=8點 2=16點		*/
      } CFONT;			/* 中文字型的開啟情形		*/

typedef struct
      {
	int  Maxx;		// 螢幕最大的水平座標
	int  Maxy;		// 螢幕最大的垂直座標
	// int	Sx1;		// 螢幕繪圖區的最小水平座標
	// int	Sy1;		// 螢幕繪圖區的最小垂直座標
	// int	Sx2;		// 螢幕繪圖區的最大水平座標
	// int	Sy2;		// 螢幕繪圖區的最大垂直座標
	int   ProgSx[2];	// PROG 1/2 繪圖區的最小水平座標
	int   ProgSy[2];	// PROG 1/2 繪圖區的最小垂直座標
	int   ProgMx[2];	// PROG 1/2 繪圖區的最大水平座標
	int   ProgMy[2];	// PROG 1/2 繪圖區的最大垂直座標
	int  Px1;		// 螢幕POP 問答區的最小水平座標
	int  Py1;		// 螢幕POP 問答區的最小垂直座標
	int  Px2;		// 螢幕POP 問答區的最大水平座標
	int  Py2;		// 螢幕POP 問答區的最大垂直座標
	float Dx1[2];		// 工作圖面的最小水平座標
	float Dy1[2];		// 工作圖面的最小垂直座標
	float Dx2[2];		// 工作圖面的最大水平座標
	float Dy2[2];		// 工作圖面的最大垂直座標
	float Dscal[2]; 	// 螢幕繪圖區對工作圖面的比例
	float XDscal[2];	// 螢幕繪圖區對工作圖面的X軸比例
	float YDscal[2];	// 螢幕繪圖區對工作圖面的Y軸比例
	float XDirect[2];	// X 軸的方向 1=正常  -1=反向
	float YDirect[2];	// Y 軸的方向 1=正常  -1=反向
	float CNCx1[2]; 	// 載入的CNC檔的最小水平座標
	float CNCy1[2]; 	// 載入的CNC檔的最小垂直座標
	float CNCx2[2]; 	// 載入的CNC檔的最大水平座標
	float CNCy2[2]; 	// 載入的CNC檔的最大垂直座標
	float ZOOMmin[2];	// 工作圖面的最小範圍
	int   UnitMode; 	// 1=MiniMeter	  2=inch
	float UnitScale;	// 1.0=minimeter  25.4=inch  96.07
	// float GRID;		// 格點大小 (須為正值 0= 取消格點功能)
	int   MENU[2];		// 定義目前MENU的位置
	char  CHINA;		// =1 為中文畫面  =0 為英文畫面
	long  SPEED_Cut;	// 直線切割速度 (刀片,線刀轉CNC用)
	long  SPEED_Cut_now;	// 目前的直線切割速度 (刀片,線刀轉CNC用)
	long  SPEED_Cut2;	// 弧切割速度 (刀片,線刀轉CNC用)
	long  SPEED_Rot;	// 迴刀速度 (刀片轉CNC用)
	long  SPEED_Rot_now;	// 92.01.03
	long  SPEED_Max;	// 最大的切割速度   <變數名稱 超過十字以上者,無效!>
	// float HOLD_Ang;	// 最大不暫停角 (線刀轉CNC用)	*/
	// int	 Cur_Mode;	/* 目前所編輯的模式  =0 無  =1 PART  =2 PROG */
	float CNCang[100];	/*     副程式最終角度	*/
	float CNCoffX[100];	/*     起點至終點距離	*/
	float CNCoffY[100];
	char  OPENPATH[130];	// 開檔   預設路徑
	char  SP2_Path[130];	// SP2 檔案的預設目錄
	char  CNC_Path[130];	// CNC 檔案的預設目錄
	char  SP2File[2][180];	// SP2 檔名
	char  LoadFile[2][180]; // Down Load 的檔名 (*.CNC)
	long  DownloadTm;	// 以時間(秒)為值, 比對 PMAC 的 Q12 參數, 相同表示己下載
	int   PMAC_TY01;	// =0 PMAC  =1 UTC  =2 SMCC
	int   PMAC_TY02;	// 保留  (如果是PMAC,則0=Mini,1=4軸,2=8軸 (暫定))
	int   PMAC_Act; 	/* PMAC 是否在線上			  */
	int   PMAC_COM; 	/* PMAC的COM 號碼			  */
	int   PMAC_COM_IO;	/* PMAC的IO起始位置			  */
	long  PMAC_baud;	/* 傳輸速率				  */
	int   PMAC_parity;	/* 同位元 None, Odd, Even		  */
	int   PMAC_data;	/* 資料長度				  */
	int   PMAC_stop;	/* 停止位元數				  */
	int   PMAC_retry;	/* 重讀次數				  */
	float PMAC_FeedRate;	/* PMAC  feedrate			  */
	int   PMAC_err; 	// sendchar 時, 累積的錯誤次數
	int   Cline;		/* 線條顏色	       15  白		  */
	int   Cback;		/* 背景顏色		0  黑		  */
	int   Cmark;		/* 記號顏色	       12  淡紅 	  */
	int   Cedit;		/* 編輯顏色	       10  淡綠 	  */
	int   Cblock;		/* 副程式顏色  G23     11  淡青 	  */
	int   CwordPre; 	/* 文字顏色	       15  白		  */
	int   CwordBak; 	/* 文字背景顏色 	2  綠		  */
	int   Cbar01;		/* 光棒顏色  MENU      15  白		  */
	int   Cbar02;		/* 光棒顏色  POP       15  白		  */
	int   Cframe;		/* 分隔線顏色	       10  淡綠 	  */
	int   CpopFrame;	// POP 區的框線顏色
	int   CpopPre;		// POP 區文字顏色
	int   CpopBak;		// POP 區文字底色
	int   Cemu;		// 模擬切割路徑的顏色
	float BladeAng[2];	// 機器上刀的角度, 為笛卡兒座標的 ??度
	float Blade_Max_Ang[2]; // 轉刀的最大角度 ( 275度)
	float Blade_Min_Ang[2]; // 轉刀的最小角度 (-275度)
	float Blade_Hold[2];	// 最大不暫停角
	float MaxZ[2];		// 所讀取的 CNC 檔中, 刀的最大角度
	float MinZ[2];		// 所讀取的 CNC 檔中, 刀的最小角度
	float TmpZ[2];
	int   Zflag[2]; 	// 所讀取的 CNC 檔中, 是否有 Z
	int   Emu;		// 電腦模擬切割路徑 =1 模擬  =0 不模擬
	int   EmuEsc;		// =1 取消模擬 (處理 副程式 不停)
	int   EmuStep;		// =0 手動單步模擬  =1 自動延遲模擬
	int   EmuDisplayAng;	// =1 模擬時在路徑上顯示刀的方向
	int   EmuDisplaySpeed;	// 模擬時的顯示速度 (延遲係數0-99)  0=速度最快
	int   EmuDisplayType;	// =0 模擬時點顯示  =1 模擬時線顯示
	float EmuLength;	// 切割路徑的長度
	float EmuX1[2]; 	// 模擬的CNC檔的最小水平座標
	float EmuY1[2]; 	// 模擬的CNC檔的最小垂直座標
	float EmuX2[2]; 	// 模擬的CNC檔的最大水平座標
	float EmuY2[2]; 	// 模擬的CNC檔的最大垂直座標
	int   AxisUse;		// 使用的軸數 , 14A(線刀)為兩軸  14AK(轉刀)為三軸
	float StartAng[2];	// CNC 刀的起始角度 94.06.14
	int   RetStPt[2];	// CNC 切完是否回起始點 (1=要)	94.06.14
	float BladeWid[2];	// 刀的寬度	 [0]代表直刀機 [1]代表橫刀機
	float BladeTip[2];	// 刀尖距旋轉中心的距離
	int   BladeType[2];	// 0=線刀(不旋轉) 1=刀尖旋轉(刀寬=0) 2=刀中心旋轉
	int   P_RUN;		// =1 目前正在 RUN 程式
	int   P_POS;		// =1 目前馬達正在指定位置上 (RUN程式或手動控制行走, =0)
	char  Cut_LU[6];	// 刀左上 (指令) P1007
	char  Cut_U[6]; 	// 刀  上 (指令) P1006
	char  Cut_RU[6];	// 刀右上 (指令) P1005
	char  Cut_L[6]; 	// 刀左   (指令) P1004
	char  Cut_R[6]; 	// 刀右   (指令) P1003
	char  Cut_LD[6];	// 刀左下 (指令) P1002
	char  Cut_D[6]; 	// 刀  下 (指令) P1001
	char  Cut_RD[6];	// 刀右下 (指令) P1000
	char  CMD_Start01[50];	// 啟動  B1R
	char  CMD_Start02[50];	// 啟動  B2R
	char  CMD_StartH[50];	// 啟動  R
	char  CMD_Stop[50];	// 停止
	char  CMD_Hold[50];	// 暫停
	char  CMD_Zhome[50];	// 轉刀歸零
	char  CMD_PosReset[50]; // 原點重置
	char  CMD_XYhome[50];	// 14AVHK,14AVK=送料 , 14AK=移到右上角
	char  CMD_WkHome01[50]; // 回指定的原點
	char  CMD_WkHome02[50]; // 回指定的原點
	int   P100X_Delay;	// 送出 P100?=1 時, 延遲時間, 再送出 P100?=0
	float MaxDisplayLength; // 顯示PMAC路徑時, 長度超過此值即不顯示 (避免畫面雜亂)
	double PMAC_Mx91[8];	// 8 個馬達的 比例係數
	double PMAC_POS[8];	// 8 個馬達的位置
	double PMAC_PrePos[8];	// 前次刀的位置 (8軸)
	double VK2HK_Dist;	// 直刀機與橫刀機刀片間的距離
	long  PMAC_Ix08[8];	// Motor x Position Scale Factor
	long  PMAC_I10; 	// Servo Interrupt Time  預設值 3713707
	int   DispMode; 	// =0 直刀機  =1 橫刀機  圖面
	int   Icon[30][4];	// 各種小按鈕的位置
	int   CutIcon[12][4];	// 刀行進之按鈕位置
	int   FkeyIcon[10][4];	// F 功能鍵之按鈕位置
	int   Sub_Icon[30][4];	// 各副程式 通用的 ICON 位置 (隨不同的副程式, 數值改變)
	int   Sub_Disp[10][4];	// 各副程式 通用的 DISP 位置
	
	//char  IconBuf[7648];	// XOR_PUT   201807 Change  49 * 39 * 4 + 4 =  7648
	char  IconBuf[2000];	// XOR_PUT
	
	int   Disp[17][4];	// 各種顯示框的位置
	int   MOUSE_YN; 	// =1 有滑鼠  =0 無滑鼠
	int   Alarm;		// =1 出現訊息時, 會嗶
	int   TIMER;		// =1 計時器作用
	int   GetMode;		// =0 接收較穩,但時間變慢  =1 時間較正常
	int   PreCheck; 	// =1 按<F3>START , 會預先檢查是否已作 轉刀及XY歸零
	int   PassOK[11];	// 密碼是否已通過
	int   Machine_Type;	// 機器型號 0=保留  1=14A(線刀)  2=14AK  3=14AVK  4=14AVHK
	int   Dwell;            // 轉角暫停時間
	char  Nation[8][4];	// 各國文字
	char  MsgText[120][81]; // 顯示文字


      } SCREEN; 		/* 螢幕的現況			*/

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
	// 以下為 9.03.21 增加
	int   MACHINE_type;	  // 機器型號 0=通用  1=14A(線刀)  2=14AK  3=14AVK  4=14AVHK
	float BladeWide;	  // 帶刀寬度
	float WireDia;		  // 線刀直徑
	int   AxisUse;		  // 使用的軸數
	long  CutSpeed; 	  // 預設的切割速度
	long  RotSpeed; 	  // 預設的轉刀速度
	int   CutType;		  // 0=WIRE 1=BLADE(TIP) 2=BLADE(MID) 4=直橫綜合 99=線,帶刀通用
	char  Cut_LU[10];	  // 刀左上 的 指令   P1000	14AVK P1005
	char  Cut_U[10];	  // 刀  上 的 指令   P1001	      P1006
	char  Cut_RU[10];	  // 刀右上 的 指令   P1002	      P1007
	char  Cut_L[10];	  // 刀左   的 指令   P1003	      P1003
	char  Cut_R[10];	  // 刀右   的 指令   P1004	      P1004
	char  Cut_LD[10];	  // 刀左下 的 指令   P1005	      P1000
	char  Cut_D[10];	  // 刀  下 的 指令   P1006	      P1001
	char  Cut_RD[10];	  // 刀右下 的 指令   P1007	      P1002
	int   UnitMode; 	  // 單位 1=mm 2=in  96.07增加
	int   Reserve02;
	int   Reserve03;
	int   Reserve04;

      } COMMON_DATA;

long cdecl Get_PMAC_long(char *ADR);	// 取得 PMAC 位址ADR 的值(long)
int  cdecl Display_XYZ();		// 顯示 XYZ 軸的座標
long cdecl Get_PMAC_long(char *ADR);	       // 取得 PMAC 位址ADR 的值(long)
int  cdecl OnLine_Command(char *fpre, int X1, int Y1);	// 線上 輸入 指令
int  cdecl Get_PassWord(char *Title, int X1, int Y1);	// 輸入 通行碼
int  cdecl ZOOM_ALL();
