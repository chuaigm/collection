# coding by cgm
# 2017-10-18

# 1 - Import library
import pygame
#import math
import random
import string
from pygame.locals import *

# 2 - Read conf & set var
# if debug output info
is_debug=1
# 0:title, 1:original game, 2: advance game, 3: about, 4: exit
g_status=0
# 
class button(object):
    def __init__(self, desc, x, y, width=160, height=30):
        self.desc = desc
        self.x = x
        self.y = y
        self.width = width
        self.height = height
# button array
btn=[]

# param array
arr=[]
file = open("./cof/sss") 
while 1:
    line = file.readline()
    if not line:
        break
    arr.append(int(line))
if is_debug:
    print(arr[0])
# width, height = 800,600
if arr[0] == 2: 
    ww, hh = 1024, 200
#elif arr[0] == 3:
#    ww, hh = 1280, 800
else:
    ww, hh = 800, 600
#ww = string.atoi(arr[0])
#hh = string.atoi(arr[1])

# in pygame coordinate
# (0,0)-------------------> x
# |
# |
# |
# |
# V                   (800,600)
# y

#define buttons
btn.append(button("classic", ww-200, hh-250))  # 1, classic game button
btn.append(button("advance", ww-200, hh-200))  # 2, modify game button
btn.append(button(" about ", ww-200, hh-150))  # 3, about button
btn.append(button(" exit  ", ww-200, hh-100))  # 4, exit button
# keyboard detect
#        0     1      2      3      4      5      6
#        esc   enter  space  w      s      a      d
keys = [False, False, False, False, False, False, False]
# mouse position
ms_pos =[0,0]

class player_cls(object):
    def __init__(self, posi_id):
        self.posi_id = posi_id
# player obj
player=player_cls(1)

# define function
def init_game():
    player.posi_id=1

# print text in pygame draw
def print_text(tt, font_size, color, pos):
    font = pygame.font.Font("res/font/Droid Sans Mono.ttf", font_size)
    text = font.render(tt, True, color)
    screen.blit(text, pos)

# 3 - Load images
bg_title   = pygame.image.load("res/pic/bg_title.jpg")
pic_tank   = pygame.image.load("res/pic/tank.png")
pic_terrain= pygame.image.load("res/pic/terrain.png")
pic_mine   = pygame.image.load("res/pic/mine.png")
pic_grass  = pygame.image.load("res/pic/grass.png")
pic_home   = pygame.image.load("res/pic/home.png")

# zoom coefficient
z_coef=0.85
# in this game, a tank and terrain resource need to be 150x150 pixel
# TODO, make sure the res the same size
w_p,h_p = pic_terrain.get_size()  # 150x150
draw_unit=w_p # 150

p_tank=[]
for i in range(4):
    c=z_coef**i
    p_tank.append(pygame.transform.scale(pic_tank,(int(w_p*c),int(h_p*c))))

#    p3    p3
#   p2      p2
#  p1        p1
# p            p

p_terrain=[]
for i in range(4):
    c=z_coef**i
    p_terrain.append(pygame.transform.scale(pic_terrain,(int(w_p*c),int(h_p*c))))

p_mine=[]
for i in range(4):
    c=z_coef**i
    p_mine.append(pygame.transform.scale(pic_mine,(int(w_p*c),int(h_p*c))))

#here want to give draw terrian and player in the screen
x_shift=(ww-hh)/2
x_w=hh/4
y_h=hh/4
scale_shift=int(draw_unit*(1-z_coef))

# terrain is 150x150 pixel, in 800x600 screen
# player and terrain position array
#    -------------
#   12  13  14  15
#    8   9  10  11
#    4   5   6   7
#    0   1   2   3
#    -------------
draw_pos = []
draw_pos.append((x_shift,y_h*3))
draw_pos.append((x_shift+x_w,y_h*3))
draw_pos.append((x_shift+x_w*2,y_h*3))
draw_pos.append((x_shift+x_w*3,y_h*3))
draw_pos.append((x_shift+scale_shift*2,y_h*2+scale_shift))
draw_pos.append((x_shift+x_w+scale_shift,y_h*2+scale_shift))
draw_pos.append((x_shift+x_w*2,y_h*2+scale_shift))
draw_pos.append((x_shift+x_w*3-scale_shift,y_h*2+scale_shift))
draw_pos.append((x_shift+scale_shift*4,y_h+scale_shift*3))
draw_pos.append((x_shift+x_w+scale_shift*2,y_h+scale_shift*3))
draw_pos.append((x_shift+x_w*2,y_h+scale_shift*3))
draw_pos.append((x_shift+x_w*3-scale_shift*2,y_h+scale_shift*3))
draw_pos.append((x_shift+scale_shift*6,scale_shift*6))
draw_pos.append((x_shift+x_w+scale_shift*3,scale_shift*6))
draw_pos.append((x_shift+x_w*2,scale_shift*6))
draw_pos.append((x_shift+x_w*3-scale_shift*3,scale_shift*6))

#map scale
map_scale_x=8
map_scale_y=20
#tank in total map relative position, for draw
tank_in_map=[int(map_scale_x/2)-1,0]
#total map can only draw a small part in screen, the relative posion left bottem corner
ref_pos_in_map=[int(map_scale_x/2)-2,0]
#this sentence means scale_x is the outer loop, then in every x, has scale_y space
game_map=[[0 for i in range(map_scale_y)] for i in range(map_scale_x)]
#
# random a map
# y
# 19
# ...
# 5
# 4
# 3
# 2
# 0 1 2 3 x
def random_a_path(map_scale_x, map_scale_y):
    global game_map

    path_map=[[0 for i in range(map_scale_y)] for i in range(map_scale_x)]
    for j in range(1, map_scale_y):
        for i in range(map_scale_x):
            path_map[i][j]=1

    flo_pos=[int(map_scale_x/2)-1,0]
    path_map[flo_pos[0]][flo_pos[1]]=0
    # direction:
    #   0
    # 1   2
    #   3
    # last position:
    #   0
    # 1   2
    #   3
    #
    last_pos=3
    rollback_pos=flo_pos[:]
    rollback_map=path_map[:]
    rollback_last=3
    #for debug
    vec_rollback=[]
    vec_rollback.append(rollback_pos[:])
    vec_trig_roll=[]
    vec_trig_roll.append(rollback_pos[:])
    turn_from=0
    #while flo_pos[1]<map_scale_y-1:
    test_cnt=0
    # for debug
    print("test_cnt:rdm_direc:(flo_x,flo_y):(roll_x,roll_y)")
    while test_cnt<100 and flo_pos[1]<map_scale_y-1:
        rdm_direc=random.randint(0,3)
        if rdm_direc==last_pos:
            continue
        else:
            # for debug
            print("%02d:%d:(%d,%d):(%d,%d)" %(test_cnt, rdm_direc, flo_pos[0], flo_pos[1],rollback_pos[0], rollback_pos[1]))
            if rdm_direc==0:
                if flo_pos[0]>0 and flo_pos[0]<map_scale_x-1:
                    if flo_pos[1]<map_scale_y-1 and path_map[flo_pos[0]][flo_pos[1]+1]==1 \
                    and path_map[flo_pos[0]-1][flo_pos[1]+1]==1 \
                    and path_map[flo_pos[0]+1][flo_pos[1]+1]==1 :
                        flo_pos[1]+=1
                        last_pos=3
                else: # in left edge or right edge
                    flo_pos[1]+=1
                    last_pos=3
            elif rdm_direc==1:
                if turn_from==1:
                    if flo_pos[0]<map_scale_x-1 and path_map[flo_pos[0]+1][flo_pos[1]]==1 :
                        flo_pos[0]+=1
                        last_pos=1
                        turn_from=0
                    else:
                        vec_trig_roll.append(flo_pos[:])
                        flo_pos=rollback_pos[:]
                        path_map=rollback_map[:]
                        last_pos=rollback_last
                        vec_rollback.append(rollback_pos[:])
                        turn_from=0
                elif flo_pos[0]>0:
                    if path_map[flo_pos[0]-1][flo_pos[1]]==1 :
                        if flo_pos[1]>0 and path_map[flo_pos[0]-1][flo_pos[1]-1]==1 \
                        and flo_pos[1]<map_scale_y-1 and path_map[flo_pos[0]-1][flo_pos[1]+1]==1 :
                            flo_pos[0]-=1
                            last_pos=2
                    else: # path conflict
                        vec_trig_roll.append(flo_pos[:])
                        flo_pos=rollback_pos[:]
                        path_map=rollback_map[:]
                        last_pos=rollback_last
                        vec_rollback.append(rollback_pos[:])
                        turn_from=0
                else: # flo_pos[0]==0
                    if flo_pos[1]>0 and path_map[flo_pos[0]+1][flo_pos[1]-1]==1 and path_map[flo_pos[0]+1][flo_pos[1]]==1:
                        flo_pos[0]+=1
                        last_pos=1
                    elif flo_pos[1]<map_scale_y-1:
                        flo_pos[1]+=1
                        last_pos=3
            elif rdm_direc==2:
                if turn_from==2:
                    if flo_pos[0]>0 and path_map[flo_pos[0]-1][flo_pos[1]]==1 :
                        flo_pos[0]-=1
                        last_pos=2
                        turn_from=0
                    else:
                        vec_trig_roll.append(flo_pos[:])
                        flo_pos=rollback_pos[:]
                        path_map=rollback_map[:]
                        last_pos=rollback_last
                        vec_rollback.append(rollback_pos[:])
                        turn_from=0
                elif flo_pos[0]<map_scale_x-1:
                    if path_map[flo_pos[0]+1][flo_pos[1]]==1 :
                        if flo_pos[1]>0 and path_map[flo_pos[0]+1][flo_pos[1]-1]==1 \
                        and flo_pos[1]<map_scale_y-1 and path_map[flo_pos[0]+1][flo_pos[1]+1]==1 :
                            flo_pos[0]+=1
                            last_pos=1
                    else: # path conflict
                        vec_trig_roll.append(flo_pos[:])
                        flo_pos=rollback_pos[:]
                        path_map=rollback_map[:]
                        last_pos=rollback_last
                        vec_rollback.append(rollback_pos[:])
                        turn_from=0
                else: # flo_pos[0]==map_scale_x-1
                    if flo_pos[1]>0 and path_map[flo_pos[0]-1][flo_pos[1]-1]==1 and path_map[flo_pos[0]-1][flo_pos[1]]==1:
                        flo_pos[0]-=1
                        last_pos=2
                    elif flo_pos[1]<map_scale_y-1:
                        flo_pos[1]+=1
                        last_pos=3
            elif rdm_direc==3:
                if last_pos==0: # continue downward
                    if flo_pos[1]>0 and path_map[flo_pos[0]][flo_pos[1]-1]==1:
                        flo_pos[1]-=1
                        last_pos=0
                    else:
                        vec_trig_roll.append(flo_pos[:])
                        flo_pos=rollback_pos[:]
                        path_map=rollback_map[:]
                        last_pos=rollback_last
                        vec_rollback.append(rollback_pos[:])
                        turn_from=0
                elif last_pos==1: # from left
                    if flo_pos[1]>0 and path_map[flo_pos[0]][flo_pos[1]-1]==1 \
                    and flo_pos[0]>0 and path_map[flo_pos[0]-1][flo_pos[1]-1]==1 \
                    and flo_pos[0]<map_scale_x-1 and path_map[flo_pos[0]+1][flo_pos[1]-1]==1 :
                        if turn_from==0:
                            rollback_pos=flo_pos[:]
                            rollback_map=path_map[:]
                            rollback_last=last_pos
                        turn_from=1
                        flo_pos[1]-=1
                        last_pos=0
                elif last_pos==2: # from right
                    if flo_pos[1]>0 and path_map[flo_pos[0]][flo_pos[1]-1]==1 \
                    and flo_pos[0]>0 and path_map[flo_pos[0]-1][flo_pos[1]-1]==1 \
                    and flo_pos[0]<map_scale_x-1 and path_map[flo_pos[0]+1][flo_pos[1]-1]==1 :
                        if turn_from==0:
                            rollback_pos=flo_pos[:]
                            rollback_map=path_map[:]
                            rollback_last=last_pos
                        turn_from=2
                        flo_pos[1]-=1
                        last_pos=0
        #path_map[flo_pos[0]][flo_pos[1]]=test_cnt
        path_map[flo_pos[0]][flo_pos[1]]=0
        test_cnt+=1
    #for i in range(len(vec_rollback)):
    #    path_map[vec_rollback[i][0]][vec_rollback[i][1]]='Z'
    #    path_map[vec_trig_roll[i][0]][vec_trig_roll[i][1]]='R'
    print(vec_rollback)

    #pick a good path
    if flo_pos[1]==map_scale_y-1 and test_cnt>40 and test_cnt<80:
        game_map=path_map[:]
        return 0
    else:
        return 1

# 
def random_a_map(map_scale_x, map_scale_y):
    remap=1
    while 1==remap:
        remap=random_a_path(map_scale_x, map_scale_y)

random_a_map(map_scale_x, map_scale_y)

# 3 - Initialize the game
pygame.init()
screen=pygame.display.set_mode((ww, hh))
pygame.display.set_caption("Tank War -- by cgm")
FPSClock = pygame.time.Clock()
#pygame.font.init()
font = pygame.font.Font("res/font/Droid Sans Mono.ttf", 12)

# 4 - keep looping
running=True
while running:
    # 5 - clear the screen before drawing it again
    screen.fill(0)
    # 6 - loop through the events
    for event in pygame.event.get():
        # check if the event is the X button
        if event.type==pygame.QUIT:
            # if it is quit the game
            running=False
        if event.type == pygame.KEYDOWN:
            if event.key==pygame.K_ESCAPE:
                keys[0]=True
                if g_status!=0:
                    g_status=0
            elif event.key==pygame.K_RETURN:
                keys[1]=True
            elif event.key==pygame.K_SPACE:
                keys[2]=True
                # for debug
                random_a_map(map_scale_x, map_scale_y)
            #    -------------
            #   12  13  14  15
            #    8   9  10  11
            #    4   5   6   7
            #    0   1   2   3
            #    -------------
            elif event.key==pygame.K_w or event.key==pygame.K_UP:
                keys[3]=True
                # not at the top of the map
                if ref_pos_in_map[1]<map_scale_y-4:
                    if player.posi_id<4:
                        player.posi_id+=4
                    else:
                        ref_pos_in_map[1]+=1
                else:
                    if player.posi_id<12:
                        player.posi_id+=4
                if tank_in_map[1]<map_scale_y-1:
                    tank_in_map[1]+=1
            elif event.key==pygame.K_s or event.key==pygame.K_DOWN:
                keys[4]=True
                if player.posi_id>3:
                    player.posi_id-=4
                    tank_in_map[1]-=1
                elif ref_pos_in_map[1]>0:
                    ref_pos_in_map[1]-=1
                    player.posi_id+=4
            elif event.key==pygame.K_a or event.key==pygame.K_LEFT:
                keys[5]=True
                if player.posi_id not in (0,4,8,12):
                    player.posi_id-=1
                    tank_in_map[0]-=1
                elif ref_pos_in_map[0]>0:
                    ref_pos_in_map[0]-=1
                    player.posi_id+=1
            elif event.key==pygame.K_d or event.key==pygame.K_RIGHT:
                keys[6]=True
                if player.posi_id not in (3,7,11,15):
                    player.posi_id+=1
                    tank_in_map[0]+=1
                elif ref_pos_in_map[0]<map_scale_x-4:
                    ref_pos_in_map[0]+=1
                    player.posi_id-=1
        if event.type == pygame.KEYUP:
            if event.key==pygame.K_ESCAPE:
                keys[0]=False
            elif event.key==pygame.K_RETURN:
                keys[1]=False
            elif event.key==pygame.K_SPACE:
                keys[2]=False
            elif event.key==pygame.K_w or event.key==pygame.K_UP:
                keys[3]=False
            elif event.key==pygame.K_s or event.key==pygame.K_DOWN:
                keys[4]=False
            elif event.key==pygame.K_a or event.key==pygame.K_LEFT:
                keys[5]=False
            elif event.key==pygame.K_d or event.key==pygame.K_RIGHT:
                keys[6]=False
        if event.type==pygame.MOUSEBUTTONDOWN:
            ms_pos=pygame.mouse.get_pos()
            if g_status==0:
                if btn[0].x<event.pos[0]<btn[0].x+btn[0].width:
                    if btn[0].y<event.pos[1]<btn[0].y+btn[0].height:
                        g_status=1
                    elif btn[1].y<event.pos[1]<btn[1].y+btn[1].height:
                        g_status=2
                    elif btn[2].y<event.pos[1]<btn[2].y+btn[2].height:
                        g_status=3
                    elif btn[3].y<event.pos[1]<btn[3].y+btn[3].height:
                        running=False
    # ===== event over =====

    # 7 - draw the screen elements
    # in title
# ==========status=0:title==========
    if g_status==0:
        # draw background
        screen.blit(bg_title,(0,0))
        # draw button
        for i in btn:
            pygame.draw.rect(screen, (70,0,200),[i.x, i.y, i.width, i.height], 0)
            font = pygame.font.Font("res/font/Droid Sans Mono.ttf", 24)
            text = font.render(i.desc, True, (255,255,0))
            textRect = text.get_rect()
            textRect.centerx = i.x+i.width/2
            textRect.centery = i.y+i.height/2
            screen.blit(text, textRect)
# ==========status=1:classic game==========
    elif g_status==1:
        # terrain draw first
        l_pos=0
        for i in draw_pos:
            if l_pos<4:
                if game_map[ref_pos_in_map[0]+l_pos][ref_pos_in_map[1]] == 1:
                    screen.blit(p_terrain[0], i)
                    screen.blit(p_mine[0], i)
            elif l_pos<8:
                if game_map[ref_pos_in_map[0]+l_pos-4][ref_pos_in_map[1]+1] == 1:
                    screen.blit(p_terrain[1], i)
                    screen.blit(p_mine[1], i)
            elif l_pos<12:
                if game_map[ref_pos_in_map[0]+l_pos-8][ref_pos_in_map[1]+2] == 1:
                    screen.blit(p_terrain[2], i)
                    screen.blit(p_mine[2], i)
            else:
                if game_map[ref_pos_in_map[0]+l_pos-12][ref_pos_in_map[1]+3] == 1:
                    screen.blit(p_terrain[3], i)
                    screen.blit(p_mine[3], i)
            l_pos+=1
        # player tank draw
        if player.posi_id<4:
            screen.blit(p_tank[0], draw_pos[player.posi_id])
        elif player.posi_id<8:
            screen.blit(p_tank[1], draw_pos[player.posi_id])
        elif player.posi_id<12:
            screen.blit(p_tank[2], draw_pos[player.posi_id])
        else:
            screen.blit(p_tank[3], draw_pos[player.posi_id])
# ==========status=2:advance game==========
    elif g_status==2:
        screen.blit(pic_home,(300,300))
# ==========status=3:about page==========
    elif g_status==3:
        screen.blit(pic_grass,(300,300))
# ==========status    over==========

    if is_debug:
        # version
        font = pygame.font.Font("res/font/Droid Sans Mono.ttf", 12)
        text = font.render("pygame_version="+pygame.version.ver, True, (255,255,255))
        textRect = text.get_rect()
        textRect.bottomleft = (0, hh)
        screen.blit(text, textRect)
        # key code
        if keys[3]:
            test = font.render("^", True, (255,255,255))
            screen.blit(test, (0,0))
        if keys[4]:
            test = font.render("v", True, (255,255,255))
            screen.blit(test, (12,0))
        if keys[5]:
            test = font.render("<", True, (255,255,255))
            screen.blit(test, (12*2,0))
        if keys[6]:
            test = font.render(">", True, (255,255,255))
            screen.blit(test, (12*3,0))
        # mouse position
        text = font.render("ms_pos="+str(ms_pos[0])+", "+str(ms_pos[1]), True, (255,255,240))
        screen.blit(text, (0,12))
        # test clock
        text = font.render("tick="+str(pygame.time.get_ticks()), True, (255,255,240))
        screen.blit(text, (0,12*2))

        # test output map array
        for j in range(map_scale_y):
            for i in range(map_scale_x):
                if i == tank_in_map[0] and j==tank_in_map[1]:
                    text = font.render("T", True, (255,255,255))
                else:
                    text = font.render(str(game_map[i][j]), True, (255,255,255))
                screen.blit(text, (i*12, 2*hh/3-j*12))
        pygame.draw.rect(screen, (255,0,0), (0+ref_pos_in_map[0]*12, 2*hh/3-12*(ref_pos_in_map[1]+3),12*4,12*4),1)

        #text = font.render("rollback_pos="+str(rollback_pos[0])+","+str(rollback_pos[1]), True, (255,255,240))
        #screen.blit(text, (0,12*9))

        # test tmp
        #text = font.render(str(player.posi_id), True, (255,255,240))
        #screen.blit(text, (0,100))
    # debug code above ^^^
 
    # === draw over ===

    # 8 - update the screen
    #pygame.display.flip()
    pygame.display.update()
    FPSClock.tick(30)
# main loop over====================
pygame.quit()
