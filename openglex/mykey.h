#ifndef __MYKEY
#define __MYKEY

//rect1在rect2中
#define RECT_IN_RECT(rect1,rect2) (rect1.bottom>rect2.bottom	\
						&& rect1.top<rect2.top	\
						&& rect1.left>rect2.left	\
						&& rect1.right<rect2.right	)
//rect1在rect2之外
#define RECT_OUT_RECT(rect1,rect2) (rect1.top<rect2.bottom	\
						|| rect1.bottom>rect2.top	\
						|| rect1.left>rect2.right	\
						|| rect1.right<rect2.left	)
//x z在rect之中
#define POINT_IN_RECT(x,z,rect) (x<rect.top \
						&& x>rect.bottom \
						&& z<rect.right \
						&& z>rect.left	)

//文件处理
#define ISTEXT(x) ((x)>=32 && (x)<=126)

//获取一行文本
int FGetLine(char *pchar,FILE *fp)
{
	int ch=-1;
	int i=0;

	//过滤前面的非法字符
	while(!ISTEXT(ch) && !feof(fp))
	{
		ch=fgetc(fp);
	}

	if(!ISTEXT(ch))
	{
		pchar[0]=0;
		return 0;
	}
	
	while(ISTEXT(ch) && !feof(fp))
	{		
		pchar[i]=ch;
		i++;
		ch=fgetc(fp);
	}
	pchar[i]=0;
	return 1;
}

int FGetLineJumpCom(char *pchar,FILE *fp)
{
	int ch=-1;
	int i=0;

	//过滤前面的非法字符
	while(!ISTEXT(ch) && !feof(fp))
	{
		ch=fgetc(fp);
	}

	if(!ISTEXT(ch))
	{
		pchar[0]=0;
		return 0;
	}

	//jump comment begin
	if(';' == ch)
	{
		//读完一行
		while(ch!=10)
		{
			ch=fgetc(fp);
		}
		//过滤非法字符
		while(!ISTEXT(ch) && !feof(fp))
		{
			ch=fgetc(fp);
		}
	}
	//jump comment end
	
	//得到新的一行
	while(ISTEXT(ch) && !feof(fp))
	{		
		pchar[i]=ch;
		i++;
		ch=fgetc(fp);
	}
	pchar[i]=0;
	return 1;
}

//KEY

#define	KEY_A	65
#define	KEY_B	66
#define	KEY_C	67
#define	KEY_D	68
#define	KEY_E	69
#define	KEY_F	70
#define	KEY_G	71
#define	KEY_H	72
#define	KEY_I	73
#define	KEY_J	74
#define	KEY_K	75
#define	KEY_L	76
#define	KEY_M	77
#define	KEY_N	78
#define	KEY_O	79
#define	KEY_P	80
#define	KEY_Q	81
#define	KEY_R	82
#define	KEY_S	83
#define	KEY_T	84
#define	KEY_U	85
#define	KEY_V	86
#define	KEY_W	87
#define	KEY_X	88
#define	KEY_Y	89
#define	KEY_Z	90

#define	KEY_F1 0x70
#define	KEY_F2 0x71
#define	KEY_F3 0x72
#define	KEY_F4 0x73
#define	KEY_F5 0x74
#define	KEY_F6 0x75
#define	KEY_F7 0x76
#define	KEY_F8 0x77
#define	KEY_F9 0x78
#define	KEY_F10 0x79
#define	KEY_F11 0x7a
#define	KEY_F12 0x7b

#endif