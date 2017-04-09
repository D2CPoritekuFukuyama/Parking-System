#!/usr/bin/env python3
# coding: UTF-8

import os
import json
import wiringpi2 as wp
import ml_module

ENT_HOLE_SENSOR = 17 #入り口用ホールセンサー
EXT_HOLE_SENSOR = 22 #出口用ホールセンサー
GATE_SWITCH = 23 #ゲートのスイッチ
GATE_STATUS = True #True=open, False=close

def get_push():
    #チャタリング防止コード---------------
    wp.delay(50)
    if wp.digitalRead(23) != 1:
     return
    #---------------チャタリング防止コード
    gate_actuation()

def gate_actuation():
    global GATE_STATUS
    if GATE_STATUS:
        os.system('./motor/gate_close')
        GATE_STATUS=False
    else:
        os.system('./motor/gate_open')
        GATE_STATUS=True 

#会員認証結果を格納
member = []
#kerasのモデルの初期化
Nplate_reader = ml_module.ML_module()
#GPIOの初期化
wp.wiringPiSetupGpio()
#入り口のホールセンサー用GPIO0を入力で初期化
wp.pinMode(ENT_HOLE_SENSOR, wp.GPIO.INPUT)
#入り口のホールセンサー用GPIO3を入力で初期化
wp.pinMode(EXT_HOLE_SENSOR, wp.GPIO.INPUT)
#ゲートのスイッチのGPIO4を入力で初期化
wp.pinMode(GATE_SWITCH, wp.GPIO.INPUT)
#ゲートスイッチの割り込み処理を設定
wp.wiringPiISR(GATE_SWITCH, wp.GPIO.INT_EDGE_BOTH, get_push)


while True:
    if wp.digitalRead(ENT_HOLE_SENSOR) == 0:
        os.system('./Nplate_rcg/main')
        Nplate_reader.read_Nplate()
        print(Nplate_reader.Nplate)
        #rubyを使ってSQLserverへ接続して会員認証
        os.system('ruby ruby/DAO/azure_MemberDAO.rb ' + \
                Nplate_reader.Nplate["place_name"] + \
                Nplate_reader.Nplate["hiragana"] +\
                Nplate_reader.Nplate["number"])
        #rubyが出力した認証用sql文の結果のjsonファイルをロード
        f = open("model/member_record.json")        
        member = json.load(f)
        print(member)
        #member内のレコード数が1なら認証完了
        if(len(member["results"]) == 1):
            #ゲートを開ける
            os.system('./motor/gate_open')
            GATE_STATUS=False
            #ゲートを通過するまで待機
            while wp.digitalRead(EXT_HOLE_SENSOR) == 1:
                wp.delay(1)
            #ゲートを占める
            os.system('./motor/gate_close')
            GATE_STATUS=True
        os.remove('./Dataset/4.jpg')        
