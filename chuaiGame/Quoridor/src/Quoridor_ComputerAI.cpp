#include "stdafx.h"
#include "gData.h"
#include "Quoridor_ComputerAI.h"
#include "Quoridor_openGL.h"
#include "A_star_alg.h"

extern CQuoridor* pgm;

Quoridor_ComputerAI::Quoridor_ComputerAI(void)
{
}

Quoridor_ComputerAI::~Quoridor_ComputerAI(void)
{
}

void Quoridor_ComputerAI::SimpleComputer()
{
    // 只有当前玩家是电脑才做处理
    if (ply_head->id==ID_COMPUTER)
    {
        //Sleep(800);
        //if (random(0,2.0)==0)
        if (random(0,1.0)==0)
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
            tmppoint.x=ply_head->x*2;
            tmppoint.y=ply_head->y*2;
            pgm->playerMovablePos(tmppoint);
            pos2d targ;
            targ.x=-1;
            targ.y=-1;
            // 看看可走位置中有没有对玩家胜利有利的位置
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
                // 纯选随机点
                targ=pgm->preselect_pos[random(0,pgm->preselect_pos.size())];
            }
            // 这种情况，进入到人物棋子处理阶段
            char tmp=0;
            // 交换
            tmp=gameData[targ.x][targ.y];
            gameData[targ.x][targ.y]=gameData[ply_head->x*2][ply_head->y*2];
            gameData[ply_head->x*2][ply_head->y*2]=tmp;

            ply_head->x=targ.x/2;
            ply_head->y=targ.y/2;

            pgm->preselect_pos.clear();
            // 在目标位置上，更新玩家变量
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
        else
        {	// 在敌人玩家前面放墙
            switch (ply_head->next->color)
            {
            case GD_YELLOW:
                break;
            case GD_RED:
                break;
            case GD_GREEN:
                break;
            case GD_BLUE:
                if (ply_head->next->x>0&&ply_head->next->x<8)
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
        // 下一位玩家
        ply_head=ply_head->next;
    }   // 如果不是电脑，什么都不做
}

void Quoridor_ComputerAI::TestBestPath(pos2d start, int end)
{
    AStarAlg aa;
    size_t last_path_length=999;
    int ret = 0;
    std::vector<pos2d> tmppath;

    switch (end)
    {
    case GD_YELLOW:
        for (int i = GDSIZE-1;i>0;i-=2)
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
        break;
    case GD_GREEN:
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
}
