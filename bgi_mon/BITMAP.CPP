//#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "monitor.h"
#include "chinese.h"

extern SCREEN scr;

char  HelpDAT[20] = "MONITOR.DAT";


unsigned char colormp2[][4] ={ { 0x0,0x0,0x0,0x0 } // 0
    ,{ 0xff,0x0,0x0,0x0 } // 1
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

typedef struct {
 unsigned char bmphic[55];
} BMP_HI ;



typedef struct		       /* BMP 檔頭資料		    */
       {
	int  Type;	       /* "BM"   識別標幟           */
	long Size;	       /* 本檔案的大小		    */
	int  Reser1;	       /* 保留	  0		    */
	int  Reser2;	       /* 保留	  0		    */
	long Offset;	       /* 實際影像資料的位址	    */
       } BMP_Head ;

typedef struct		       /* 影像資料的檔頭資料	    */
       {
	long Size;	       /* 本結構的大小 (=40byte)    */
	long Width;	       /* 影像圖形的寬度 (pixels)   */
	long Height;	       /* 影像圖形的高度 (pixels)   */
	int  Planes;	       /* 色層的數目 DIB = 1	    */
	int  BiCount;	       /* 每一pixels所需的bit數 (16色=4)    */
	long Compress;	       /* =0 不壓縮 =1 8位元RLE =2 4位元RLE */
	long SizeImage;        /* 影像資料實際大小 (不壓縮時可=0)   */
	long XPPM;	       /* 看起來最好的X軸輸出解析度 (每公尺圖點數) */
	long YPPM;	       /* 看起來最好的Y軸輸出解析度 (每公尺圖點數) */
	long ClrUsed;	       /* 使用的顏色數目	    */
	long ClrImport;        /* 主要使用顏色數目	    */
       } BITMAP_P ;

typedef struct		       /* 顏色的定義		    */
       {
	unsigned char B;       /* 藍色			    */
	unsigned char G;       /* 綠色			    */
	unsigned char R;       /* 紅色			    */
	unsigned char Reserv;  /* 保留			    */
       } RGB ;

typedef struct
  {
    int  BmpCount;	    // 實際 BMP 的數量
    int  Reserve[7];	    // 保留
  } FirstData;

typedef struct
  {
    char  FileName[16];     // 該 BMP 的檔案名稱(含副檔名)
    long  Addr; 	    // 該 BMP 的起始位址
    int   Reserve[6];
  } BmpData;



int Get_BMP16A( int x, int y, FILE *fin , long Fadr  )
{
   int width,height;              //width and height of the bitmap
   unsigned long temp=0,i=0,j=0;  //some variables i need
   unsigned long ww;
   width=0;height=0;              //initialise wisth and height to zero
   
   fseek(fin,18,SEEK_SET);        //got to offset 18 in file
   for(i=0x1;i<=0x10000;i*=0x100) //read the width
   {
           temp=fgetc(fin);
           width+=(temp*i);
   }
   fseek(fin,22,SEEK_SET);        //go to offset 22 in file
   for(i=0x1;i<=0x10000;i*=0x100) //read the height
   {
           temp=fgetc(fin);
           height+=(temp*i);
   }
   
   //printf("Color table\n") ;
   fseek(fin,54,SEEK_SET);        //go to offset 54 in file
   for( i=0 ; i<16 ; i++ )
   {
   		//printf("%2d " ,i ) ;
		temp=fgetc(fin);
		colormp2[i][0] = temp ;
   		//printf("%2x " ,temp ) ;
		temp=fgetc(fin);
		colormp2[i][1] = temp ;
   		//printf("%2x " ,temp ) ;
		temp=fgetc(fin);
		colormp2[i][2] = temp ;
   		//printf("%2x " ,temp ) ;
		temp=fgetc(fin);
   		//printf("\n") ;
   }
   
   
   //printf("Color table\n") ;
   //for(int i=0; i<=15; i++) printf("%2d   %2x %2x %2x \n" ,i ,colormp2[i][0] ,colormp2[i][1] ,colormp2[i][2]  ) ;
   //printf("\n") ;
   
//w 16  h 16
//1 ww = 16
//2 ww = 8
//     ww/4 = 2
//     (ww/4)*4= 8
//3 ww = 8   
   
   //printf("w %d  h %d \n" ,width ,height );
   
   ww=width;         
   
   //printf("1 ww = %lu \n", ww );
   
   //ww is the number of reads to make for each horizontal line
   //ww has to be half of width - since each pixel is only 4 bits of information
   //just getting the correct value
   if( ww%2 ) ww++; 
   ww/=2;
   
   //printf("2 ww = %lu \n", ww );
   //printf("     ww/4 = %lu \n", ww/4 );
   //printf("     (ww/4)*4= %lu \n", (ww/4)*4 );
   
   // now - ww is stored as sets of 4 pixels each
   // if width is less than ww*2 we ignore what we read
   if( ww%4 ) ww=( ww/4 ) * 4 + 4 ;
   
   //printf("3 ww = %lu \n", ww );
   
   // size = 4 + wide * height * 4 (rgbs)
   void *pic1 ;
   int size2 = 4 + width * height * 4 ; 
   pic1 = malloc( size2 );
   
   //printf("size2: %d \n " ,size2 ) ;
   
   unsigned char *tp01 = (unsigned char *) pic1 ;
   tp01[0] = width%256 ;
   tp01[1] = width/256 ;
   tp01[2] = height%256 ;
   tp01[3] = height/256 ;
   
   //printf("w: %d  %d     h: %d  %d \n " ,tp01[1] ,tp01[0] ,tp01[3] ,tp01[2] ) ;
   
   // Ok! offset 118 - lets read the pixels - 
   // remember the bitmap is stroed up - side - down
   fseek(fin,118,SEEK_SET); 
   
   unsigned long  pln1 = 4 ;
   int ch,ch1,ch2;
   for( i=0 ; i<height ; i++ )
   {
   	 //printf("%2x " ,i  ) ;
     for(j=0;j<ww;j++)
     {
           	ch=fgetc(fin); 
           	//printf("%2x " ,ch ) ;
           	
           	//each character read is 2 pixels - yes 4 bits per pixel - so 16 colors
           	ch1=ch; ch2=ch;                                
           	//find those colors using bitwise ops
           	ch1 = ch1 & (0xf0);  // ch1 = ch1$gt; $gt; 4;
           	ch1 = ch1 >> 4 ;
           	ch2 = ch2 & (0x0f); 
           	//ok so put the first pixel read on screen
           	if( j*2 < width )
           	{
           		*(tp01+pln1) = colormp2[ch1][0]  ;
           		pln1++;
           		*(tp01+pln1) = colormp2[ch1][1]  ;
           		pln1++;
           		*(tp01+pln1) = colormp2[ch1][2]  ;
           		pln1++;
           		pln1++;
			}
           	if( j*2+1 < width )
           	{
           		*(tp01+pln1) = colormp2[ch2][0]  ;
           		pln1++;
           		*(tp01+pln1) = colormp2[ch2][1]  ;
           		pln1++;
           		*(tp01+pln1) = colormp2[ch2][2]  ;
           		pln1++;
           		pln1++;
			}
     }
     //printf("\n") ;
   }
   
   putimage( x, y , pic1, COPY_PUT );
   fclose(fin); //close the file handle
   free(pic1) ;
   
   return 1;
}



/*
int Get_BMP16A(int x, int y, FILE *fin, long Fadr)	// 秀出BMP 16色的圖案
{						       // 定位點 在圖案的左下角
   BMP_Head  bmp;
   BITMAP_P    bit;
   RGB	     *rgbbuf;
   unsigned  char *putbuf;
   int	     i, j ,pix1, pix2, ln1, ln2;
   int	     Rplane, Gplane, Bplane, Hplane;	  // 各色平面資料的開始位址
   unsigned char data, mask1, mask2;
   unsigned int  PUTbyt, LNbyt;

   fseek(fin, Fadr, SEEK_SET);
   fread(&bmp,sizeof(BMP_Head),1,fin);	     // BMP 檔頭資料
   fread(&bit,sizeof(BITMAP_P),  1,fin);	     // 影像資料的檔頭資料
   LNbyt  = (int)((bit.Width - 1) / 8) + 1;
   PUTbyt = 4 + LNbyt * 4;		     // 2(w)+2(h)+ 4 個色平面
   rgbbuf = (RGB *)malloc(sizeof(RGB) * 16); // 配置 16 個顏色定義  (16色)
   putbuf = (unsigned char *)malloc(PUTbyt); // 配置一行put buf 的空間

   fread(rgbbuf,sizeof(RGB),16,fin);
   if (bit.BiCount != 4)   return(2);	   // 只支援 16 色模式
   if (bit.Compress != 0)  return(3);	   // 只支援不壓縮模式

   *putbuf = (bit.Width -1) ;		   // putimage 的寬
   *(putbuf+1) = (bit.Width -1) >> 8;
   *(putbuf+2) = 0;			   // putimage 的高
   *(putbuf+3) = 0;
   Hplane = 4 ; 			   // 亮色平面資料的開始位址
   Rplane = 4 + LNbyt ; 		   // 紅色平面資料的開始位址
   Gplane = 4 + 2 * LNbyt ;		   // 綠色平面資料的開始位址
   Bplane = 4 + 3 * LNbyt ;		   // 藍色平面資料的開始位址

   if (x < 0) return(4);		  // put圖案, 超出範圍
   if (y < 0) return(5);

   for (i=0; i<bit.Height; i++)
    { for (j=4; j< PUTbyt; j++)  *(putbuf+j) = 0;     // 清除緩衝區
      for (j=0; j< LNbyt*4; j++)
       { data = fgetc(fin);
	 pix1 = data >> 4 ;
	 pix2 = data & 0x0f;
	 ln1  = (int)(j / 4);
	 ln2  = (int)(j / 4);
	 mask1= 0x80 >> ((j % 4) * 2);
	 mask2= mask1 >> 1;
	 if ((rgbbuf+pix1)->R > 0x40 )	     // 紅及亮平面
	   { *(putbuf+Rplane+ln1) |= mask1;
	     if  ((rgbbuf+pix1)->R > 0xc0 )  *(putbuf+Hplane+ln1) |= mask1;
	   }
	 if ((rgbbuf+pix1)->G > 0x40 )	     // 綠及亮平面
	   { *(putbuf+Gplane+ln1) |= mask1;
	     if  ((rgbbuf+pix1)->G > 0xc0 )  *(putbuf+Hplane+ln1) |= mask1;
	   }
	 if ((rgbbuf+pix1)->B > 0x40 )	     // 藍及亮平面
	   { *(putbuf+Bplane+ln1) |= mask1;
	     if  ((rgbbuf+pix1)->R > 0xc0 )  *(putbuf+Hplane+ln1) |= mask1;
	   }
	 if ((rgbbuf+pix2)->R > 0x40 )	     // 紅及亮平面
	   { *(putbuf+Rplane+ln2) |= mask2;
	     if  ((rgbbuf+pix2)->R > 0xc0 )  *(putbuf+Hplane+ln2) |= mask2;
	   }
	 if ((rgbbuf+pix2)->G > 0x40 )	     // 綠及亮平面
	   { *(putbuf+Gplane+ln2) |= mask2;
	     if  ((rgbbuf+pix2)->G > 0xc0 )  *(putbuf+Hplane+ln2) |= mask2;
	   }
	 if ((rgbbuf+pix2)->B > 0x40 )	     // 藍及亮平面
	   { *(putbuf+Bplane+ln2) |= mask2;
	     if  ((rgbbuf+pix2)->R > 0xc0 ) *(putbuf+Hplane+ln2) |= mask2;
       }   }
       putimage(x, y-i, putbuf, COPY_PUT);
    }

   free(rgbbuf);
   free(putbuf);
   return(0);
}
*/


int Get_BMP_Size(char *str, long Data[])    // 取得BMP 16色的資訊
{ // Data[0] = Addr;	   // 該BMP圖形在檔案中的位址
  // Data[1] = Size;	   // 本結構的大小 (=40byte)
  // Data[2] = Width;	   // 影像圖形的寬度 (pixels)
  // Data[3] = Height;	   // 影像圖形的高度 (pixels)

  FILE	    *fin;
  FirstData MyFData;
  BmpData   MyBmpData;
  BMP_Head  bmp;
  BITMAP_P    bit;
  int	    mi;

  if ((fin = fopen(HelpDAT, "rb")) == 0)   return(1);    // 無法開啟檔案
  
  fread(&MyFData, sizeof(FirstData), 1, fin);
  
  for (mi=0; mi<MyFData.BmpCount; mi++)
    { fread(&MyBmpData, sizeof(BmpData), 1, fin);
      if (stricmp(MyBmpData.FileName, str) == 0)  // 找到
	{ Data[0] = MyBmpData.Addr;
	  fseek(fin, Data[0], SEEK_SET);
	  fread(&bmp,sizeof(BMP_Head),1,fin);	// BMP 檔頭資料
	  fread(&bit,sizeof(BITMAP_P),  1,fin);	// 影像資料的檔頭資料
	  Data[1] = bit.Size;			// 本結構的大小 (=40byte)
	  Data[2] = bit.Width;			// 影像圖形的寬度 (pixels)
	  Data[3] = bit.Height; 		// 影像圖形的高度 (pixels)
	  fclose(fin);
	  return(0);
    }	}
  fclose(fin);
  return(2);   // 找不到
}


int Get_BMP16(int x, int y, char *str, int mode)    // 秀出BMP 16色的圖案
    // mode=0 (x,y)為圖案中心點, =1 左上角, =2 右上角, =3 右下角, =4 左下角
{
   FILE      *fin;
   int	     mi, mj;
   int	     x1, y1;			     // 圖案的左下角座標
   long      Fadr;
   FirstData MyFData;
   BmpData   MyBmpData;
   BMP_Head  bmp;
   BITMAP_P    bit;
   
   printf("%s \n" ,str ) ;
   
   if ((fin = fopen(HelpDAT, "rb")) == 0)   return(1);    // 無法開啟檔案
   
   
   //printf("size1 %d  size2 %d  \n" ,sizeof(FirstData) ,sizeof(BmpData) ) ;
   
   //fread(&MyFData, sizeof(FirstData) , 1, fin);
   fread( &MyFData ,16 , 1 ,fin );
   
   for (mi=0; mi<MyFData.BmpCount; mi++)
   { 
   		//fread(&MyBmpData, sizeof(BmpData), 1, fin);
   		fread( &MyBmpData ,32 ,1 ,fin );
   		
   		//printf("%2d  %s \n" ,mi ,MyBmpData.FileName ) ;
   		
       	if (stricmp(MyBmpData.FileName, str) == 0)  // 找到
	 	{ 
		 	Fadr = MyBmpData.Addr;
	   		fseek(fin, Fadr, SEEK_SET);
	   		fread(&bmp,sizeof(BMP_Head),1,fin);	     // BMP 檔頭資料
	   		fread(&bit,sizeof(BITMAP_P),  1,fin);	     // 影像資料的檔頭資料
	   		break;	  // 跳出迴圈
     	}	 
   }
   
   //printf("step2 mi %d  MyFData.BmpCount %d \n" ,mi ,MyFData.BmpCount ) ;
   
   if (mi >= MyFData.BmpCount)
   { 
   		fclose(fin);
       	return(2);
   }
   
   
   /*
   ////////////////////////////////////////////////////////
   /////////////////// 201807 added ///////////////////////
   char BMP_Path[130];	 // SP2 檔 的預設目錄
   char tmpstr[256];
   getcwd(tmpstr, 180);
   strcpy(BMP_Path, tmpstr);
   strcat(BMP_Path, "\\BMP\\");
   strcat( BMP_Path , MyBmpData.FileName );
   str = BMP_Path ;
   
   if ((fin = fopen(str, "rb")) == 0) {  printf("Can not open file \n" );   return(1);  }  // 無法開啟檔案
   
   // 150 KB (153,718 位元組) 153718
   
   BMP_HI    bmphi ;
   fread( &bmphi ,54 ,1 ,fin);	     // BMP 檔頭資料
   bmp.Size = bmphi.bmphic[4] * 65536 + bmphi.bmphic[3] * 256 + bmphi.bmphic[2] ;
   bit.Width  =  bmphi.bmphic[19] * 256 + bmphi.bmphic[18] ;
   bit.Height =  bmphi.bmphic[23] * 256 + bmphi.bmphic[22] ;
   bit.BiCount  = bmphi.bmphic[29] * 256 + bmphi.bmphic[28] ;
   bit.Compress = bmphi.bmphic[31] * 256 + bmphi.bmphic[30] ;
   //printf("%d \n", bmp.Size );
   //printf("%d \n", bit.Width );
   //printf("%d \n", bit.Height );
   //printf("%d \n", bit.BiCount );
   //printf("%d \n", bit.Compress );
   
   ////////////////////////////////////////////////////////
   /////////////////// END 201807 added ///////////////////////
   */
   
   
   
   
   
   
   // 201807 Change 1 to 4
   switch(mode)
    { 
	  case 4:				       // 定位點為圖案左上角
	  x1 = x;    y1 = y + bit.Height -1;   break;
	  
      case 2:				       // 定位點為圖案右上角
	  x1 = x - bit.Width ;	y1 = y + bit.Height -1;   break;
      case 3:				       // 定位點為圖案右下角
	  x1 = x - bit.Width ;	 y1 = y;  break;
      
	  case 1:				       // 定位點為圖案左下角
	  x1 = x;  y1 = y;  break;
	  
      default:				       // 定位點為圖案中央
	  x1 = x - bit.Width / 2;  y1 = y + (bit.Height / 2) - 1;  break;
    }
   
   /*
   switch(mode)
    { case 1:				       // 定位點為圖案左上角
	  x1 = x;    y1 = y + bit.Height -1;   break;
      case 2:				       // 定位點為圖案右上角
	  x1 = x - bit.Width ;	y1 = y + bit.Height -1;   break;
      case 3:				       // 定位點為圖案右下角
	  x1 = x - bit.Width ;	 y1 = y;  break;
      case 4:				       // 定位點為圖案左下角
	  x1 = x;  y1 = y;  break;
      default:				       // 定位點為圖案中央
	  x1 = x - bit.Width / 2;  y1 = y + (bit.Height / 2) - 1;  break;
    }
    */
   
   if (x1 < 0)	   // put圖案, 超出範圍
     { 
	   fclose(fin);
       return(3);
     }
   if (y1 < 0)
     { 
	   fclose(fin);
       return(4);
     }
   
   Get_BMP16A(x1, y1, fin, Fadr);   // 秀出BMP 16色的圖案

   fclose(fin);
   return(0);
}


// mode=0 (x,y)為圖案中心點, =1 左上角, =2 右上角, =3 右下角, =4 左下角 // 秀出BMP 16色的圖案
int show_BMP16(int x, int y, char *str, int mode)    
{
   FILE      *fin;
   int	     x1, y1;			     // 圖案的左下角座標
   BMP_Head  bmp ;
   BITMAP_P  bit ;
   
   printf("str: %s \n" ,str );
   
   if ((fin = fopen(str, "rb")) == 0)   return(1);    // 無法開啟檔案

/*   
fread(buffer,size,count,fp); 
fwrite(buffer,size,count,fp); 
3.說明 
（1）buffer：是一個指標，對fread來說，它是讀入數據的存放地址。對fwrite來說，是要輸出數據的地址。 
（2）size：要讀寫的字節數； 
（3）count:要進行讀寫多少個size字節的數據項； 
（4）fp:文件型指標。    */
/*
 printf() 時要指定整數、浮點數、字元等進行顯示
%c	以字元方式輸出
%d	10 進位整數輸出
%f	浮點數輸出
%u	無號整數輸出
%x, %X	將整數以 16 進位方式輸出
%s	字串輸出
%lu	long unsigned 型態的整數
%p	指標型態
*/
   
   // 150 KB (153,718 位元組) 153718
   
   BMP_HI    bmphi ;
   fread( &bmphi ,54 ,1 ,fin);	     // BMP 檔頭資料
   bmp.Size = bmphi.bmphic[4] * 65536 + bmphi.bmphic[3] * 256 + bmphi.bmphic[2] ;
   bit.Width  =  bmphi.bmphic[19] * 256 + bmphi.bmphic[18] ;
   bit.Height =  bmphi.bmphic[23] * 256 + bmphi.bmphic[22] ;
   bit.BiCount  = bmphi.bmphic[29] * 256 + bmphi.bmphic[28] ;
   bit.Compress = bmphi.bmphic[31] * 256 + bmphi.bmphic[30] ;
   printf("%d \n", bmp.Size );
   printf("%d \n", bit.Width );
   printf("%d \n", bit.Height );
   printf("%d \n", bit.BiCount );
   printf("%d \n", bit.Compress );
   
   
   //printf("mode: %d \n" ,mode );
   
   switch(mode)
    { case 1:				       // 定位點為圖案左上角
	  x1 = x;    y1 = y + bit.Height -1;   break;
      case 2:				       // 定位點為圖案右上角
	  x1 = x - bit.Width ;	y1 = y + bit.Height -1;   break;
      case 3:				       // 定位點為圖案右下角
	  x1 = x - bit.Width ;	 y1 = y;  break;
      case 4:				       // 定位點為圖案左下角
	  x1 = x;  y1 = y;  break;
      default:				       // 定位點為圖案中央
	  x1 = x - bit.Width / 2;  y1 = y + (bit.Height / 2) - 1;  break;
    }
   
   //printf("x1: %d y1:%d \n" ,x1,y1 );
   
   if (x1 < 0) return(2);     // put圖案, 超出範圍
   if (y1 < 0) return(3);
   
   Get_BMP16A(x1, y1, fin, 0);	 // 秀出BMP 16色的圖案
   
   
   fclose(fin);
   return(0);
}


/*
int show_BMP16(int x, int y, char *str, int mode)    // 秀出BMP 16色的圖案
    // mode=0 (x,y)為圖案中心點, =1 左上角, =2 右上角, =3 右下角, =4 左下角
{
   FILE      *fin;
   int	     x1, y1;			     // 圖案的左下角座標
   BMP_Head  bmp;
   BITMAP_P    bit;

   if ((fin = fopen(str, "rb")) == 0)   return(1);    // 無法開啟檔案

   fread(&bmp,sizeof(BMP_Head),1,fin);	     // BMP 檔頭資料
   fread(&bit,sizeof(BITMAP_P),  1,fin);	     // 影像資料的檔頭資料

   switch(mode)
    { case 1:				       // 定位點為圖案左上角
	  x1 = x;    y1 = y + bit.Height -1;   break;
      case 2:				       // 定位點為圖案右上角
	  x1 = x - bit.Width ;	y1 = y + bit.Height -1;   break;
      case 3:				       // 定位點為圖案右下角
	  x1 = x - bit.Width ;	 y1 = y;  break;
      case 4:				       // 定位點為圖案左下角
	  x1 = x;  y1 = y;  break;
      default:				       // 定位點為圖案中央
	  x1 = x - bit.Width / 2;  y1 = y + (bit.Height / 2) - 1;  break;
    }
   if (x1 < 0) return(2);     // put圖案, 超出範圍
   if (y1 < 0) return(3);

   Get_BMP16A(x1, y1, fin, 0);	 // 秀出BMP 16色的圖案

   fclose(fin);
   return(0);
}
*/
