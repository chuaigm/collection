
#include "GlobalData.h"


unsigned char KeyState[ 256 ] = {0};		// ����״̬����


short g_GameState = 0;			// ��Ϸ״̬��0����Ϸ����. 1:�˵�ѡ��  2:��Ϸ����  3:�ɾ�  4:����

short g_MenuStateMain = 1;		// ���˵�ѡ��
short g_MenuStateSub  = 1;		// �μ��˵�ѡ��

CSCOpenGLmodel g_myModel;		// ȫ��ģ����

int g_Height , g_Width;			// ���ڸ����

float HIGHtime[10] = {0};		// ���ʱ��
short HIGHexce[10] = {0};		// ��߾�����
DWORD HIGHscore[10] = {0};		// ��߷���