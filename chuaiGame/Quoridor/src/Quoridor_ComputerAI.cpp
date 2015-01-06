#include "stdafx.h"
#include "Quoridor_ComputerAI.h"
#include "Quoridor_openGL.h"

extern CQuoridor* pgm;

Quoridor_ComputerAI::Quoridor_ComputerAI(void)
{
}

Quoridor_ComputerAI::~Quoridor_ComputerAI(void)
{
}

void Quoridor_ComputerAI::SimpleComputer()
{
    // ֻ�е�ǰ����ǵ��Բ�������
    if (pgm->ply_head->id==ID_COMPUTER)
    {
        //Sleep(800);
        //if (random(0,2.0)==0)
        if (pgm->random(0,1.0)==0)
        {
            /*
            switch (ply_head->color)
            {
            case GD_YELLOW:
                break;
            case GD_RED:
                break;
            case GD_GREEN:
                break;
            case GD_BLUE:
                break;
            }
            */
            pos2d tmppoint;
            tmppoint.x=pgm->ply_head->x*2;
            tmppoint.y=pgm->ply_head->y*2;
            pgm->playerMovablePos(tmppoint);
            pos2d targ;
            targ.x=-1;
            targ.y=-1;
            // ��������λ������û�ж����ʤ��������λ��
            for (size_t i=0; i<pgm->preselect_pos.size();i++)
            {
                switch (pgm->ply_head->color)
                {
                case GD_YELLOW:
                    if (pgm->preselect_pos[i].x>tmppoint.x)
                    {
                        targ=pgm->preselect_pos[i];
                    }
                    break;
                case GD_RED:
                    if (pgm->preselect_pos[i].y<tmppoint.y)
                    {
                        targ=pgm->preselect_pos[i];
                    }
                    break;
                case GD_GREEN:
                    if (pgm->preselect_pos[i].x<tmppoint.x)
                    {
                        targ=pgm->preselect_pos[i];
                    }
                    break;
                case GD_BLUE:
                    if (pgm->preselect_pos[i].y>tmppoint.y)
                    {
                        targ=pgm->preselect_pos[i];
                    }
                    break;
                }
                if (targ.x>0 && targ.y>0)
                {
                    break;
                }
            }
            if (targ.x<0 || targ.y<0)
            {
                // ��ѡ�����
                targ=pgm->preselect_pos[pgm->random(0,pgm->preselect_pos.size())];
            }
            // ������������뵽�������Ӵ���׶�
            char tmp=0;
            // ����
            tmp=pgm->gameData[targ.x][targ.y];
            pgm->gameData[targ.x][targ.y]=pgm->gameData[pgm->ply_head->x*2][pgm->ply_head->y*2];
            pgm->gameData[pgm->ply_head->x*2][pgm->ply_head->y*2]=tmp;

            pgm->ply_head->x=targ.x/2;
            pgm->ply_head->y=targ.y/2;

            pgm->preselect_pos.clear();
            // ��Ŀ��λ���ϣ�������ұ���
            switch (pgm->ply_head->color)
            {
            case GD_YELLOW:
                if (pgm->ply_head->x==8)
                {
                    pgm->win_flag=GD_YELLOW;
                    pgm->iGameState=GAME_WIN;
                }
                break;
            case GD_RED:
                if (pgm->ply_head->y==0)
                {
                    pgm->win_flag=GD_RED;
                    pgm->iGameState=GAME_WIN;
                }
                break;
            case GD_GREEN:
                if (pgm->ply_head->x==0)
                {
                    pgm->win_flag=GD_GREEN;
                    pgm->iGameState=GAME_WIN;
                }
                break;
            case GD_BLUE:
                if (pgm->ply_head->y==8)
                {
                    pgm->win_flag=GD_BLUE;
                    pgm->iGameState=GAME_WIN;
                }
                break;
            default:
                break;
            }
        }
        else
        {	// �ڵ������ǰ���ǽ
            switch (pgm->ply_head->next->color)
            {
            case GD_YELLOW:
                break;
            case GD_RED:
                break;
            case GD_GREEN:
                break;
            case GD_BLUE:
                if (pgm->ply_head->next->x>0&&pgm->ply_head->next->x<8)
                {
                    //int x=ply_head->next->x;
                    //int y=ply_head->next->y;
                    //pos2d tpwall1,tmpwall2;
                    //tpwall1.x=x;
                    //tpwall1.y=y+1;
                    //tpwall2.x=x+1;
                    //tpwall2.y=y;
                    //wall_vec.push_back()
                    //gameData[][]=GD_WALL;
                }
                break;
            }
        }
        // ��һλ���
        pgm->ply_head=pgm->ply_head->next;
    }   // ������ǵ��ԣ�ʲô������
}
