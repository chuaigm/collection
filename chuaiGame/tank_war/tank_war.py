# 1 - Import library
import pygame
import math
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
    arr.append(string.atoi(line))
if is_debug:
    print arr[0]
# width, height = 800,600
if arr[0] == 2: 
    ww, hh = 1024, 200
#elif arr[0] == 3:
#    ww, hh = 1280, 800
else:
    ww, hh = 800, 600
#ww = string.atoi(arr[0])
#hh = string.atoi(arr[1])

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
player=player_cls(5)
# player position array
tank_draw_pos = []
tank_draw_pos.append((0,hh/2))
tank_draw_pos.append((ww/4,hh/2))
tank_draw_pos.append((ww/4*2,hh/2))
tank_draw_pos.append((ww/4*3,hh/2))
tank_draw_pos.append((0,hh*3/4))
tank_draw_pos.append((ww/4,hh*3/4))
tank_draw_pos.append((ww/4*2,hh*3/4))
tank_draw_pos.append((ww/4*3,hh*3/4))
#    -------------
#
#
#    0   1   2   3
#    4   5   6   7
#    -------------
# define function
def init_game():
    player.posi_id=5

# 3 - Load images
bg_title = pygame.image.load("res/pic/bg_title.jpg")
player_p = pygame.image.load("res/pic/player_tank.jpg")
grass  = pygame.image.load("res/pic/grass.png")
home   = pygame.image.load("res/pic/home.png")

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
            #   0   1   2   3
            #   4   5   6   7
            elif event.key==pygame.K_w:
                keys[3]=True
                if player.posi_id>3:
                    player.posi_id-=4
            elif event.key==pygame.K_s:
                keys[4]=True
                if player.posi_id<4:
                    player.posi_id+=4
            elif event.key==pygame.K_a:
                keys[5]=True
                if player.posi_id>0 and player.posi_id!=4:
                    player.posi_id-=1
            elif event.key==pygame.K_d:
                keys[6]=True
                if player.posi_id<7 and player.posi_id!=3:
                    player.posi_id+=1
        if event.type == pygame.KEYUP:
            if event.key==pygame.K_ESCAPE:
                keys[0]=False
            elif event.key==pygame.K_RETURN:
                keys[1]=False
            elif event.key==pygame.K_SPACE:
                keys[2]=False
            elif event.key==pygame.K_w:
                keys[3]=False
            elif event.key==pygame.K_s:
                keys[4]=False
            elif event.key==pygame.K_a:
                keys[5]=False
            elif event.key==pygame.K_d:
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
        screen.blit(player_p,(tank_draw_pos[player.posi_id][0],tank_draw_pos[player.posi_id][1]))
# ==========status=2:advance game==========
    elif g_status==2:
        screen.blit(home,(300,300))
# ==========status=3:about page==========
    elif g_status==3:
        screen.blit(grass,(300,300))
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
            test = font.render("W", True, (255,255,255))
            screen.blit(test, (0,0))
        if keys[4]:
            test = font.render("A", True, (255,255,255))
            screen.blit(test, (12,0))
        if keys[5]:
            test = font.render("S", True, (255,255,255))
            screen.blit(test, (12*2,0))
        if keys[6]:
            test = font.render("D", True, (255,255,255))
            screen.blit(test, (12*3,0))
        # mouse position
        text = font.render("ms_pos="+str(ms_pos[0])+", "+str(ms_pos[1]), True, (255,255,240))
        screen.blit(text, (0,12))
        # test clock
        text = font.render("tick="+str(pygame.time.get_ticks()), True, (255,255,240))
        screen.blit(text, (0,12*2))
    # debug code above ^^^
 
    # === draw over ===

    # 8 - update the screen
    #pygame.display.flip()
    pygame.display.update()
    FPSClock.tick(60)
# main loop over====================
pygame.quit()
