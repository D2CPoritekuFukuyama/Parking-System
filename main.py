#!/usr/bin/env python3
# coding: UTF-8

import os
import wiringpi2 as wp

ENT_HOLE_SENSOR = 17 #入り口用ホールセンサー
EXT_HOLE_SENSOR = 22 #出口用ホールセンサー
GATE_SWITCH = 23 #ゲートのスイッチ
GATE_STATUS = True #True=open, False=close

def gate_close():
    #チャタリング防止コード---------------
    wp.delay(50)
    if wp.digitalRead(23) != 1:
     return
    #---------------チャタリング防止コード

    global GATE_STATUS
    if GATE_STATUS:
        os.system('./motor/gate_close')
        GATE_STATUS=False
    else:
        os.system('./motor/gate_open')
        GATE_STATUS=True 

#GPIOの初期化
wp.wiringPiSetupGpio()
#入り口のホールセンサー用GPIO0を入力で初期化
wp.pinMode(ENT_HOLE_SENSOR, wp.GPIO.INPUT)
#入り口のホールセンサー用GPIO3を入力で初期化
wp.pinMode(EXT_HOLE_SENSOR, wp.GPIO.INPUT)
#ゲートのスイッチのGPIO4を入力で初期化
wp.pinMode(GATE_SWITCH, wp.GPIO.INPUT)
#ゲートスイッチの割り込み処理を設定
wp.wiringPiISR(GATE_SWITCH, wp.GPIO.INT_EDGE_BOTH, gate_close)

while True:
    if wp.digitalRead(ENT_HOLE_SENSOR) == 0:
        os.system('./Nplate_rcg/main')
