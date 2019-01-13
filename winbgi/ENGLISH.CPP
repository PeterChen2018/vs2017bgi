
#include "graphics.h"

#include <conio.h>
#include <dos.h>
//#include <dir.h>
#include <io.h>
//#include <mem.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reset.h"
#include "english.h"


extern CFONT cfont;


unsigned char colormap[][4] ={ { 0x0,0x0,0x0,0x0 } // 0    Peter added
    ,{ 0xff,0x0,0x0,0x0 } // 1 // B 藍色 G 綠色 R 紅色 Reserve 保留
    ,{ 0x0,0xa0,0x0,0x0 } //2 
    ,{ 0xe0,0xe0,0x0,0x0 } //3
    ,{ 0x0,0x0,0xc0,0x0 } //4
    ,{ 0xe0,0x0,0xe0,0x0 } //5
    ,{ 0x0,0x40,0x80,0x0 } //6
    ,{ 0xc0,0xc0,0xc0,0x0 } //7 
    ,{ 0x60,0x60,0x60,0x0 } //8
    ,{ 0xff,0x80,0x80,0x0 } //9
    ,{ 0x40,0xff,0x40,0x0 } //10
    ,{ 0xff,0xff,0xc0,0x0 } //11
    ,{ 0x40,0x40,0xff,0x0 } //12
    ,{ 0xff,0x80,0xff,0x0 } //13
    ,{ 0x0,0xff,0xff,0x0 } //14
    ,{ 0xff,0xff,0xff,0x0 } //15
    } ;


struct ffblk   // 0    Peter added
{
	char ff_reserved[21];  	//reserved by DOS
	char ff_attrib;              	//attribute found
	char ff_ftime;              	//file time
	char ff_fdate;              	//file date
	char ff_fsize;              	//file size
	char ff_name;             	//found file name
} ;


// T15 English  8 (bits) * 4 * 15 (Height) + 4 (Width Height) = 484 bytes
// T15 Chinese 分 High byte 和 low byte  8 (bits) * 4 * 15 (Height) + 4 (Width Height) 
struct { unsigned int w;	      // 圖型寬度 bit數 -1
	     unsigned int h;	      // 圖型高度 條數 -1
	     unsigned char map[30] ;  // English 15 bytes Chinese 30 bytes
	     unsigned char img[484] ; 
} font ;  // cprstr 印字時之字型資料  


/*
// 清除四方區域
int Clear_Corner(int X1, int Y1, int X2, int Y2, int mode, int color1, int color2)
{ 
  int Rect[10], MSflag, DAflag;
  MSflag = GMouseDisp(0);
  DAflag = SetDrawArea(0);		   // VGA 邊界
  
  Rect[0] = Rect[6] = Rect[8] = X1;	   // 四個角落的座標值
  Rect[1] = Rect[3] = Rect[9] = Y1;
  Rect[2] = Rect[4] = X2;
  Rect[5] = Rect[7] = Y2;
  
  setcolor(color1);			     // 邊框顏色
  setfillstyle ( mode, color2);  // mode=0 背景色 =1 填實
  fillpoly ( 5, Rect);
  
  //printf("color1 %d    mode %d color2 %d  Rect %d \n" ,color1 ,mode ,color2 ,Rect ) ;
  
  if (DAflag)  SetDrawArea(1);		   // 繪圖邊界
  if (MSflag)  GMouseDisp(1);
  return(0);
}
*/

// EGA, VGA, 字型資料應先作處理, 才可傳給 putimage() 函數
// 須處理四個色面 HRGB (亮紅綠藍) color=9(1001)為亮藍, 餘類推
//cfont.Wmode   Name		Value   	Description
//    			COPY_PUT   	0	Copy
//    			XOR_PUT		1	Exclusive or
//    			OR_PUT		2	Inclusive or
//    			AND_PUT		3	And
//    			NOT_PUT		4	Copy the inverse of the source

// void putcharmap(int x, int y)  // 設定字型資料並印出   

void putcharmap( unsigned char charbuf[] ,int x, int y)  // 設定字型資料並印出   
{
  int i, j, k, source, destin, color, bcolor;
  unsigned char map1[8], map2[8], map3[8];    // 暫存的資料緩衝區
  
  // charbuf[]
  
  for ( i = 0 ; i < 484 ; i++ ) font.img[i] = 0x0 ; // clean img
  
  color  = cfont.Color ;
  bcolor = cfont.BColor ;
  
  //printf("x = %d y = %d  cfont.Wmode = %d \n" ,x ,y ,cfont.Wmode ) ;
  //printf("color %d bcolor %d \n" ,color ,bcolor ) ;
  
  font.img[0] = font.w + 1 ;
  font.img[1] = 0x0 ;
  font.img[2] = font.h + 1 ;
  font.img[3] = 0x0 ;
  
  //printf("img.w %d img.h %d\n" ,font.img[0] ,font.img[2] );
  
    int b1 ,bpc = 15 ,unit_size = 32  ;
    for ( i = 0 ; i < bpc ; i++ ) {
		//printf("============= > i %d char %x \n" ,i ,charbuf[i] );
        for (j = 0; j < unit_size; j+=4 ) {
        	b1 = j / 4 ;
        	if( ( charbuf[i] & ( 1 << b1 ) ) != 0 ) {
        		//printf("1 %x %x %x %x\n" ,colormap[color][0] ,colormap[color][1] ,colormap[color][2],colormap[color][3] ) ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 4 ] = colormap[color][0] ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 3 ] = colormap[color][1] ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 2 ] = colormap[color][2] ;
			}
			else {
				//printf("0 \n" ) ;
				//printf("0 %x %x %x %x\n" ,colormap[bcolor][0] ,colormap[bcolor][1] ,colormap[bcolor][2],colormap[bcolor][3] ) ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 4 ] = colormap[bcolor][0] ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 3 ] = colormap[bcolor][1] ;
        		font.img[ 4 + (bpc-i-1) * unit_size + ( unit_size - j ) - 2 ] = colormap[bcolor][2] ;
			}
		}
    }
    
    putimage(x, y ,&font.img ,cfont.Wmode );
    
}

void english (int x, int y, unsigned char v )  /* 在座標 x, y 印出 ASCII 碼 (英、數字等) */
{
  int len;
  font.w= 7;			    // 字寬  8 bit   
  font.h= 14;			    // 字高 15 條    
  cfont.Cw= 1;				// CW 字寬	   
  
  len = ( font.h +1 ) * cfont.Cw ;
  
  //printf("=========================\n") ;
  //printf("english len = %d   height %d    cw %d \n" ,len ,font.h , cfont.Cw ) ;
  //printf("%d \n" ,(long) v * len ) ;
  
  lseek( cfont.Handl , (long) v * len , 0 );	  // 中文字型檔  
  _read( cfont.Handl , &font.map , len );
  
  //printf("font.w %d font.h %d \n" ,font.w ,font.h );
  
  //printf("==========  %c  ============== \n" ,v ) ;
  //for(int i=0; i<=14; i++) printf("%2d %2x \n" ,i ,font.map[i] ) ;
  //printf("============================== \n"  ) ;
  
  putcharmap( &font.map[0] ,x ,y );
}

void chinese(int x, int y, unsigned char high, unsigned char low)  /* 在座標x,y印出中文字型(含特殊符號) */
{
  int len, code, foff;
  unsigned char tmp_pattern[30];
  
  font.w= 7 ;   //字寬  8 bit * 2     font.w= 15; 字寬 16 bit
  font.h= 14;	// 字高 15 條    
  cfont.Cw= 2;	// CW 字寬	   
  low -= (low>0x7e) ? 0x62 : 0x40 ;
  code = (unsigned int)(high-0xa1) * 157 + low;
  if (code<471)  foff= 3840;		  // 特殊符號碼, 扣除 ASC 檔案大小 
  else {code -= 471; foff= 3840+ 12240; } // 內碼 A440 開始 
  len= (font.h +1) * cfont.Cw;
  lseek(cfont.Handl, (long)code*len+foff, 0);
  // _read(cfont.Handl, &font.map, len);
  _read(cfont.Handl, tmp_pattern, len);
  
  // printf("font.w %d font.h %d \n" ,font.w ,font.h );
  //printf("==========  %x %x  ============== \n" ,high ,low ) ;
  //for(int i=0; i<=14; i++) printf("%2d %2x %2x\n" ,i ,tmp_pattern[i*2] ,tmp_pattern[i*2+1] ) ;
  //printf("============================== \n"  ) ;
  
  // 轉換 
  for (int i = 0; i <= 28; i = i + 2) {
    font.map[i / 2] = tmp_pattern[i];
    font.map[15 + i / 2] = tmp_pattern[i + 1];
  }
  
  //printf("==========  %x %x  ============== \n" ,high ,low ) ;
  //for(int i=0; i<=14; i++) printf("%2d %2x %2x\n" ,i ,font.map[i*2] ,font.map[i*2+1] ) ;
  //printf("============================== \n"  ) ;
  
  for (int i = 0; i <= 1; i++) {
  	putcharmap( &font.map[i * 15] ,x + i * 8 ,y ) ;
  } 
  
}

void cprstr(int x, int y, char *form,...)   // 在 (x,y) 的位置印出字串
{
  va_list arg_ptr;
  
  unsigned char code;
  char str[81], *s = str ;
  
  va_start( arg_ptr, form );	    //  當函數所接收的引數為變數時, 透過此來接收
  
  vsprintf( str, form, arg_ptr) ;
  
  		// debug used
		//code = *s++;
		//printf("english x = %d y = %d  code = %c = %d \n" ,x ,y ,code ,code ) ; english( x, y, code );
      	//printf("chinese x = %d y = %d \n" ,x ,y ) ; chinese( x, y, code, *s++ );
	
    while( *s )
    { 
	  code = *s++;
      
	  if ( code < 127 ) {
		//printf("english x = %d y = %d  code = %c = %d \n" ,x ,y ,code ,code ) ;
	    english( x, y, code );
	  } 
      else {
      	//printf("chinese x = %d y = %d \n" ,x ,y ) ;
	    chinese( x, y, code, *s++ );
	  } 
      
	  y += cfont.Cy ;
      x += (code<127) ? cfont.Cx/2 : cfont.Cx ;
    }
    
    
}



/*
void MsgLine(char *Str1)   // 在 訊息列上, 顯示文字
{
  char TmpStr[82];
  int C1, C2;
  int DaFlag, DaX1, DaY1, DaX2, DaY2;
  int MsShape, MsDisp;
  int ColorBak;

  for (int mi=0; mi<80; mi++)  TmpStr[mi] = 32;   // 空白字元
  TmpStr[80] = 0;
  GetDrawArea(&DaFlag, &DaX1, &DaY1, &DaX2, &DaY2); // 取得螢幕繪圖區域
  SetDrawArea(0);				    // 設定繪圖區域   VGA 區域
  GetGmouseShape(&MsShape, &MsDisp);		    // 讀取繪圖模式下滑鼠游標的樣式
  GMouseDisp(0);				    // 隱藏滑鼠
  ColorBak = getcolor();
  C1 = cfont.Color;			// 儲存原來的顏色設定
  C2 = cfont.BColor;
  cfont.Color = scr.CwordPre;
  cfont.BColor = scr.CwordBak;
  cprstr(scr.Ax1, scr.Ay1, TmpStr);	// 先填入一行空白行
  cprstr(scr.Ax1, scr.Ay1, Str1);	// 再顯示所要的字串
  cfont.Color = C1;			// 恢復原來的顏色設定
  cfont.BColor = C2;
  setcolor(ColorBak);
  GMouseDisp(MsDisp);
  SetDrawArea(DaFlag);			// 設回 原來的區域
}

void Show_Msg(int x, int y, char *Fstr, char *Str1, int color1, int color2)   // 顯示文字
{
  int C1, C2, MSflag;
  C1 = cfont.Color;	 
  C2 = cfont.BColor; 
  SetDrawArea(0);	     // 設定 VGA 區域
  
  //MSflag = GMouseDisp(0);    // 繪圖模式下, 游標的顯現(1) 或隱藏(0)
  
  cfont.Color = color1;    
  cfont.BColor = color2; 
  
  cprstr( x, y, Fstr, Str1 );
  
  cfont.Color = C1;   
  cfont.BColor = C2;
  
  //if (MSflag)  GMouseDisp(1);
  
}


int trim_str(char *ptr) 		    // 將字串前後的空白字元刪除     
{					    // (字串只到 0x0D 或 0x0A 之前) 
  int i, j, k;
  k=0; while(ptr[k]!=0x0A && ptr[k]!=0) k++;   ptr[k]= 0;   // 將字串後的新行字元刪除  
  k=0; while(ptr[k]!=0x0D && ptr[k]!=0) k++;   ptr[k]= 0;   // 將字串後的歸位字元刪除  
  k=strlen(ptr)-1; while(ptr[k]==0x20 && k>=0) ptr[k--]= 0; // 將字串後的空白字元刪除  
  while (ptr[0]==0x20)					    // 將字串前的空白字元刪除  
  { k= strlen(ptr);  for (i=0; i<k; i++) ptr[i]= ptr[i+1];}
  return(strlen(ptr));
}

int replace_str(char *strptr, char *oldstr, char *newstr)  // 將字串內的某字串轉成新字串		   
{
  char *ptr1, *ptr2;
  char str1[256];

  ptr1 = strptr;
  str1[0] = 0;
  while ((ptr2 = strstr(ptr1, oldstr)) != 0)
    { *ptr2 = 0;
      strcat(str1, ptr1);      // 將舊字串前不受改變的字串寫入	
      strcat(str1, newstr);    // 寫入新字串			
      ptr1 = ptr2 + strlen(oldstr);    // 舊字串的長度 
    }
  strcat(str1, ptr1);	       // 將舊字串後不受改變的字串寫入	
  strcpy(strptr, str1);        // 再拷貝回原來的字串位置	
  return(0);
}

int un_compress(char *s)			 // 將含 tab  之字串解開    
{
   int len, pos=0, i, k, m;
   char ch, *sp= s;
   while(*sp) { 				 // 未到字串結尾	    
      k= strlen(s);
      while(*sp!=9 && *sp) {sp++; pos++;}	 // 尋找是否含 tab  字元    
      if(*sp) { 				 // 若含 tab  字元	    
	 *sp= ' ';                               // 補足空白字元            
	 m= (pos/8+1)*8-pos-1;
	 memmove(sp, sp+m, k-pos+1);
	 memset(sp, m, ' ');
   }  }
   return(0);
}  // end of un_compress 
*/

void beep()
{
  //sound(450);
  delay(100);
  //nosound();
}





int TextEdit(int X1, int Y1, int SpaceCol, int MaxCol, int Cur, char *str)   // 編輯文字  
{      // 以(X1,Y1)起點, SpaceCol字空間,  最大輸入MaxCol字, 游標在第 Cur 格, *str 字串儲存的位置 
  int i, j;
  int InsFlag = 1;
  int key, key1;
  int ColPos=0;     // 顯示字框中第一個字元, 在字串中的位置
  int Length;	    // 字串長度
  
  
  //char bar[64];     // 文字游標緩衝區
  //bar[0] = 7;  bar[1] = 0;		  // 文字游標的 寬-1		
  //bar[2] =14;  bar[3] = 0;		  // 文字游標的 高-1		
  //for(i= 4; i<64; i++)	bar[i]= 0xc0;	 // 文字游標的內容  (彩色)     

  char bar[396];     // 文字游標緩衝區  7 * 14 * 4 + 4 = 396
  bar[0] =  7;  bar[1] = 0;		 // 文字游標的 寬-1
  bar[2] = 14;  bar[3] = 0;		 // 文字游標的 高-1
  for(i= 4; i<396; i++)	bar[i]= 0xc0;	 // 文字游標的內容  (彩色)


  Length = strlen(str);   // 字串長度 
  if (Cur > Length)  Cur = Length;
  if (Cur < 0) Cur = 0;
  for (i=0; i<SpaceCol; i++)
    { if ((ColPos+i) < Length)	cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
      else  cprstr(X1+i*8, Y1, " ");
    }
  putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
  while (1)
    { key = getch();
      switch (key)
	{ case 0:      // 功能鍵 
	     switch(getch())
	       { case 15:   // Shift Tab 
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    Cur = 0;
		    ColPos=0;
		    for (i=0; i<SpaceCol; i++)
		      { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			else  cprstr(X1+i*8, Y1, " ");
		      }
		    return(3);
		 case 82:    // Ins 鍵 
		    InsFlag *= -1;
		    break;
		 case 83:    // Del 鍵 
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    if ((ColPos + Cur) < Length)
		      { for (i=ColPos+Cur; i<Length; i++)
			  str[i] = str[i+1];
			Length--;
			for (i=0; i<SpaceCol; i++)
			  { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			    else  cprstr(X1+i*8, Y1, " ");
		      }   }
		    else beep();
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
		    break;
		 case  71:   // Home  
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    Cur = 0;
		    ColPos=0;
		    for (i=0; i<SpaceCol; i++)
		      { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			else  cprstr(X1+i*8, Y1, " ");
		      }
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
		    break;
		 case  79:   // End   
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    if (Length < SpaceCol)
		      { Cur = Length;
			ColPos=0;
			for (i=0; i<SpaceCol; i++)
			  { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			    else  cprstr(X1+i*8, Y1, " ");
		      }   }
		    else
		      { Cur = SpaceCol -1;
			ColPos = Length - SpaceCol +1;
			for (i=0; i<SpaceCol; i++)
			  { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			    else  cprstr(X1+i*8, Y1, " ");
		      }   }
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
		    break;
		 case 75:    // 向左鍵 
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    Cur--;
		    if (Cur < 0)
		      { Cur = 0;
			if (ColPos > 0)  ColPos--;
			for (i=0; i<SpaceCol; i++)
			  { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
			    else  cprstr(X1+i*8, Y1, " ");
			  }
		      }
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
		    break;
		 case 77:    // 向右鍵 
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 清除目前的游標 
		    Cur++;
		    if ((Cur + ColPos) > Length )  // 超過字串長度, 不予處理 
		      { Cur--;	beep(); }
		    else
		      { if (Cur >= SpaceCol)
			  { Cur = SpaceCol -1;
			    ColPos++;
			    for (i=0; i<SpaceCol; i++)
			      { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
				else  cprstr(X1+i*8, Y1, " ");
			  }   }
		      }
		    putimage(X1+Cur*8, Y1, &bar, 1);   // 顯示目前的游標 
		    break;
	       }
	     break;
	  case	8:    // Back Del 
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 清除目前的游標 
	     if ((ColPos+Cur) > 0)
	       { for (i=ColPos+Cur-1; i<Length; i++)  str[i] = str[i+1];
		 Length--;
		 Cur--;
		 if (Cur < 0)
		   { Cur = 0;  if (ColPos > 0)	ColPos--; }
		 for (i=0; i<SpaceCol; i++)
		   { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
		     else  cprstr(X1+i*8, Y1, " ");
	       }   }
	     else beep();
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 顯示目前的游標 
	     break;
	  case	9:    // TAB   鍵 
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 清除目前的游標 
	     Cur = 0;
	     ColPos=0;
	     for (i=0; i<SpaceCol; i++)
	       { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
		 else  cprstr(X1+i*8, Y1, " ");
	       }
	     return(2);
	  case 13:    // ENTER 鍵 
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 清除目前的游標 
	     Cur = 0;
	     ColPos=0;
	     for (i=0; i<SpaceCol; i++)
	       { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
		 else  cprstr(X1+i*8, Y1, " ");
	       }
	     return(1);
	  case 27:    // ESC  鍵 
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 清除目前的游標 
	     Cur = 0;
	     ColPos=0;
	     for (i=0; i<SpaceCol; i++)
	       { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
		 else  cprstr(X1+i*8, Y1, " ");
	       }
	     return(0);
	  default:    // 輸入資料 
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 清除目前的游標 
	     Cur++;
	     if ((Cur + ColPos) > MaxCol)     // 超過最大字串限制, 不予處理
	       { Cur--;   beep(); }
	     else
	       { if (Cur >= SpaceCol)  { Cur = SpaceCol -1; ColPos++; }
		 if (InsFlag == 1)	// 插入模式 
		   { for (i=Length+1; i>=ColPos+Cur; i--)  str[i] = str[i-1];
		     str[ColPos+Cur-1] = key;
		     if (Length >= MaxCol) str[Length] = 0;
		     else Length++;    // 字串長度
		   }
		 else			// 替換模式 
		   { str[ColPos+Cur-1] = key;
		     if ((Cur + ColPos) > Length )  // 超過字串長度, 增加長度 
		       { str[ColPos+Cur] = 0;
			 if (Length >= MaxCol) str[Length] = 0;
			 else  Length++;
		       }
		   }
		 for (i=0; i<SpaceCol; i++)
		   { if ((ColPos+i) < Length)  cprstr(X1+i*8, Y1, "%c", str[ColPos+i]);
		     else  cprstr(X1+i*8, Y1, " ");
		   }
	       }
	     putimage(X1+Cur*8, Y1, &bar, 1);	// 顯示目前的游標 
	     break;
	}
    }
}





