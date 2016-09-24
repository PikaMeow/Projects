# -*- coding: utf-8 -*-
import pygame
from pygame.locals import *
import random




class Player(pygame.sprite.Sprite):
    def __init__(self):
        super(Player,self).__init__()
        #self.surf = pygame.Surface((75, 25))

        #导入图片路径，并用convert()函数创建副本，可以更快地将其画在屏幕上
        self.image=pygame.image.load('jet.png').convert()
        #设置图片为透明(若不设置则为透明),第二个参数有助于更快渲染
        self.image.set_colorkey((255,255,255),RLEACCEL)

        #self.surf.fill((255, 255, 255))

        self.rect = self.image.get_rect()


    def update(self,pressed_keys):
        if pressed_keys[K_UP]:

            self.rect.move_ip(0,-5)
        if pressed_keys[K_DOWN]:
            self.rect.move_ip(0,5)
        if pressed_keys[K_LEFT]:
            self.rect.move_ip(-5,0)
        if pressed_keys[K_RIGHT]:
            self.rect.move_ip(5,0)


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
        #self.surf=pygame.Surface((20,10))
        #self.surf.fill((255,255,255))

        self.image=pygame.image.load('missile.png').convert()
        self.image.set_colorkey((255,255,255),RLEACCEL)

        self.rect=self.image.get_rect(center=(random.randint(820,900),random.randint(0,600)))
        self.speed=random.randint(1,2)

    def update(self):
        self.rect.move_ip(-self.speed,0)


        if self.rect.right<0:
            self.kill()

#创建一些云的对象，看起来飞机在天空中飞
class Cloud(pygame.sprite.Sprite):
    def __init__(self):
        super(Cloud,self).__init__()
        self.image=pygame.image.load('cloud.png').convert()
        self.image.set_colorkey((0,0,0),RLEACCEL)
        #云从右边飘进来，好像飞机在从左向右行驶
        self.rect=self.image.get_rect(
            center=(random.randint(820,900),random.randint(0,300))
        )

    def update(self):
        self.rect.move_ip(-1,0)
        if self.rect.right<0:
            self.kill()

pygame.init()

screen=pygame.display.set_mode((800,600))



ADDENEMY=pygame.USEREVENT+1
pygame.time.set_timer(ADDENEMY,250)

#为云的出现添加事件
ADDCLOUD=pygame.USEREVENT+2
pygame.time.set_timer(ADDCLOUD,1000)

player=Player()


background=pygame.Surface(screen.get_size())
background.fill((135,206,250))



enemies=pygame.sprite.Group()
all_sprites=pygame.sprite.Group()
clouds=pygame.sprite.Group()

all_sprites.add(player)

running=True




while running:

    for event in pygame.event.get():

        if event.type==KEYDOWN:

            if event.key==K_ESCAPE:
                running=False

        elif event.type==QUIT:
            running=False
        elif event.type==ADDENEMY:
            new_enemy=Enemy()
            enemies.add(new_enemy)
            all_sprites.add(new_enemy)
        elif event.type==ADDCLOUD:
            new_cloud=Cloud()
            clouds.add(new_cloud)
            all_sprites.add(new_cloud)



    screen.blit(background, (0, 0))


    pressed_keys=pygame.key.get_pressed()


    player.update(pressed_keys)

    enemies.update()

    clouds.update()



    for entity in all_sprites:
        screen.blit(entity.image,entity.rect)


    if pygame.sprite.spritecollideany(player,enemies):

        player.kill()

    pygame.display.flip()

