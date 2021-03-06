// modify by cgm 2015-8-21
// 修改自网络获取的一段程序
// A星路径搜索算法

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
	int s_x;	// 坐标(最终输出路径需要)
	int s_y;
	int s_g;	// 起点到此点的距离( 由g和h可以得到f，此处f省略，f=g+h )
	int	s_h;	// 启发函数预测的此点到终点的距离
	int s_style;// 结点类型：起始点，终点，障碍物
	int s_is_in_closetable;			// 是否在close表中
	int s_is_in_opentable;			// 是否在open表中
	struct AStarNode * s_parent;	// 父节点
}AStarNode, *pAStarNode;

AStarNode  map_maze[10][10];	// 结点数组
pAStarNode open_table[100];		// open表
pAStarNode close_table[100];	// close表
pAStarNode path_stack[100];		// 保存路径的栈
int   open_node_count=0;		// open表中节点数量
int	  close_node_count=0;		// close表中结点数量
int   top = -1;					// 栈顶

// 交换两个元素
void swap_node( int idx1, int idx2 )  
{  
	pAStarNode tmp = open_table[idx1];
	open_table[idx1] = open_table[idx2];
	open_table[idx2] = tmp;
}  

// 堆调整
void adjust_heap( int /*i*/nIndex )    
{ 	
	int curr = nIndex;
	int child = curr * 2 + 1;	// 得到左孩子idx( 下标从0开始，所有做孩子是curr*2+1 )
	int parent = ( curr - 1 ) / 2;	// 得到双亲idx

	if (nIndex < 0 || nIndex >= open_node_count)
	{
		return;
	}
	
	// 从上往下调整( 要比较左右孩子和cur parent )
	// 
	while ( child < open_node_count )
	{
		// 小根堆是双亲值小于孩子值
		// 
		if ( child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h  > open_table[child+1]->s_g + open_table[child+1]->s_h )
		{
			++child;	// 判断左右孩子大小
		}
		
		if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
		{
			break;
		}
		else
		{
			swap_node( child, curr );			// 交换节点
			curr = child;				// 再判断当前孩子节点
			child = curr * 2 + 1;			// 再判断左孩子
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

// 判断邻居点是否可以进入open表
void insert_to_opentable( int x, int y, pAStarNode curr_node, pAStarNode end_node, int w )
{
	int i;

	if ( map_maze[x][y].s_style != BARRIER )		// 不是障碍物
	{
		if ( !map_maze[x][y].s_is_in_closetable )	// 不在闭表中
		{
			if ( map_maze[x][y].s_is_in_opentable )	// 在open表中
			{
				// 需要判断是否是一条更优化的路径
				if ( map_maze[x][y].s_g > curr_node->s_g + w )	// 如果更优化
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
	printf("have better way!!!!\n");
					adjust_heap( i );					// 下面调整点
				}
			}
			else									// 不在open中
			{
				map_maze[x][y].s_g = curr_node->s_g + w;
				map_maze[x][y].s_h = 10*(abs(end_node->s_x - x ) + abs(end_node->s_y - y));
				map_maze[x][y].s_parent = curr_node;
				map_maze[x][y].s_is_in_opentable = 1;
				open_table[open_node_count++] = &(map_maze[x][y]);

				adjust_heap(open_node_count-1);					// 下面调整点
			}
		}
	}
}

// 查找邻居
// 对上下左右8个邻居进行查找
// 这里注意地图边界值
void get_neighbors( pAStarNode curr_node, pAStarNode end_node )
{
	int x = curr_node->s_x;
	int y = curr_node->s_y;

	// 下面对于8个邻居进行处理！
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
	// 是否是八方向算法
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
	// 调整堆
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
	// 地图数组的定义
	// 
	AStarNode *start_node;			// 起始点
	AStarNode *end_node;			// 结束点
	AStarNode *curr_node;			// 当前点
	int       is_found;			// 是否找到路径
	int maze[][10] ={			// 仅仅为了好赋值给map_maze
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
	
	// 下面准备点
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

			if ( map_maze[i][j].s_style == STARTNODE )	// 起点
			{
				start_node = &(map_maze[i][j]);
			}
			else if( map_maze[i][j].s_style == ENDNODE )	// 终点
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

	// 下面使用A*算法得到路径
	// 	
	
	// 1 .首先指定起点
	open_table[open_node_count++] = start_node;			// 起始点加入open表
	
	start_node->s_is_in_opentable = 1;				// 加入open表
	start_node->s_g = 0;
	start_node->s_h = 10*(abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y));
	start_node->s_parent = NULL;
	
	if ( start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y )
	{
		printf("起点==终点！\n");
		return 0;
	}
	
	is_found = 0;
	// 2. 开始循环处理
	while( 1 )
	{

		curr_node = open_table[0];		// open表的第一个点一定是f值最小的点(通过堆排序得到的)
		open_table[0] = open_table[--open_node_count];	// 最后一个点放到第一个点，然后进行堆调整
		// 变了头上的一个从上往下调
		adjust_heap( 0 );				// 调整堆
		
		close_table[close_node_count++] = curr_node;	// 当前点加入close表
		curr_node->s_is_in_closetable = 1;		// 已经在close表中了
		// 严格意义上来说，这里应该把在open表标志置为0，相当于从open表中取出此元素
		curr_node->s_is_in_opentable = 0;		// 已经在close表中了

		if ( curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y )// 终点在close中，结束
		{
			is_found = 1;
			break;
		}

		get_neighbors( curr_node, end_node );			// 对邻居的处理

		if ( open_node_count == 0 )				// 没有路径到达
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
		// 这行加不加都行
		//curr_node = end_node;
		
		while( curr_node )
		{
			path_stack[++top] = curr_node;
			curr_node = curr_node->s_parent;
		}

		while( top >= 0 )		// 下面是输出路径看看~
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
		printf("么有找到路径\n");
	}

	puts("");

	return 0;
}

