// modify by cgm 2015-8-21
// �޸��������ȡ��һ�γ���
// A��·�������㷨

#include <stdio.h>
#include <stdlib.h>

#define BLANKNODE	0
#define STARTNODE	1
#define ENDNODE		2
#define BARRIER		3

#define __linux__
//#define __eight_direction_enable__
//#define __debug_output_opentable__

typedef struct AStarNode
{
	int s_x;	// ����(�������·����Ҫ)
	int s_y;
	int s_g;	// ��㵽�˵�ľ���( ��g��h���Եõ�f���˴�fʡ�ԣ�f=g+h )
	int	s_h;	// ��������Ԥ��Ĵ˵㵽�յ�ľ���
	int s_style;// ������ͣ���ʼ�㣬�յ㣬�ϰ���
	int s_is_in_closetable;			// �Ƿ���close����
	int s_is_in_opentable;			// �Ƿ���open����
	struct AStarNode * s_parent;	// ���ڵ�
}AStarNode, *pAStarNode;

AStarNode  map_maze[10][10];	// �������
pAStarNode open_table[100];		// open��
pAStarNode close_table[100];	// close��
pAStarNode path_stack[100];		// ����·����ջ
int   open_node_count=0;		// open���нڵ�����
int	  close_node_count=0;		// close���н������
int   top = -1;					// ջ��

// ��������Ԫ��
void swap_node( int idx1, int idx2 )  
{  
	pAStarNode tmp = open_table[idx1];
	open_table[idx1] = open_table[idx2];
	open_table[idx2] = tmp;
}  

// �ѵ���
void adjust_heap( int /*i*/nIndex )    
{ 	
	int curr = nIndex;
	int child = curr * 2 + 1;	// �õ�����idx( �±��0��ʼ��������������curr*2+1 )
	int parent = ( curr - 1 ) / 2;	// �õ�˫��idx

	if (nIndex < 0 || nIndex >= open_node_count)
	{
		return;
	}
	
	// �������µ���( Ҫ�Ƚ����Һ��Ӻ�cuur parent )
	// 
	while ( child < open_node_count )
	{
		// С������˫��ֵС�ں���ֵ
		// 
		if ( child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h  > open_table[child+1]->s_g + open_table[child+1]->s_h )
		{
			++child;	// �ж����Һ��Ӵ�С
		}
		
		if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
		{
			break;
		}
		else
		{
			swap_node( child, curr );			// �����ڵ�
			curr = child;				// ���жϵ�ǰ���ӽڵ�
			child = curr * 2 + 1;			// ���ж�����
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

// �ж��ھӵ��Ƿ���Խ���open��
void insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w )
{
	int i;

	if ( map_maze[x][y].s_style != BARRIER )		// �����ϰ���
	{
		if ( !map_maze[x][y].s_is_in_closetable )	// ���ڱձ���
		{
			if ( map_maze[x][y].s_is_in_opentable )	// ��open����
			{
				// ��Ҫ�ж��Ƿ���һ�����Ż���·��
				if ( map_maze[x][y].s_g > curr_node->s_g + w )	// ������Ż�
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
	printf("have better way!!!!\n");
					adjust_heap( i );					// ���������
				}
			}
			else									// ����open��
			{
				map_maze[x][y].s_g = curr_node->s_g + w;
				map_maze[x][y].s_h = 10*(abs(end_node->s_x - x ) + abs(end_node->s_y - y));
				map_maze[x][y].s_parent = curr_node;
				map_maze[x][y].s_is_in_opentable = 1;
				open_table[open_node_count++] = &(map_maze[x][y]);

				adjust_heap(open_node_count-1);					// ���������
			}
		}
	}
}

// �����ھ�
// ����������8���ھӽ��в���
// ����ע���ͼ�߽�ֵ
void get_neighbors( pAStarNode curr_node, pAStarNode end_node )
{
	int x = curr_node->s_x;
	int y = curr_node->s_y;

	// �������8���ھӽ��д�����
	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && y >= 0 && y < 10 )
	{
		insert_to_opentable( x+1, y, curr_node, end_node, 10 );
	}

	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && y >= 0 && y < 10 )
	{
		insert_to_opentable( x-1, y, curr_node, end_node, 10 );
	}

	if ( x >= 0 && x < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x, y+1, curr_node, end_node, 10 );
	}

	if ( x >= 0 && x < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x, y-1, curr_node, end_node, 10 );
	}

#ifdef __eight_direction_enable__
	// �Ƿ��ǰ˷����㷨
	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x+1, y+1, curr_node, end_node, 14 );
	}

	if ( ( x + 1 ) >= 0 && ( x + 1 ) < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x+1, y-1, curr_node, end_node, 14 );
	}

	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && ( y + 1 ) >= 0 && ( y + 1 ) < 10 )
	{
		insert_to_opentable( x-1, y+1, curr_node, end_node, 14 );
	}

	if ( ( x - 1 ) >= 0 && ( x - 1 ) < 10 && ( y - 1 ) >= 0 && ( y - 1 ) < 10 )
	{
		insert_to_opentable( x-1, y-1, curr_node, end_node, 14 );
	}
#endif
	// ������
	adjust_heap( 0 );
}

void print_maze_color()
{
	int i,j;
	printf("x\\y|0 1 2 3 4 5 6 7 8 9 \n");
	printf("------------------------\n");
	for( i = 0; i < 10; ++i )
	{
		printf(" %d |",i);
		for ( j = 0; j < 10; ++j )
		{
			if(map_maze[i][j].s_is_in_closetable == 1)
			{
				printf("\033[0;31m""%d ""\033[m", map_maze[i][j].s_style);
			}
			else if(map_maze[i][j].s_is_in_opentable == 1)
			{
				printf("\033[0;32m""%d ""\033[m", map_maze[i][j].s_style);
			}
			else
				printf("%d ", map_maze[i][j].s_style);

		}

		printf("\n");
	}
	printf("------------------------\n");
}

int main()
{ 
	// ��ͼ����Ķ���
	// 
	AStarNode *start_node;			// ��ʼ��
	AStarNode *end_node;			// ������
	AStarNode *curr_node;			// ��ǰ��
	int       is_found;			// �Ƿ��ҵ�·��
	int maze[][10] ={			// ����Ϊ�˺ø�ֵ��map_maze
		/*
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,3,0,0,0,0,0 },
						{ 0,0,1,0,3,0,2,0,0,0 },
						{ 0,0,0,0,3,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
				*/

						{ 1,0,0,3,0,3,0,0,0,0 },
						{ 0,0,3,0,0,3,0,3,0,3 },
						{ 3,0,0,0,0,3,3,3,0,3 },
						{ 3,0,3,0,0,0,0,0,0,3 },
						{ 3,0,0,2,0,3,0,0,0,3 },
						{ 3,0,0,3,0,0,0,3,0,3 },
						{ 3,0,0,0,0,3,3,0,0,0 },
						{ 0,0,0,0,0,0,0,0,0,0 },
						{ 3,3,3,0,0,3,0,3,0,3 },
						{ 3,0,0,0,0,3,3,3,0,3 },
					};
	int		  i,j,x;
	
	// ����׼����
	// 
	printf("x\\y|0 1 2 3 4 5 6 7 8 9 \n");
	printf("------------------------\n");
	for( i = 0; i < 10; ++i )
	{
		printf(" %d |",i);
		for ( j = 0; j < 10; ++j )
		{
			map_maze[i][j].s_g = 0;
			map_maze[i][j].s_h = 0;
			map_maze[i][j].s_is_in_closetable = 0;
			map_maze[i][j].s_is_in_opentable = 0;
			map_maze[i][j].s_style = maze[i][j];
			map_maze[i][j].s_x = i;
			map_maze[i][j].s_y = j;
			map_maze[i][j].s_parent = NULL;

			if ( map_maze[i][j].s_style == STARTNODE )	// ���
			{
				start_node = &(map_maze[i][j]);
			}
			else if( map_maze[i][j].s_style == ENDNODE )	// �յ�
			{
				end_node = &(map_maze[i][j]);
			}
#ifdef __linux__
			if(maze[i][j]==BARRIER) {
			printf("\033[44m%d \033[m", maze[i][j]);
			} else {
			printf("%d ", maze[i][j]);
			}
#else
			printf("%d ", maze[i][j]);
#endif
		}

		printf("\n");
	}
	printf("------------------------\n");

	// ����ʹ��A*�㷨�õ�·��
	// 	
	
	// 1 .����ָ�����
	open_table[open_node_count++] = start_node;			// ��ʼ�����open��
	
	start_node->s_is_in_opentable = 1;				// ����open��
	start_node->s_g = 0;
	start_node->s_h = 10*(abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y));
	start_node->s_parent = NULL;
	
	if ( start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y )
	{
		printf("���==�յ㣡\n");
		return 0;
	}
	
	is_found = 0;
	// 2. ��ʼѭ������
	while( 1 )
	{

		curr_node = open_table[0];		// open���ĵ�һ����һ����fֵ��С�ĵ�(ͨ��������õ���)
		open_table[0] = open_table[--open_node_count];	// ���һ����ŵ���һ���㣬Ȼ����жѵ���
		// ����ͷ�ϵ�һ���������µ�
		adjust_heap( 0 );				// ������
		
		close_table[close_node_count++] = curr_node;	// ��ǰ�����close��
		curr_node->s_is_in_closetable = 1;		// �Ѿ���close������
		// �ϸ���������˵������Ӧ�ð���open����־��Ϊ0���൱�ڴ�open����ȡ����Ԫ��
		curr_node->s_is_in_opentable = 0;		// �Ѿ���close������

		if ( curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y )// �յ���close�У�����
		{
			is_found = 1;
			break;
		}

		get_neighbors( curr_node, end_node );			// ���ھӵĴ���

		if ( open_node_count == 0 )				// û��·������
		{
			is_found = 0;
			break;
		}
#ifdef __debug_output_opentable__
		// for test
		printf("-------test output opentable-------\n");
		for ( x = 0; x < open_node_count; ++x )
		{
			//printf("(%d,%d):%d |", open_table[x]->s_x, open_table[x]->s_y, open_table[x]->s_g+open_table[x]->s_h);
			printf("(%d,%d):%d-(%d,%d) |", open_table[x]->s_x, open_table[x]->s_y, open_table[x]->s_g+open_table[x]->s_h,open_table[x]->s_parent->s_x,open_table[x]->s_parent->s_y);
		}
		printf("\n");

		print_maze_color();
		
		getchar();
#endif
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

		while( top >= 0 )		// ���������·������~
		{
			if ( top > 0 )
			{
				printf("(%d,%d)-->", path_stack[top]->s_x, path_stack[top]->s_y);
				top--;
			}
			else
			{
				printf("(%d,%d)", path_stack[top]->s_x, path_stack[top]->s_y);
				break;
			}
		}
	}
	else
	{
		printf("ô���ҵ�·��");
	}

	puts("");

	return 0;
}
