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

void Quoridor_ComputerAI::FoolAI()
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

// 此函数中注释描述以AI本身为第一人称，其他都是敌人
void Quoridor_ComputerAI::AI_action()
{
    // 此判断应该在外层保证
    //if (ply_head->id==ID_COMPUTER)
    //{
    //}
    int mycolor = -1;
    // 先记录一下自己是谁
    mycolor = ply_head->color;
    // 记录自己当前的位置
    pos2d mypos;
    mypos.x = ply_head->x*2;
    mypos.y = ply_head->y*2;
    // 计算自己当前最优路径长度
    int mybestlen = TestBestPath(mypos, mycolor);
    // 记录自己的最优路径走法
    std::vector<pos2d> mybestpath = best_path;
    // 记录最短路径的长度和最短路径的对手
    int shortestpathlen = mybestlen;
    //int shortestcolor = mycolor;
    std::vector<pos2d> shortestenemypath;
    // 保护一下玩家轮询指针
    player* tmp_ply_head = ply_head;
    // 循环计算其他对手的最优路径以及长度
    while (tmp_ply_head->next->color != mycolor)
    {
        // 先将指针往下移动一下
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
    // 如果别人的路径比我更优，并且我手里有可用的墙，那么就放墙挡对手
    if (shortestpathlen < mybestlen && ply_head->wall_num_left>0)
    {
        int x0 = shortestenemypath[0].x;
        int y0 = shortestenemypath[0].y;
        int x1 = shortestenemypath[1].x;
        int y1 = shortestenemypath[1].y;
        // 如果敌人下一步要走的位置不是其他玩家所占
        if (gameData[x1][y1] == GD_BLANK)
        {
            // 如果是竖直方向可行走，那么需要挡一个水平方向的墙
            // 它们的x坐标相同
            if (x0 == x1)
            {
                int tmpwally = (y0+y1)/2;
                //if (x0 == 0 && gameData[2][tmpwally] == GD_BLANK)
                //{
                //}
                // 这里相当于隐含既然找到这个路径，则当前路径上，肯定不存在墙
                if (x0+2<GDSIZE && gameData[x0+2][tmpwally] == GD_BLANK)
                {
                    pos2d wall1;
                    wall1.x = x0;
                    wall1.y = tmpwally;
                    pos2d wall2;
                    wall2.x = x0+2;
                    wall2.y = tmpwally;
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(wall1);
                    wall_vec.push_back(wall2);
                    // 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
                    gameData[wall1.x][wall1.y]=GD_WALL;
                    gameData[wall1.x+1][wall1.y]=GD_WALL;
                    gameData[wall2.x][wall2.y]=GD_WALL;

                    ply_head->wall_num_left--;
                }
            }
            // 如果是水平方向可行走，那么需要挡一个垂直方向的墙
            // 他们的y坐标相同
            else if (y0 == y1)
            {
                int tmpwallx = (x0+x1)/2;

                // 这里相当于隐含既然找到这个路径，则当前路径上，肯定不存在墙
                if (y0+2<GDSIZE && gameData[tmpwallx][y0+2] == GD_BLANK)
                {
                    pos2d wall1;
                    wall1.x = tmpwallx;
                    wall1.y = y0;
                    pos2d wall2;
                    wall2.x = tmpwallx;
                    wall2.y = y0+2;
                    // 压入墙绘制队列,一定先压入左边的块
                    wall_vec.push_back(wall1);
                    wall_vec.push_back(wall2);
                    // 更新游戏算法数据,这里注意，把相连两墙位置的中间连接处也赋值
                    gameData[wall1.x][wall1.y]=GD_WALL;
                    gameData[wall1.x][wall1.y+1]=GD_WALL;
                    gameData[wall1.x][wall2.y]=GD_WALL;

                    ply_head->wall_num_left--;
                }
            }
            else
            {
                MessageBox(NULL,"这里是不可能发生的，最优路径存在斜向路径", "Quoridor_ComputerAI",MB_OK);
                exit(-1);
            }
        }
        // 如果下一步要走的地方被一个棋子所占有
        // 那么，需要讨论一下下一步要走位置的关系，可以走位与最优路径的交集
        else
        {
            //TODO
        }
    }
    // 如果我的路径是最短的，那么我就往前走一步
    else
    {
        // 先获取我当前可走的点
        pgm->playerMovablePos(mypos);
        // 是否找到重合点，跳出双循环标志
        int dloopjump = 0;
        pos2d targ;
        // 看看可走位置中有没有与最优路径是重叠的
        for (size_t i=0; i<mybestpath.size();i++)
        {
            // 这里判断我自己最优路径，只看前四个，
            // 因为，再远的，应该不会在可走范围内
            for (size_t j=0; j<pgm->preselect_pos.size(); j++)
            {
                // 找到最优路径中与可走位置的相同点，跳出双循环
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
        if (dloopjump)
        {
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
        {
            MessageBox(NULL,"AI算法中，没有找到最优路径和可走位置的匹配关系", "Quoridor_ComputerAI",MB_OK);
            exit(-1);
        }
    }
    // 下一位玩家
    ply_head=ply_head->next;
}

// 此函数start参数，是当前选取的位置，就是选取的玩家的位置
// end参数，是选取的这个玩家的颜色，因为玩家的颜色不同，预期终点的位置就不同
int Quoridor_ComputerAI::TestBestPath(pos2d start, int end)
{
    AStarAlg aa;
    size_t last_path_length=999;
    int ret = 0;
    std::vector<pos2d> tmppath;

    switch (end)
    {
    case GD_YELLOW:
        // 由于目标的终点不是确定的一个，需要遍历所有彼岸一排终点
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
    // 返回最优路径长度
    return last_path_length;
}
