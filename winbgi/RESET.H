

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


