#include "stdafx.h"
#include "A_star_alg.h"

AStarAlg::AStarAlg(void)
{
    memset(map_maze,0,sizeof(map_maze));
    memset(open_table,0,sizeof(open_table));
    memset(close_table,0,sizeof(close_table));
    memset(path_stack,0,sizeof(path_stack));
    open_node_count=0;        // open表中节点数量
    close_node_count=0;       // close表中结点数量
    top = -1;                 // 栈顶
    start_node = NULL;
    end_node = NULL;
}

AStarAlg::~AStarAlg(void)
{
}

void AStarAlg::Init(volatile char gameData[][GDSIZE],int start_x, int start_y,int end_x,int end_y)
{
    memset(map_maze,0,sizeof(map_maze));
    memset(open_table,0,sizeof(open_table));
    memset(close_table,0,sizeof(close_table));
    memset(path_stack,0,sizeof(path_stack));
    open_node_count=0;        // open表中节点数量
    close_node_count=0;       // close表中结点数量
    top = -1;                 // 栈顶
    start_node = NULL;
    end_node = NULL;

    for( int i = 0; i < GDSIZE; ++i )
    {
        for ( int j = 0; j < GDSIZE; ++j )
        {
            map_maze[i][j].s_g = 0;
            map_maze[i][j].s_h = 0;
            map_maze[i][j].s_is_in_closetable = 0;
            map_maze[i][j].s_is_in_opentable = 0;
            // 这里根据实际情况进行数据意义的转换
            //map_maze[i][j].s_style = gameData[i][j];
            if (gameData[i][j]!=0 || (i%2==1&&j%2==1))
            {
                map_maze[i][j].s_style = BARRIER;
            } else {
                map_maze[i][j].s_style = BLANKNODE;
            }
            map_maze[i][j].s_x = i;
            map_maze[i][j].s_y = j;
            map_maze[i][j].s_parent = NULL;
        }
    }
    // 设置起点与终点
    // 起点
    map_maze[start_x][start_y].s_style = STARTNODE;
    start_node = &(map_maze[start_x][start_y]);
    // 终点
    map_maze[end_x][end_y].s_style = ENDNODE;
    end_node = &(map_maze[end_x][end_y]);
}

void AStarAlg::swap_node( int idx1, int idx2 )
{
    pAStarNode tmp = open_table[idx1];
    open_table[idx1] = open_table[idx2];
    open_table[idx2] = tmp;
}

void AStarAlg::adjust_heap( int nIndex )
{
    int curr = nIndex;
    int child = curr * 2 + 1;   // 得到左孩子idx( 下标从0开始，所有做孩子是curr*2+1 )
    int parent = ( curr - 1 ) / 2;  // 得到双亲idx

    if (nIndex < 0 || nIndex >= open_node_count)
    {
        return;
    }

    // 从上往下调整( 要比较左右孩子和cur parent )
    while ( child < open_node_count )
    {
        // 小根堆是双亲值小于孩子值
        //
        if ( child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h  > open_table[child+1]->s_g + open_table[child+1]->s_h )
        {
            ++child;    // 判断左右孩子大小
        }

        if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
        {
            break;
        }
        else
        {
            swap_node( child, curr );           // 交换节点
            curr = child;               // 再判断当前孩子节点
            child = curr * 2 + 1;           // 再判断左孩子
        }
    }

    if (curr != nIndex)
    {
        return;
    }

    // 往上调整( 只需要比较cuur child和parent )
    //
    while (curr != 0)
    {
        if (open_table[curr]->s_g + open_table[curr]->s_h >= open_table[parent]->s_g + open_table[parent]->s_h)
        {
            break;
        }
        else
        {
            swap_node( curr, parent );
            curr = parent;
            parent = (curr-1)/2;
        }
    }

}

void AStarAlg::insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w )
{
    int i;

    if ( map_maze[x][y].s_style != BARRIER )        // 不是障碍物
    {
        if ( !map_maze[x][y].s_is_in_closetable )   // 不在闭表中
        {
            if ( map_maze[x][y].s_is_in_opentable ) // 在open表中
            {
                // 需要判断是否是一条更优化的路径
                if ( map_maze[x][y].s_g > curr_node->s_g + w )  // 如果更优化
                {
                    map_maze[x][y].s_g = curr_node->s_g + w;
                    map_maze[x][y].s_parent = curr_node;
                    // 在open表中，搜索这个节点
                    for ( i = 0; i < open_node_count; ++i )
                    {
                        if ( open_table[i]->s_x == map_maze[x][y].s_x && open_table[i]->s_y == map_maze[x][y].s_y )
                        {
                            break;
                        }
                    }
                    adjust_heap( i );                   // 下面调整点
                }
            }
            else                                    // 不在open中
            {
                map_maze[x][y].s_g = curr_node->s_g + w;
                map_maze[x][y].s_h = 10*(abs(end_node->s_x - x ) + abs(end_node->s_y - y));
                map_maze[x][y].s_parent = curr_node;
                map_maze[x][y].s_is_in_opentable = 1;
                open_table[open_node_count++] = &(map_maze[x][y]);

                adjust_heap(open_node_count-1);                 // 下面调整点
            }
        }
    }

}

void AStarAlg::get_neighbors( pAStarNode curr_node, pAStarNode end_node )
{
    int x = curr_node->s_x;
    int y = curr_node->s_y;

    // 下面对于8个邻居进行处理！
    if ( ( x + 1 ) >= 0 && ( x + 1 ) < GDSIZE && y >= 0 && y < GDSIZE )
    {
        insert_to_opentable( x+1, y, curr_node, end_node, GDSIZE );
    }
    if ( ( x - 1 ) >= 0 && ( x - 1 ) < GDSIZE && y >= 0 && y < GDSIZE )
    {
        insert_to_opentable( x-1, y, curr_node, end_node, GDSIZE );
    }
    if ( x >= 0 && x < GDSIZE && ( y + 1 ) >= 0 && ( y + 1 ) < GDSIZE )
    {
        insert_to_opentable( x, y+1, curr_node, end_node, GDSIZE );
    }
    if ( x >= 0 && x < GDSIZE && ( y - 1 ) >= 0 && ( y - 1 ) < GDSIZE )
    {
        insert_to_opentable( x, y-1, curr_node, end_node, GDSIZE );
    }
    // 调整堆
    adjust_heap( 0 );
}

// 计算最优路径函数
// 在调用前，需要执行init来设定起点与终点
int AStarAlg::A_start_calculate()
{
    AStarNode *curr_node;           // 当前点
    int       is_found;         // 是否找到路径
    // 1 .首先指定起点
    open_table[open_node_count++] = start_node;         // 起始点加入open表

    start_node->s_is_in_opentable = 1;              // 加入open表
    start_node->s_g = 0;
    start_node->s_h = 10*(abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y));
    start_node->s_parent = NULL;

    if ( start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y )
    {
        printf("起点==终点！\n");
        return -1;
    }
    is_found = 0;
    // 2. 开始循环处理
    while( 1 )
    {
        curr_node = open_table[0];      // open表的第一个点一定是f值最小的点(通过堆排序得到的)
        open_table[0] = open_table[--open_node_count];  // 最后一个点放到第一个点，然后进行堆调整
        // 变了头上的一个从上往下调
        adjust_heap( 0 );               // 调整堆

        close_table[close_node_count++] = curr_node;    // 当前点加入close表
        curr_node->s_is_in_closetable = 1;      // 已经在close表中了
        // 严格意义上来说，这里应该把在open表标志置为0，相当于从open表中取出此元素
        curr_node->s_is_in_opentable = 0;       // 已经在close表中了

        if ( curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y )// 终点在close中，结束
        {
            is_found = 1;
            break;
        }

        get_neighbors( curr_node, end_node );           // 对邻居的处理

        if ( open_node_count == 0 )             // 没有路径到达
        {
            is_found = 0;
            break;
        }
    }

    if ( is_found )
    {
        // 这行加不加都行
        //curr_node = end_node;

        while( curr_node )
        {
            path_stack[++top] = curr_node;
            curr_node = curr_node->s_parent;
        }

        get_path();
        return 0;

        //while( top >= 0 )       // 下面是输出路径看看~
        //{
        //    if ( top > 0 )
        //    {
        //        printf("(%d,%d)-->", path_stack[top]->s_x, path_stack[top]->s_y);
        //        top--;
        //    }
        //    else
        //    {
        //        printf("(%d,%d)", path_stack[top]->s_x, path_stack[top]->s_y);
        //        break;
        //    }
        //}
    }
    else
    {
        //printf("么有找到路径\n");
        return -1;
    }

}

// 这个函数目前是为Quoridor专写，只把玩家可走的点存储下来
void AStarAlg::get_path()
{
    pos2d tmp2d;
    while( top > 0 )
    {
        tmp2d.x = path_stack[top]->s_x;
        tmp2d.y = path_stack[top]->s_y;
        // 只把玩家能走的点存储下来
        if (tmp2d.x%2==0 && tmp2d.y%2==0)
        {
            best_path.push_back(tmp2d);
        }
        top--;
    }
    if (top == 0)
    {
        tmp2d.x = path_stack[top]->s_x;
        tmp2d.y = path_stack[top]->s_y;
        // 只把玩家能走的点存储下来
        if (tmp2d.x%2==0 && tmp2d.y%2==0)
        {
            best_path.push_back(tmp2d);
        }
    }
}
