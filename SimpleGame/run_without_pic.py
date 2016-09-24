# -*- coding: utf-8 -*-
import pygame
from pygame.locals import * #包含一些需要的常量
import random

#Sprite是屏幕上事物的二维表达，通过扩展这个类，
#可以包含想要在屏幕上呈现的对象一个或多个图形表示
#这里新建一个Player类继承它，可以使用它的内建方法，使用surf和rect等属性
class Player(pygame.sprite.Sprite):
    def __init__(self):
        super(Player,self).__init__()
        self.surf = pygame.Surface((75, 25))
        # Surface可以看作一张白纸，可以随意绘画。创建时指定像素的长度和宽度
        self.surf.fill((255, 255, 255))
        # 为Surface填充颜色(R G B)，这里是白色
        self.rect = self.surf.get_rect()
        # 获取四元组 Ract(left,top,width,height),代表Surface的大小和位置

    def update(self,pressed_keys):
        if pressed_keys[K_UP]:
            #如果按压'上箭头',则向上移动5个像素点的位置
            self.rect.move_ip(0,-5)
        if pressed_keys[K_DOWN]:
            self.rect.move_ip(0,5)
        if pressed_keys[K_LEFT]:
            self.rect.move_ip(-5,0)
        if pressed_keys[K_RIGHT]:
            self.rect.move_ip(5,0)

        #保证方块不会越界
        if self.rect.left < 0:
            self.rect.left = 0
        elif self.rect.right > 800:
            self.rect.right = 800
        if self.rect.top <= 0:
            self.rect.top = 0
        elif self.rect.bottom >= 600:
            self.rect.bottom = 600

class Enemy(pygame.sprite.Sprite):
    def __init__(self):
        super(Enemy,self).__init__()
        self.surf=pygame.Surface((20,10))
        self.surf.fill((255,255,255))
        #从屏幕右方(820)的随机位置(0-600)上出现
        self.rect=self.surf.get_rect(center=(820,random.randint(0,600)))
        self.speed=random.randint(1,2)

    def update(self):
        self.rect.move_ip(-self.speed,0)
        #到达屏幕左端消失后从sprite组中删除，从而不会再被渲染出来
        #但kill不会释放内存，需要使用者确保不再引用
        if self.rect.right<0:
            self.kill()

pygame.init() #初始化所有导入的PyGame模块，在其他操作之前调用

screen=pygame.display.set_mode((800,600))
#创建屏幕对象(画布)，向set_mode()中传递包含屏幕窗口大小的元组 (宽度，高度)

#自定义事件，是一个整形变量，但是要比内置的事件值大
ADDENEMY=pygame.USEREVENT+1

#将事件插入事件队列中。每隔250毫秒触发一次事件，在主循环之外执行
pygame.time.set_timer(ADDENEMY,250)

player=Player()
#初始化Player,现在仅仅是个矩形

background=pygame.Surface(screen.get_size())
background.fill((0,0,0))
#新建一个Surface对象，完全遮盖住屏幕对象，并填充黑色

#建立Groups(Sprite的集合),方便处理冲突和更新问题
enemies=pygame.sprite.Group()
all_sprites=pygame.sprite.Group()

#将player对象添加到all_sprites组中
all_sprites.add(player)

running=True
#游戏主循环(事件循环)的指示变量，保证主循环的运行


#游戏主循环：
while running:
    #遍历事件队列(包括用户的输入及一些其他事件)
    for event in pygame.event.get():
        #检测按键盘事件，KEYDOWN是导入的locals里定义的常量
        if event.type==KEYDOWN:
            #按下Esc,主循环终止
            if event.key==K_ESCAPE:
                running=False
        #出现QUIT信号，终止主循环
        elif event.type==QUIT:
            running=False
        elif (event.type==ADDENEMY):
            new_enemy=Enemy()
            enemies.add(new_enemy)
            all_sprites.add(new_enemy)

    #每次循环都渲染一次黑色背景，否则在移动过程中之前的矩形块也会残留下来
    screen.blit(background, (0, 0))

    #获取所有按键事件组成的字典
    pressed_keys=pygame.key.get_pressed()

    #调用方法对所按的键做出反应
    player.update(pressed_keys)

    enemies.update()

    #screen.blit(player.surf, player.rect)  # 如果第二个参数是surf.rect,会使用rect四元组的前两个参数(rect左上角的x和y轴坐标)作为绘画的起始位置
    # 将新建的Surface绘制(Blit)到另一个Surface(刚开始创建的屏幕对象)上，坐标为 x:400,y:300,是屏幕的正中心位置

    #渲染组中所有对象(刚开始相当于直接对player进行渲染，因为只有这一个sprite)
    for entity in all_sprites:
        screen.blit(entity.surf,entity.rect)

    #使用spritecollideany(sprite对象,sprite.Groups)检测两个参数代表的对象是否发生冲突
    if pygame.sprite.spritecollideany(player,enemies):
        #如果player与enemies中的对象发成冲突，用kill()方法将其从所在的所有Groups中移出
        player.kill()

    pygame.display.flip()
    # 使用flip()会更新自上次flip后的整个屏幕,两次flip之间发生的修改都将在屏幕上显示
