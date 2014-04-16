
#include "GlobalData.h"


unsigned char KeyState[ 256 ] = {0};		// 键盘状态数组


short g_GameState = 0;			// 游戏状态：0：游戏标题. 1:菜单选择  2:游戏核心  3:成就  4:关于

short g_MenuStateMain = 1;		// 主菜单选项
short g_MenuStateSub  = 1;		// 次级菜单选项

CSCOpenGLmodel g_myModel;		// 全局模型类

int g_Height , g_Width;			// 窗口高与宽

float HIGHtime[10] = {0};		// 最高时间
short HIGHexce[10] = {0};		// 最高绝妙数
DWORD HIGHscore[10] = {0};		// 最高分数