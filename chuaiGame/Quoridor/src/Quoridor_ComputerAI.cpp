#include "stdafx.h"
#include "gData.h"
#include "Quoridor_ComputerAI.h"
#include "Quoridor_openGL.h"
#include "A_star_alg.h"
#include "time.h"

extern CQuoridor* pgm;

Quoridor_ComputerAI::Quoridor_ComputerAI(void)
{
    // ���ʹ����������ĵط���Ҫ��Ҫ��ʹ���������
    //srand(unsigned(time(0)));
    // �����������ʵ����ֻ��Ҫ�ڳ�������ʱִ��һ�μ��ɣ�
    //���Ծ�����������ȥִ��
}

Quoridor_ComputerAI::~Quoridor_ComputerAI(void)
{
}

void Quoridor_ComputerAI::FoolAI()
{
    // ֻ�е�ǰ����ǵ��Բ�������
    if (ply_head->id==ID_COMPUTER)
    {
        //if (random(0,2.0)==0)
        if (1)
        {
            pos2d tmppoint;
            tmppoint.x=ply_head->x*2;
            tmppoint.y=ply_head->y*2;
            pgm->playerMovablePos(tmppoint);
            pos2d targ;
            targ.x=-1;
            targ.y=-1;
            // ��������λ������û�ж����ʤ��������λ��
            for (size_t i=0; i<pgm->preselect_pos.size();i++)
            {
                switch (ply_head->color)
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
                targ=pgm->preselect_pos[random(0,pgm->preselect_pos.size())];
            }
            // ������������뵽�������Ӵ���׶�
            char tmp=0;
            // ����
            tmp=gameData[targ.x][targ.y];
            gameData[targ.x][targ.y]=gameData[ply_head->x*2][ply_head->y*2];
            gameData[ply_head->x*2][ply_head->y*2]=tmp;

            ply_head->x=targ.x/2;
            ply_head->y=targ.y/2;

            pgm->preselect_pos.clear();
            // ��Ŀ��λ���ϣ�������ұ���
            switch (ply_head->color)
            {
            case GD_YELLOW:
                if (ply_head->x==8)
                {
                    pgm->win_flag=GD_YELLOW;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_RED:
                if (ply_head->y==0)
                {
                    pgm->win_flag=GD_RED;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_GREEN:
                if (ply_head->x==0)
                {
                    pgm->win_flag=GD_GREEN;
                    iGameState=GAME_WIN;
                }
                break;
            case GD_BLUE:
                if (ply_head->y==8)
                {
                    pgm->win_flag=GD_BLUE;
                    iGameState=GAME_WIN;
                }
                break;
            default:
                break;
            }
        }
        //else
        //{   // �ڵ������ǰ���ǽ
        //    switch (ply_head->next->color)
        //    {
        //    case GD_YELLOW:
        //        break;
        //    case GD_RED:
        //        break;
        //    case GD_GREEN:
        //        break;
        //    case GD_BLUE:
        //        if (ply_head->next->x>0&&ply_head->next->x<8)
        //        {
        //            //int x=ply_head->next->x;
        //            //int y=ply_head->next->y;
        //            //pos2d tpwall1,tmpwall2;
        //            //tpwall1.x=x;
        //            //tpwall1.y=y+1;
        //            //tpwall2.x=x+1;
        //            //tpwall2.y=y;
        //            //wall_vec.push_back()
        //            //gameData[][]=GD_WALL;
        //        }
        //        break;
        //    }
        //}
        // ��һλ���
        ply_head=ply_head->next;
    }   // ������ǵ��ԣ�ʲô������
}

// �˺�����ע��������AI����Ϊ��һ�˳ơ��ҡ����������ǵ���
void Quoridor_ComputerAI::AI_action()
{
    // ���ж�Ӧ������㱣֤
    //if (ply_head->id==ID_COMPUTER)
    //{
    //}
    // �ȼ�¼һ���Լ���˭
    int mycolor = ply_head->color;
    // ��¼�Լ���ǰ��λ��
    pos2d mypos;
    mypos.x = ply_head->x*2;
    mypos.y = ply_head->y*2;
    // �����Լ���ǰ����·������
    int mybestlen = TestBestPath(mypos, mycolor);
    // ��¼�Լ�������·���߷�
    std::vector<pos2d> mybestpath = best_path;
    // ��¼���·���ĳ��Ⱥ����·���Ķ���
    int shortestpathlen = mybestlen;
    //int shortestcolor = mycolor;
    std::vector<pos2d> shortestenemypath;
    // ����һ�������ѯָ��
    player* tmp_ply_head = ply_head;
    // ѭ�������������ֵ�����·���Լ�����
    while (tmp_ply_head->next->color != mycolor)
    {
        // �Ƚ�ָ�������ƶ�һ��
        tmp_ply_head = tmp_ply_head->next;
        pos2d tmppos;
        tmppos.x = tmp_ply_head->x*2;
        tmppos.y = tmp_ply_head->y*2;
        int tmplen = TestBestPath(tmppos, tmp_ply_head->color);
        if (tmplen < shortestpathlen)
        {
            shortestpathlen = tmplen;
            //shortestcolor = tmp_ply_head->color;
            shortestenemypath = best_path;
        }
    }
    // ������˵�·�����Ҹ��ţ������������п��õ�ǽ����ô�ͷ�ǽ������
    if (shortestpathlen < mybestlen && ply_head->wall_num_left>0)
    {
        int x0 = shortestenemypath[0].x;
        int y0 = shortestenemypath[0].y;
        int x1 = shortestenemypath[1].x;
        int y1 = shortestenemypath[1].y;
        // ���������һ��Ҫ�ߵ�λ�ò������������ռ
        if (gameData[x1][y1] == GD_BLANK)
        {
            // �������ֱ��������ߣ���ô��Ҫ��һ��ˮƽ�����ǽ
            // ���ǵ�x������ͬ
            if (x0 == x1)
            {
                // Ҫ��ǽ����һ��Y�����ǹ̶���
                int tmpwally = (y0+y1)/2;

                // �����൱��������Ȼ�ҵ����·������ǰ·���ϣ��϶�������ǽ
                // �൱�������ţ�gameData[x0][tmpwally] == GD_BLANK
                // ���ȣ��ж�����λ�ÿ��Է�ǽ
                if (x0+2<GDSIZE && gameData[x0+1][tmpwally] == GD_BLANK 
                                && gameData[x0+2][tmpwally] == GD_BLANK)
                {
                    // ���ж�����λ���Ƿ����
                    if (x0-2 >= 0 && gameData[x0-1][tmpwally] == GD_BLANK 
                                  && gameData[x0-2][tmpwally] == GD_BLANK)
                    {
                        // ������߶����Է�ǽ����ô���ѡ��һ������
                        // ���溯������Ծ��ȵĲ���0��1
                        if (random(0,2.0))
                        {
                            goto AI_WALL_RIGHT;
                        }
                        else
                        {
                            goto AI_WALL_LEFT;
                        }
                    }
                    // �����ʾֻ�������ҵ�λ�÷�ǽ
                    else
                    {
AI_WALL_RIGHT:
                        // ����ǽ
                        // ע�⣬������δ��룬�Ƿ���һ������ǽ
                        pos2d wall1;
                        wall1.x = x0;
                        wall1.y = tmpwally;
                        pos2d wall2;
                        wall2.x = x0+2;
                        wall2.y = tmpwally;
                        // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                        wall_vec.push_back(wall1);
                        wall_vec.push_back(wall2);
                        // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                        gameData[wall1.x][wall1.y]=GD_WALL;
                        gameData[wall1.x+1][wall1.y]=GD_WALL;
                        gameData[wall2.x][wall2.y]=GD_WALL;

                        // ����·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬
                        // ��ô��ǽ�Ƿ�����Ҫ��ԭԭ������Ϸ����
                        if (!pgm->judgeWallLegal())
                        {
                            // �ָ�ǽ����Ϊ��
                            gameData[wall1.x][wall1.y]=GD_BLANK;
                            gameData[wall1.x+1][wall1.y]=GD_BLANK;
                            gameData[wall2.x][wall2.y]=GD_BLANK;
                            // �����ո�ѹ���ǽ���ƶ���
                            wall_vec.pop_back();
                            wall_vec.pop_back();

                            // TODO , ������������߼����㷨���ڱ�ĵط����ǽ
                            goto AI_HAVE_TO_MOVE;
                        }

                        ply_head->wall_num_left--;
                    }
                }
                // ��Σ������ʾ���Ҳ����ԣ����ж������Ƿ����
                else if (x0-2 >= 0 && gameData[x0-1][tmpwally] == GD_BLANK
                                   && gameData[x0-2][tmpwally] == GD_BLANK)
                {
AI_WALL_LEFT:
                    // ����ǽ
                    // ע�⣬������δ��룬�Ƿ���һ������ǽ
                    pos2d wall1;
                    wall1.x = x0-2;
                    wall1.y = tmpwally;
                    pos2d wall2;
                    wall2.x = x0;
                    wall2.y = tmpwally;
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(wall1);
                    wall_vec.push_back(wall2);
                    // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                    gameData[wall1.x][wall1.y]=GD_WALL;
                    gameData[wall1.x+1][wall1.y]=GD_WALL;
                    gameData[wall2.x][wall2.y]=GD_WALL;

                    // ����·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬
                    // ��ô��ǽ�Ƿ�����Ҫ��ԭԭ������Ϸ����
                    if (!pgm->judgeWallLegal())
                    {
                        // �ָ�ǽ����Ϊ��
                        gameData[wall1.x][wall1.y]=GD_BLANK;
                        gameData[wall1.x+1][wall1.y]=GD_BLANK;
                        gameData[wall2.x][wall2.y]=GD_BLANK;
                        // �����ո�ѹ���ǽ���ƶ���
                        wall_vec.pop_back();
                        wall_vec.pop_back();

                        // TODO , ������������߼����㷨���ڱ�ĵط����ǽ
                        goto AI_HAVE_TO_MOVE;
                    }

                    ply_head->wall_num_left--;
                }
                // �������Է�ǽ����ô�Լ���һ��
                else 
                {
                    goto AI_HAVE_TO_MOVE;
                }
            }
            // �����ˮƽ��������ߣ���ô��Ҫ��һ����ֱ�����ǽ
            // ���ǵ�y������ͬ
            else if (y0 == y1)
            {
                // Ҫ��ǽ����һ��X�����ǹ̶���
                int tmpwallx = (x0+x1)/2;

                // �����൱��������Ȼ�ҵ����·������ǰ·���ϣ��϶�������ǽ
                // �൱�������ţ�gameData[tmpwallx][y0] == GD_BLANK
                // ���ȣ��ж�����λ���Ƿ���Է�ǽ
                if (y0+2<GDSIZE && gameData[tmpwallx][y0+1] == GD_BLANK 
                                && gameData[tmpwallx][y0+2] == GD_BLANK)
                {
                    // ���ж�����λ���Ƿ����
                    if (y0-2 >= 0 && gameData[tmpwallx][y0-1] == GD_BLANK 
                                  && gameData[tmpwallx][y0-2] == GD_BLANK)
                    {
                        // ������߶����Է�ǽ����ô���ѡ��һ������
                        // ���溯������Ծ��ȵĲ���0��1
                        if (random(0,2.0))
                        {
                            goto AI_WALL_UP;
                        }
                        else
                        {
                            goto AI_WALL_DOWN;
                        }
                    }
                    // �����ʾֻ�������ϵ�λ�÷�ǽ
                    else
                    {
AI_WALL_UP:
                        // ����ǽ
                        // ע�⣬������δ��룬�Ƿ���һ������ǽ
                        pos2d wall1;
                        wall1.x = tmpwallx;
                        wall1.y = y0;
                        pos2d wall2;
                        wall2.x = tmpwallx;
                        wall2.y = y0+2;
                        // ѹ��ǽ���ƶ���,һ����ѹ���±ߵĿ�
                        wall_vec.push_back(wall1);
                        wall_vec.push_back(wall2);
                        // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                        gameData[wall1.x][wall1.y]=GD_WALL;
                        gameData[wall1.x][wall1.y+1]=GD_WALL;
                        gameData[wall1.x][wall2.y]=GD_WALL;

                        // ����·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬
                        // ��ô��ǽ�Ƿ�����Ҫ��ԭԭ������Ϸ����
                        if (!pgm->judgeWallLegal())
                        {
                            // �ָ�ǽ����Ϊ��
                            gameData[wall1.x][wall1.y]=GD_BLANK;
                            gameData[wall1.x][wall1.y+1]=GD_BLANK;
                            gameData[wall2.x][wall2.y]=GD_BLANK;
                            // �����ո�ѹ���ǽ���ƶ���
                            wall_vec.pop_back();
                            wall_vec.pop_back();

                            // TODO , ������������߼����㷨���ڱ�ĵط����ǽ
                            goto AI_HAVE_TO_MOVE;
                        }

                        ply_head->wall_num_left--;
                    }
                }
                // ��Σ������ʾ���Ҳ����ԣ����ж������Ƿ����
                else if (y0-2 >= 0 && gameData[tmpwallx][y0-1] == GD_BLANK 
                                   && gameData[tmpwallx][y0-2] == GD_BLANK)
                {
AI_WALL_DOWN:
                    // ����ǽ
                    // ע�⣬������δ��룬�Ƿ���һ������ǽ
                    pos2d wall1;
                    wall1.x = tmpwallx;
                    wall1.y = y0-2;
                    pos2d wall2;
                    wall2.x = tmpwallx;
                    wall2.y = y0;
                    // ѹ��ǽ���ƶ���,һ����ѹ����ߵĿ�
                    wall_vec.push_back(wall1);
                    wall_vec.push_back(wall2);
                    // ������Ϸ�㷨����,����ע�⣬��������ǽλ�õ��м����Ӵ�Ҳ��ֵ
                    gameData[wall1.x][wall1.y]=GD_WALL;
                    gameData[wall1.x][wall1.y+1]=GD_WALL;
                    gameData[wall2.x][wall2.y]=GD_WALL;

                    // ����·ŵ�һ��ǽ��ʹ���κ�һ������޽⣬
                    // ��ô��ǽ�Ƿ�����Ҫ��ԭԭ������Ϸ����
                    if (!pgm->judgeWallLegal())
                    {
                        // �ָ�ǽ����Ϊ��
                        gameData[wall1.x][wall1.y]=GD_BLANK;
                        gameData[wall1.x][wall1.y+1]=GD_BLANK;
                        gameData[wall2.x][wall2.y]=GD_BLANK;
                        // �����ո�ѹ���ǽ���ƶ���
                        wall_vec.pop_back();
                        wall_vec.pop_back();

                        // TODO , ������������߼����㷨���ڱ�ĵط����ǽ
                        goto AI_HAVE_TO_MOVE;
                    }

                    ply_head->wall_num_left--;
                }
                // �������Է�ǽ����ô�Լ���һ��
                else 
                {
                    goto AI_HAVE_TO_MOVE;
                }
            }
            else
            {
                MessageBox(NULL,"�����ǲ����ܷ����ģ�����·������б��·��", "Quoridor_ComputerAI",MB_OK);
                exit(-1);
            }
        }
        // �����һ��Ҫ�ߵĵط���һ��������ռ��
        // ��ô����Ҫ����һ����һ��Ҫ��λ�õĹ�ϵ�������ߵ�λ��������·���Ľ���
        else
        {
            //TODO
            goto AI_HAVE_TO_MOVE;
        }
    }
    // ����ҵ�·������̵ģ���ô�Ҿ���ǰ��һ��
    else
    {
AI_HAVE_TO_MOVE:
        // �Ȼ�ȡ�ҵ�ǰ���ߵĵ�
        pgm->playerMovablePos(mypos);
        // �Ƿ��ҵ��غϵ㣬����˫ѭ����־
        int dloopjump = 0;
        pos2d targ;
        // ��������λ������û��������·�����ص���
        for (size_t i=0; i<mybestpath.size();i++)
        {
            // �����ж����Լ�����·����ֻ��ǰ�ĸ���
            // ��Ϊ����Զ�ģ�Ӧ�ò����ڿ��߷�Χ��
            for (size_t j=0; j<pgm->preselect_pos.size(); j++)
            {
                // �ҵ�����·���������λ�õ���ͬ�㣬����˫ѭ��
                if (pgm->preselect_pos[j] == mybestpath[i])
                {
                    targ = mybestpath[i];
                    dloopjump = 1;
                    break;
                }
            }
            if (dloopjump)
            {
                break;
            }
        }
        // �������·��û�������·���غϣ���ô���ѡȡ���ߵ㣬��Ϊ��һ������
        if (dloopjump != 1)
        {
            targ=pgm->preselect_pos[random(0,pgm->preselect_pos.size())];
        }
        // ������������뵽�������Ӵ���׶�
        char tmp=0;
        // ����
        tmp=gameData[targ.x][targ.y];
        gameData[targ.x][targ.y]=gameData[ply_head->x*2][ply_head->y*2];
        gameData[ply_head->x*2][ply_head->y*2]=tmp;

        ply_head->x=targ.x/2;
        ply_head->y=targ.y/2;

        pgm->preselect_pos.clear();
        // ��Ŀ��λ���ϣ�������ұ���
        switch (ply_head->color)
        {
        case GD_YELLOW:
            if (ply_head->x==8)
            {
                pgm->win_flag=GD_YELLOW;
                iGameState=GAME_WIN;
            }
            break;
        case GD_RED:
            if (ply_head->y==0)
            {
                pgm->win_flag=GD_RED;
                iGameState=GAME_WIN;
            }
            break;
        case GD_GREEN:
            if (ply_head->x==0)
            {
                pgm->win_flag=GD_GREEN;
                iGameState=GAME_WIN;
            }
            break;
        case GD_BLUE:
            if (ply_head->y==8)
            {
                pgm->win_flag=GD_BLUE;
                iGameState=GAME_WIN;
            }
            break;
        default:
            break;
        }
    }
    // ��һλ���
    ply_head=ply_head->next;
}

// �˺���start�������ǵ�ǰѡȡ��λ�ã�����ѡȡ����ҵ�λ��
// end��������ѡȡ�������ҵ���ɫ����Ϊ��ҵ���ɫ��ͬ��Ԥ���յ��λ�þͲ�ͬ
int Quoridor_ComputerAI::TestBestPath(pos2d start, int end)
{
    AStarAlg aa;
    size_t last_path_length=999;
    int ret = 0;
    std::vector<pos2d> tmppath;

    switch (end)
    {
    case GD_YELLOW:
        // ����Ŀ����յ㲻��ȷ����һ������Ҫ�������б˰�һ���յ�
        for (int i = GDSIZE-1;i>=0;i-=2)
        {
            best_path.swap(std::vector<pos2d> ());
            aa.Init(gameData,start.x,start.y,GDSIZE-1,i);
            ret = aa.A_start_calculate();
            if (ret ==0 && best_path.size()<last_path_length)
            {
                last_path_length = best_path.size();
                tmppath=best_path;
            }
        }
        break;
    case GD_RED:
        for (int i = 0;i<GDSIZE;i+=2)
        {
            best_path.swap(std::vector<pos2d> ());
            aa.Init(gameData,start.x,start.y,i,0);
            ret = aa.A_start_calculate();
            if (ret ==0 && best_path.size()<last_path_length)
            {
                last_path_length = best_path.size();
                tmppath=best_path;
            }
        }
        break;
    case GD_GREEN:
        for (int i = GDSIZE-1;i>=0;i-=2)
        {
            best_path.swap(std::vector<pos2d> ());
            aa.Init(gameData,start.x,start.y,0,i);
            ret = aa.A_start_calculate();
            if (ret ==0 && best_path.size()<last_path_length)
            {
                last_path_length = best_path.size();
                tmppath=best_path;
            }
        }
        break;
    case GD_BLUE:
        for (int i = 0;i<GDSIZE;i+=2)
        {
            best_path.swap(std::vector<pos2d> ());
            aa.Init(gameData,start.x,start.y,i,GDSIZE-1);
            ret = aa.A_start_calculate();
            if (ret ==0 && best_path.size()<last_path_length)
            {
                last_path_length = best_path.size();
                tmppath=best_path;
            }
        }
        break;
    default:
        break;
    }

    best_path = tmppath;
    // ��������·������
    return last_path_length;
}
