/*	BITMAP.H

	定義有關 BMP 圖形方面的功能

*/

int cdecl Get_BMP_Size(char *str, long Data[]); 	  // 取得BMP 16色的資訊
int cdecl Get_BMP16(int x, int y, char *str, int mode);   // 秀出BMP 16色的圖案 (從 14avhk.dat)
int cdecl show_BMP16(int x, int y, char *str, int mode);  // 秀出BMP 16色的圖案 (從檔案 str)
    // mode=0 (x,y)為圖案中心點, =1 左上角, =2 右上角, =3 右下角, =4 左下角
