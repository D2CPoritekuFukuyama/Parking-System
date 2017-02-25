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

print('setup complete')
f_model = './model'
model_filename = 'number_model.json'
weights_filename = 'number_param.hdf5'
image_file = "3.jpg"

print('load model')
json_string = open(os.path.join(f_model, model_filename)).read()
model = model_from_json(json_string)

print('load weights')
model.compile(loss='categorical_crossentropy', optimizer=Adam(lr=0.001, beta_1=0.5), metrics=['accuracy'])
model.load_weights(os.path.join(f_model,weights_filename))

print('load image')
image = Image.open(image_file, 'r')
image = ImageOps.grayscale(image)
image = image.resize((28, 28))
test_X = np.asarray(image)
test_X = test_X/255.
test_X = test_X.reshape(1, 28, 28,1)
print('start classtering')
classes = model.predict_classes(test_X, batch_size=1)
#for i in range(42):
#    test_Y = np_utils.to_categorical([i],42)
#    score = model.evaluate(test_X, test_Y, verbose=0)
#    if score[1]==1.0:
#        break
print(classes[0])
quit()
