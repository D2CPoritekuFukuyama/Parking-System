# coding: UTF-8
from PIL import Image
from PIL import ImageOps
from keras.preprocessing import image
import numpy as np
from keras.datasets import mnist
from keras.models import model_from_json
from keras.utils import np_utils
from keras.optimizers import Adam
import keras.callbacks
import keras.backend.tensorflow_backend as KTF
import tensorflow as tf

import os.path

class ML_module():
    f_model = './model'
    number_model_filename = 'number_model_100epoch.json'
    hiragana_model_filename = 'hiragana_model_4conv.json'
    number_weights_filename = 'number_param_100epoch.hdf5'
    hiragana_weights_filename = 'hiragana_param_4conv.hdf5'

    def __init__(self):
        global f_model
        #読み込んだナンバープレートの情報を格納
        self.Nplate = {'number':'', 'hiragana':'', 'place_name':''}
        #ひらがな認識の答えに対応したひらがなリストファイルを読み込んで配列に格納
        self.hiragana_list = []
        for line in open('model/hiragana_list.txt', 'r'):
            self.hiragana_list.append(line.replace('\n',''))
        #ひらがなと数字の認識modelのロード
        print("数字モデルのロード")
        self.number_model = model_from_json(\
            open(os.path.join(ML_module.f_model, ML_module.number_model_filename)).read()\
        ) 
        print("ひらがなモデルのロード")
        self.hiragana_model = model_from_json(\
                open(os.path.join(ML_module.f_model, ML_module.hiragana_model_filename)).read()\
        ) 
        self.number_model.compile(loss='categorical_crossentropy',\
                optimizer=Adam(lr=0.001, beta_1=0.5), metrics=['accuracy'])
        self.hiragana_model.compile(loss='categorical_crossentropy',\
                optimizer=Adam(lr=0.001, beta_1=0.5), metrics=['accuracy'])
        print("数字モデルの重みをロード")
        self.number_model.load_weights(os.path.join(ML_module.f_model,ML_module.number_weights_filename))
        print("ひらがなモデルの重みをロード")
        self.hiragana_model.load_weights(os.path.join(ML_module.f_model,ML_module.hiragana_weights_filename))
        print("load complete")
#        quit()

    def __read_number(self):
        dataset =np.empty([0,28,28,1])
        f_dataset = "./Dataset"
        for file_name in range(1, 5):
            image = Image.open(os.path.join(f_dataset, str(file_name)+".jpg"), 'r')
            image = ImageOps.grayscale(image)
            image = image.resize((28, 28))
            image_arr = np.asarray(image)
            image_arr = image_arr / 255.
            image_arr = image_arr.reshape(1, 28, 28, 1)
            dataset = np.append(dataset, image_arr, axis=0)
        classes = self.number_model.predict_classes(dataset, batch_size=4)
        self.Nplate['number'] = str(classes[0]) + str(classes[1]) + str(classes[2]) + str(classes[3])

    def __read_hiragana(self):
        dataset =np.empty([0,28,28,1])
        f_dataset = "./Dataset"
        image = Image.open(os.path.join(f_dataset, "0.jpg"), 'r')
        image = ImageOps.grayscale(image)
        image = image.resize((28, 28))
        image_arr = np.asarray(image)
        image_arr = image_arr / 255.
        image_arr = image_arr.reshape(1, 28, 28, 1)
        dataset = np.append(dataset, image_arr, axis=0)
        classes = self.hiragana_model.predict_classes(dataset, batch_size=4)
        self.Nplate['hiragana'] = self.hiragana_list[classes[0]]

    def __read_place_name(self):
        self.Nplate['place_name'] = '岡山'

    def read_Nplate(self):
        self.__read_number()
        self.__read_hiragana()
        self.__read_place_name()


if __name__=='__main__':
    import ml_module
    image_file = "Dataset/4.jpg"
    Nplate_reader = ml_module.ML_module()
    print('start classtering')
    while True:
        os.system('./Nplate_rcg/main')
        Nplate_reader.read_Nplate()
        print(Nplate_reader.Nplate)
        os.remove('./Dataset/4.jpg')        
    quit()
