#include "stdafx.h"
#include "A_star_alg.h"

AStarAlg::AStarAlg(void)
{
    memset(map_maze,0,sizeof(map_maze));
    memset(open_table,0,sizeof(open_table));
    memset(close_table,0,sizeof(close_table));
    memset(path_stack,0,sizeof(path_stack));
    open_node_count=0;        // open���нڵ�����
    close_node_count=0;       // close���н������
    top = -1;                 // ջ��
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
    open_node_count=0;        // open���нڵ�����
    close_node_count=0;       // close���н������
    top = -1;                 // ջ��
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
            // �������ʵ������������������ת��
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
    // ����������յ�
    // ���
    map_maze[start_x][start_y].s_style = STARTNODE;
    start_node = &(map_maze[start_x][start_y]);
    // �յ�
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
    int child = curr * 2 + 1;   // �õ�����idx( �±��0��ʼ��������������curr*2+1 )
    int parent = ( curr - 1 ) / 2;  // �õ�˫��idx

    if (nIndex < 0 || nIndex >= open_node_count)
    {
        return;
    }

    // �������µ���( Ҫ�Ƚ����Һ��Ӻ�cur parent )
    while ( child < open_node_count )
    {
        // С������˫��ֵС�ں���ֵ
        //
        if ( child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h  > open_table[child+1]->s_g + open_table[child+1]->s_h )
        {
            ++child;    // �ж����Һ��Ӵ�С
        }

        if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
        {
            break;
        }
        else
        {
            swap_node( child, curr );           // �����ڵ�
            curr = child;               // ���жϵ�ǰ���ӽڵ�
            child = curr * 2 + 1;           // ���ж�����
        }
    }

    if (curr != nIndex)
    {
        return;
    }

    // ���ϵ���( ֻ��Ҫ�Ƚ�cuur child��parent )
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

    if ( map_maze[x][y].s_style != BARRIER )        // �����ϰ���
    {
        if ( !map_maze[x][y].s_is_in_closetable )   // ���ڱձ���
        {
            if ( map_maze[x][y].s_is_in_opentable ) // ��open����
            {
                // ��Ҫ�ж��Ƿ���һ�����Ż���·��
                if ( map_maze[x][y].s_g > curr_node->s_g + w )  // ������Ż�
                {
                    map_maze[x][y].s_g = curr_node->s_g + w;
                    map_maze[x][y].s_parent = curr_node;
                    // ��open���У���������ڵ�
                    for ( i = 0; i < open_node_count; ++i )
                    {
                        if ( open_table[i]->s_x == map_maze[x][y].s_x && open_table[i]->s_y == map_maze[x][y].s_y )
                        {
                            break;
                        }
                    }
                    adjust_heap( i );                   // ���������
                }
            }
            else                                    // ����open��
            {
                map_maze[x][y].s_g = curr_node->s_g + w;
                map_maze[x][y].s_h = 10*(abs(end_node->s_x - x ) + abs(end_node->s_y - y));
                map_maze[x][y].s_parent = curr_node;
                map_maze[x][y].s_is_in_opentable = 1;
                open_table[open_node_count++] = &(map_maze[x][y]);

                adjust_heap(open_node_count-1);                 // ���������
            }
        }
    }

}

void AStarAlg::get_neighbors( pAStarNode curr_node, pAStarNode end_node )
{
    int x = curr_node->s_x;
    int y = curr_node->s_y;

    // �������8���ھӽ��д���
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
    // ������
    adjust_heap( 0 );
}

// ��������·������
// �ڵ���ǰ����Ҫִ��init���趨������յ�
int AStarAlg::A_start_calculate()
{
    AStarNode *curr_node;           // ��ǰ��
    int       is_found;         // �Ƿ��ҵ�·��
    // 1 .����ָ�����
    open_table[open_node_count++] = start_node;         // ��ʼ�����open��

    start_node->s_is_in_opentable = 1;              // ����open��
    start_node->s_g = 0;
    start_node->s_h = 10*(abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y));
    start_node->s_parent = NULL;

    if ( start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y )
    {
        printf("���==�յ㣡\n");
        return -1;
    }
    is_found = 0;
    // 2. ��ʼѭ������
    while( 1 )
    {
        curr_node = open_table[0];      // open��ĵ�һ����һ����fֵ��С�ĵ�(ͨ��������õ���)
        open_table[0] = open_table[--open_node_count];  // ���һ����ŵ���һ���㣬Ȼ����жѵ���
        // ����ͷ�ϵ�һ���������µ�
        adjust_heap( 0 );               // ������

        close_table[close_node_count++] = curr_node;    // ��ǰ�����close��
        curr_node->s_is_in_closetable = 1;      // �Ѿ���close������
        // �ϸ���������˵������Ӧ�ð���open���־��Ϊ0���൱�ڴ�open����ȡ����Ԫ��
        curr_node->s_is_in_opentable = 0;       // �Ѿ���close������

        if ( curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y )// �յ���close�У�����
        {
            is_found = 1;
            break;
        }

        get_neighbors( curr_node, end_node );           // ���ھӵĴ���

        if ( open_node_count == 0 )             // û��·������
        {
            is_found = 0;
            break;
        }
    }

    if ( is_found )
    {
        // ���мӲ��Ӷ���
        //curr_node = end_node;

        while( curr_node )
        {
            path_stack[++top] = curr_node;
            curr_node = curr_node->s_parent;
        }

        get_path();
        return 0;

        //while( top >= 0 )       // ���������·������~
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
        //printf("ô���ҵ�·��\n");
        return -1;
    }

}

// �������Ŀǰ��ΪQuoridorרд��ֻ����ҿ��ߵĵ�洢����
void AStarAlg::get_path()
{
    pos2d tmp2d;
    while( top > 0 )
    {
        tmp2d.x = path_stack[top]->s_x;
        tmp2d.y = path_stack[top]->s_y;
        // ֻ��������ߵĵ�洢����
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
        // ֻ��������ߵĵ�洢����
        if (tmp2d.x%2==0 && tmp2d.y%2==0)
        {
            best_path.push_back(tmp2d);
        }
    }
}
