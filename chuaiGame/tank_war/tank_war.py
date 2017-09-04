# 1 - Import library
import pygame
import math
import random
import string
from pygame.locals import *

# 2 - Read conf & set var
is_debug=1

# 
class button(object):
    def __init__(self, desc, x, y, width=100, height=30):
        self.desc = desc
        self.x = x
        self.y = y
        self.width = width
        self.height = height

btn=[]
btn.append(button("start", 600, 400)) # 1, start button
btn.append(button("exit", 600, 400))  # 2, xxx button

arr=[]
pos=0
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

keys = [False, False, False, False]
ms_pos =[0,0]
playerpos=[100,100]
acc=[0,0]
arrows=[]
badtimer=100
badtimer1=0
badguys=[[640,100]]
healthvalue=194


# 3 - Load images
bg_title = pygame.image.load("res/pic/bg_title.jpg")
player = pygame.image.load("res/pic/dude.png")
grass  = pygame.image.load("res/pic/grass.png")
home   = pygame.image.load("res/pic/home.png")
arrow  = pygame.image.load("res/pic/arrow.png")
badguyimg = pygame.image.load("res/pic/enemy.png")
healthbar = pygame.image.load("res/pic/healthbar.png")
health = pygame.image.load("res/pic/health.png")
gameover = pygame.image.load("res/pic/gameover.png")
youwin = pygame.image.load("res/pic/youwin.png")

# 3 - Initialize the game
pygame.init()
screen=pygame.display.set_mode((ww, hh))
pygame.display.set_caption("Tank War -- by cgm")
pygame.font.init()
font = pygame.font.Font(None, 20)

# 4 - keep looping
while True:
    # 5 - clear the screen before drawing it again
    screen.fill(0)

    # 6 - loop through the events
    for event in pygame.event.get():
        # check if the event is the X button
        if event.type==pygame.QUIT:
            # if it is quit the game
            pygame.quit()
            exit(0)
        if event.type == pygame.KEYDOWN:
            if event.key==pygame.K_w:
                keys[0]=True
            elif event.key==pygame.K_a:
                keys[1]=True
            elif event.key==pygame.K_s:
                keys[2]=True
            elif event.key==pygame.K_d:
                keys[3]=True
        if event.type == pygame.KEYUP:
            if event.key==pygame.K_w:
                keys[0]=False
            elif event.key==pygame.K_a:
                keys[1]=False
            elif event.key==pygame.K_s:
                keys[2]=False
            elif event.key==pygame.K_d:
                keys[3]=False
        if event.type==pygame.MOUSEBUTTONDOWN:
            ms_pos=pygame.mouse.get_pos()
        if event.type==pygame.MOUSEBUTTONDOWN and btn[0].x<event.pos[0]<btn[0].x+btn[0].width and \
                btn[0].y<event.pos[1]<btn[0].y+btn[0].height:
            pygame.quit()
            exit(0)
    # ===== event over =====

    # 7 - draw the screen elements
    # draw background
    screen.blit(bg_title,(0,0))
    pygame.draw.rect(screen, (60,0,250),[btn[0].x, btn[0].y, btn[0].width, btn[0].height], 0)
    text = font.render(btn[0].desc, True, (255,255,0))
    screen.blit(text, [btn[0].x, btn[0].y])

    if is_debug:
        # version
        text = font.render("pygame_version="+pygame.version.ver, True, (255,255,255))
        textRect = text.get_rect()
        textRect.bottomleft = (0, hh)
        screen.blit(text, textRect)
        # key code
        if keys[0]:
            test = font.render("W", True, (255,255,255))
            screen.blit(test, (0,0))
        if keys[1]:
            test = font.render("A", True, (255,255,255))
            screen.blit(test, (12,0))
        if keys[2]:
            test = font.render("S", True, (255,255,255))
            screen.blit(test, (12*2,0))
        if keys[3]:
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
            
 

"""

# 4 - keep looping through
running = 1
exitcode = 0
while running:
    badtimer-=1
    # 5 - clear the screen before drawing it again
    screen.fill(0)
    # 6 - draw the screen elements
    for x in range(width/grass.get_width()+1):
        for y in range(height/grass.get_height()+1):
            screen.blit(grass,(x*64,y*64))
    screen.blit(home,(0,30))
    screen.blit(home,(0,135))
    screen.blit(home,(0,240))
    screen.blit(home,(0,345))
    #screen.blit(player, playerpos)
    # 6.1 - Set player position and rotation
    position = pygame.mouse.get_pos()
    angle = math.atan2(position[1]-(playerpos[1]+32),position[0]-(playerpos[0]+26))
    playerrot = pygame.transform.rotate(player, 360-angle*57.29)
    playerpos1 = (playerpos[0]-playerrot.get_rect().width/2, playerpos[1]-playerrot.get_rect().height/2)
    screen.blit(playerrot, playerpos1)
    # 6.2 - Draw arrows
    for bullet in arrows:
        index=0
        velx=math.cos(bullet[0])*10
        vely=math.sin(bullet[0])*10
        bullet[1]+=velx
        bullet[2]+=vely
        if bullet[1]<-64 or bullet[1]>640 or bullet[2]<-64 or bullet[2]>480:
            arrows.pop(index)
        index+=1
        for projectile in arrows:
            arrow1 = pygame.transform.rotate(arrow, 360-projectile[0]*57.29)
            screen.blit(arrow1, (projectile[1], projectile[2]))
    # 6.3 - Draw badgers
    if badtimer==0:
        badguys.append([640, random.randint(50,430)])
        badtimer=100-(badtimer1*2)
        if badtimer1>=35:
            badtimer1=35
        else:
            badtimer1+=5
    index=0
    for badguy in badguys:
        if badguy[0]<-64:
            badguys.pop(index)
        badguy[0]-=7
        # 6.3.1 - Attack castle
        badrect=pygame.Rect(badguyimg.get_rect())
        badrect.top=badguy[1]
        badrect.left=badguy[0]
        if badrect.left<64:
            healthvalue -= random.randint(5,20)
            badguys.pop(index)
        #6.3.2 - Check for collisions
        index1=0
        for bullet in arrows:
            bullrect=pygame.Rect(arrow.get_rect())
            bullrect.left=bullet[1]
            bullrect.top=bullet[2]
            if badrect.colliderect(bullrect):
                acc[0]+=1
                badguys.pop(index)
                arrows.pop(index1)
            index1+=1
        # 6.3.3 - Next bad guy
        index+=1
    for badguy in badguys:
        screen.blit(badguyimg, badguy)
    # 6.4 - Draw clock
    font = pygame.font.Font(None, 24)
    survivedtext = font.render(str((90000-pygame.time.get_ticks())/60000)+":"+str((90000-pygame.time.get_ticks())/1000%60).zfill(2), True, (0,0,0))
    textRect = survivedtext.get_rect()
    textRect.topright=[635,5]
    screen.blit(survivedtext, textRect)
    # 6.5 - Draw health bar
    screen.blit(healthbar, (5,5))
    for health1 in range(healthvalue):
        screen.blit(health, (health1+8,8))
    # 7 - update the screen
    pygame.display.flip()
    # 8 - loop through the events
    for event in pygame.event.get():
        # check if the event is the X button
        if event.type==pygame.QUIT:
            # if it is quit the game
            pygame.quit()
            exit(0)
        if event.type == pygame.KEYDOWN:
            if event.key==K_w:
                keys[0]=True
            elif event.key==K_a:
                keys[1]=True
            elif event.key==K_s:
                keys[2]=True
            elif event.key==K_d:
                keys[3]=True
        if event.type == pygame.KEYUP:
            if event.key==pygame.K_w:
                keys[0]=False
            elif event.key==pygame.K_a:
                keys[1]=False
            elif event.key==pygame.K_s:
                keys[2]=False
            elif event.key==pygame.K_d:
                keys[3]=False
        if event.type==pygame.MOUSEBUTTONDOWN:
            position=pygame.mouse.get_pos()
            acc[1]+=1
            arrows.append([math.atan2(position[1]-(playerpos1[1]+32),position[0]-(playerpos1[0]+26)),playerpos1[0]+32,playerpos1[1]+32])
    # 9 - Move player
    if keys[0]:
        playerpos[1]-=5
    elif keys[2]:
        playerpos[1]+=5
    if keys[1]:
        playerpos[0]-=5
    elif keys[3]:
        playerpos[0]+=5
#10 - Win/Lose check
    if pygame.time.get_ticks()>=90000:
        running=0
        exitcode=1
    if healthvalue<=0:
        running=0
        exitcode=0
    if acc[1]!=0:
        accuracy=acc[0]*1.0/acc[1]*100
    else:
        accuracy=0
# 11 - Win/lose display        
if exitcode==0:
    pygame.font.init()
    font = pygame.font.Font(None, 24)
    text = font.render("Accuracy: "+str(accuracy)+"%", True, (255,0,0))
    textRect = text.get_rect()
    textRect.centerx = screen.get_rect().centerx
    textRect.centery = screen.get_rect().centery+24
    screen.blit(gameover, (0,0))
    screen.blit(text, textRect)
else:
    pygame.font.init()
    font = pygame.font.Font(None, 24)
    text = font.render("Accuracy: "+str(accuracy)+"%", True, (0,255,0))
    textRect = text.get_rect()
    textRect.centerx = screen.get_rect().centerx
    textRect.centery = screen.get_rect().centery+24
    screen.blit(youwin, (0,0))
    screen.blit(text, textRect)
while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit(0)
    pygame.display.flip()
"""
