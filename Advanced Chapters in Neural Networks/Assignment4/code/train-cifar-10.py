#!/usr/bin/env python
# coding: utf-8

# In[1]:


get_ipython().run_line_magic('matplotlib', 'inline')
import matplotlib.pyplot as plt

from tensorflow.keras import optimizers
import tensorflow as tf
from tensorflow.keras.datasets import cifar10
import numpy as np
from keras import models
from keras import layers
from tensorflow.keras import regularizers
from keras.utils import to_categorical
from tensorflow.keras.models import load_model
from keras.preprocessing.image import ImageDataGenerator
import random
import numpy as np


# In[2]:


(train_images, train_labels), (test_images, test_labels) = cifar10.load_data()

train_images = train_images.astype("float32")
test_images = test_images.astype("float32")

train_labels = to_categorical(train_labels)
test_labels = to_categorical(test_labels)  

datagen = ImageDataGenerator(width_shift_range=0.1, 
                             height_shift_range=0.1, 
                             horizontal_flip=True)
augmented_dataset = datagen.flow(train_images, train_labels, batch_size=64)


# In[3]:


network = models.Sequential()

network.add(layers.Rescaling(scale=1.0/255, input_shape=(32, 32, 3)))
network.add(layers.Conv2D(32, (3, 3), padding='same', kernel_initializer='he_uniform', input_shape=(32, 32, 3), activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.Conv2D(32, (3, 3), padding='same', kernel_initializer='he_uniform', activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.MaxPooling2D(pool_size=(2, 2))) 
network.add(layers.Dropout(0.2))
network.add(layers.Conv2D(64, (3, 3), padding='same', kernel_initializer='he_uniform', activation='relu'))
network.add(layers.BatchNormalization())
network.add(layers.Conv2D(64, (3, 3), padding='same', kernel_initializer='he_uniform', activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.Dropout(0.3)) 
network.add(layers.MaxPooling2D(pool_size=(2, 2)))
network.add(layers.Conv2D(128, (3, 3), padding='same', kernel_initializer='he_uniform', activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.Conv2D(128, (3, 3), padding='same', kernel_initializer='he_uniform', activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.MaxPooling2D(pool_size=(2, 2))) 
network.add(layers.Dropout(0.4))
network.add(layers.Flatten())
network.add(layers.Dense(128, activation='relu')) 
network.add(layers.BatchNormalization())
network.add(layers.Dropout(0.5)) 
network.add(layers.Dense(10, activation='softmax'))

AdamOpt = optimizers.Adam(lr=0.001)
network.compile(optimizer=AdamOpt, loss='categorical_crossentropy', metrics=['accuracy'])


# In[4]:


batch_size = 64
epochs_number = 400
steps = int(train_labels.shape[0] / batch_size)
history = network.fit_generator(augmented_dataset, steps_per_epoch=steps, epochs=epochs_number, validation_data=(test_images, test_labels))


# In[5]:


acc = history.history['accuracy']
val_acc = history.history['val_accuracy']
loss = history.history['loss']
val_loss = history.history['val_loss']
epochs = range(1, len(acc)+1)
plt.plot(epochs, acc, 'bo', label='Training accuracy')
plt.plot(epochs, val_acc, 'b', label='Validation accuracy')
plt.title('Validation and training accuracy')
plt.legend()
plt.figure()

plt.plot(epochs, loss, 'bo', label='Training loss')
plt.plot(epochs, val_loss, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.legend()
plt.show()


# In[2]:


(X_train, Y_train), (X_test, Y_test) = cifar10.load_data() 
Y_test = to_categorical(Y_test)

network = load_model("model.h5")
scores = network.evaluate(X_test, Y_test)
network.summary()

print('Loss: %.3f' % scores[0])
print('Accuracy: %.3f' % scores[1])

network.save("model.h5")


# In[ ]:




