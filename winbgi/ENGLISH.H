//	CHINESE.H
//	定義有圖形模式下, 有關文字方面的功能


int bioskey( int value) ;  // Peter added



typedef struct
  {
    int X1;	    // 字串的左上角座標
    int Y1;	    // 字串的左上角座標
    int DispCol;    // 字串的顯示空間
    int MaxCol;     // 字串的最大長度
    char *Txt;	    // 字串所在的指標
  } TxtData;

typedef struct
  {
    int X1;	    // 字串的左上角座標
    int Y1;	    // 字串的左上角座標
    int ch;	    // 目前所選擇的項目
    int num;	    // 項目的數量
    int DispCol;    // 每一項目的字串顯示空間
    int DispMode;   // 項目為水平(0) 或垂直(1) 排列
    char *Txt;	    // 字串所在的指標
  } RadioData;	    // 單一旋鈕

typedef struct
  {
    int X1;	    // 字串的左上角座標
    int Y1;	    // 字串的左上角座標
    int *status;    // 目前的狀態 (1=被選取)   指標
    char *Txt;	    // 字串所在的指標
  } CheckData;	    // 檢查盒


int  cdecl Clear_Corner(int X1, int Y1, int X2, int Y2, int mode, int color1, int color2);  // 清除四方區域


//void cdecl putcharmap(int x, int y);		    // 設定字型資料並印出   
void cdecl putcharmap( unsigned char charbuf[] ,int x, int y) ;  // 設定字型資料並印出  


void cdecl english(int x, int y, unsigned char v);  /* 在座標 x, y 印出 ASCII 碼 (英、數字等) */
void cdecl chinese(int x, int y, unsigned char high, unsigned char low);  /* 在座標x,y印出中文字型(含特殊符號) */
void cdecl cprstr(int x, int y, char *form,...);   /* 在 (x,y) 的位置印出字串 */
void cdecl MsgLine(char *Str1);     // 在 訊息列上, 顯示文字   *str1 容量須超過 81 char
void cdecl Show_Msg(int x, int y, char *Fstr, char *Str1, int color1, int color2);  // 顯示文字
int  cdecl show_file(int x, int y, char *fname);    /* 顯示文件檔內的內容  */
int  cdecl trim_str(char *ptr); 		    /* 將字串前後的空白字元刪除     */
int  cdecl replace_str(char *strptr, char *oldstr, char *newstr);  /* 將字串內的某字串轉成新字串		   */
int  cdecl un_compress(char *s);			 /* 將含 tab  之字串解開    */
void cdecl beep();

int  cdecl GetDirNum(char *pathbuf, char *ext, int *Num);  /* Get Direct num */
int  cdecl GetFileNum(char *pathbuf, char *ext, int *Num);  /* Get File num */
int  cdecl GetDirList(char *pathbuf, char *ext, char *List);  /* Get Direct LIST */
int  cdecl GetFileList(char *pathbuf, char *ext, char *List);  /* Get File LIST */
int  cdecl SortList(char *FileList, int Num, int Length, int mode);  /* 排序檔案名稱 */
int  cdecl GetDisk(int Drv[]);	  /* 取得有意義的磁碟機編號 */
int  cdecl GetPathNum(char *pathbuf, char *ext, int Num[], int mode);  /* 取得目錄,檔案,磁碟機數量 */
int  cdecl GetPathList(char *pathbuf, char *ext, char *List, int Num[]);  /* 取得目錄,檔案,磁碟機內容 */
int  cdecl POP_msg(char *fpre, int mode, int X1, int Y1, int Wx, int Wy);	/* 在POP 區顯示字串, =0 按任意鍵  =1 按YN鍵消失   */
int  cdecl TextEdit(int X1, int Y1, int SpaceCol, int MaxCol, int Cur, char *str);   /* 編輯文字  */
		  // 編輯數個文字區, 單一旋鈕, 檢查盒
int  cdecl TxtEdit_Group(int Px1, int Py1, int Wx, int Wy, int ITnum, TxtData ITdata[],
			 int DTnum, TxtData DTdata[], int RDnum, RadioData RData[],
			 int CDnum, CheckData CData[]);
int  cdecl POP_GetFile(char *Title, char *FileName, char *fext, int X1, int Y1);   /* 在POP 區尋找檔名	*/

// 讀取 並 顯示 文字檔	 PgWide 顯示的頁寬  PgHight 顯示的頁高
// int	cdecl ReadTXT(char *Title, char *Fstr, char *Sect, int X1, int Y1, int PgWide, int PgHight);
void cdecl HelpTXT();	// 顯示 求助內容
