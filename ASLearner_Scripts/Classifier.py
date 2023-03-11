import tensorflow
from keras.layers import Conv2D, MaxPool2D, Dense, MaxPooling2D, Dropout
from sklearn.model_selection import train_test_split
import os
from PIL import Image
from tensorflow import convert_to_tensor
import numpy as np
import cv2 as cv


class Model:
    def __init__(self, data_path, train_path, size):
        self.model = None
        self.data_directory = data_path
        self.train_path = train_path
        self.x_train = []
        self.y_train = []
        self.x_test = []
        self.y_test = []
        self.x_validation = []
        self.y_validation = []
        self.x = []
        self.y = []
        self.training_paths = []
        self.validation_paths = []
        self.size = size
        self.key_label = {"A": 0,
                          "B": 1,
                          "C": 2,
                          "D": 3,
                          "E": 4,
                          "F": 5,
                          "G": 6,
                          "H": 7,
                          "I": 8,
                          "J": 9,
                          "K": 10,
                          "L": 11,
                          "M": 12,
                          "N": 13,
                          "O": 14,
                          "P": 15,
                          "Q": 16,
                          "R": 17,
                          "S": 18,
                          "T": 19,
                          "U": 20,
                          "V": 21,
                          "W": 22,
                          "X": 23,
                          "Y": 24,
                          "Z": 25,
                          "del": 26,
                          "nothing": 27,
                          "space": 28
                          }
        self.label_key = {0: "A",
                          1: "B",
                          2: "C",
                          3: "D",
                          4: "E",
                          5: "F",
                          6: "G",
                          7: "H",
                          8: "I",
                          9: "J",
                          10: "K",
                          11: "L",
                          12: "M",
                          13: "N",
                          14: "O",
                          15: "P",
                          16: "Q",
                          17: "R",
                          18: "S",
                          19: "T",
                          20: "U",
                          21: "V",
                          22: "W",
                          23: "X",
                          24: "Y",
                          25: "Z",
                          26: "del",
                          27: "nothing",
                          28: "space"
                          }

    def empty_containers(self):
        self.x_train = []
        self.y_train = []
        self.x_test = []
        self.y_test = []
        self.x = []
        self.y = []

    def read_image(self, file_path, resize=None, color_mode=None):
        image = Image.open(file_path)
        if color_mode:
            if color_mode == "grayscale":
                color_mode = "L"
            elif color_mode == "rgb":
                color_mode = "RGB"
            image = image.convert(color_mode)
        if resize:
            image = image.resize(resize)
        image = np.asarray(image)
        image = image / 255
        return image

    def load_data(self, start_index, final_index):
        train_directory_path = os.path.join(self.data_directory, self.train_path)
        category_list = os.listdir(train_directory_path)
        for label_name in category_list:
            for i in range(start_index, final_index + 1):
                image_name = str(label_name) + str(i) + '.jpg'
                image_path = train_directory_path + '\\' + str(label_name) + '\\' + image_name

                self.x.append(image_path)
                self.y.append(self.key_label[label_name])
        print(f"Loaded data size: {len(self.x)}")
        print(f"Loaded label size: {len(self.y)}")

    def create_model(self):
        self.model = Sequential()

        self.model.add(Conv2D(32, (5, 5), activation="relu", kernel_initializer='random_normal', input_shape=(28, 28, 1)))
        self.model.add(MaxPooling2D(2, 2))

        self.model.add(Conv2D(64, (5, 5), activation="relu", kernel_initializer='random_normal'))
        self.model.add(MaxPooling2D(2, 2))

        self.model.add(Dense(1024, activation="relu", kernel_initializer='random_normal'))
        self.mode.add(Dropout(0.2))
        self.model.add(Dense(25, activation="softmax", kernel_initializer='random_normal'))

        self.model.compile(optimizer="adam",
                           loss="sparse_categorical_crossentropy",
                           metrics=['accuracy'])

    def save_dataset_to_file(self, file_path, dataset_files, dataset_labels):
        size = len(dataset_files)
        with open(file_path, 'a') as file_handler:
            for i in range(size):
                file_handler.write(dataset_files[i] + " " + str(dataset_labels[i]) + '\n')

    def split_data(self):
        self.x = np.array(self.x)
        self.y = np.array(self.y)
        self.x_train, self.x_test, self.y_train, self.y_test = train_test_split(self.x,
                                                                                self.y,
                                                                                train_size=0.8)
        self.x_train = np.asarray(self.x_train)
        self.y_train = np.asarray(self.y_train)
        self.x_test = np.asarray(self.x_test)
        self.y_test = np.asarray(self.y_test)
        #self.save_dataset_to_file("training.txt", self.x_train, self.y_train)
        #self.save_dataset_to_file("validation.txt", self.x_test, self.y_test)

    def preprocess_images(self,
                          train_path_folder,
                          validation_path_folder,
                          processed_train_path,
                          processed_validation_path):
        processing_object = cv.createCLAHE()
        file_handler = open(train_path_folder, "r")
        lines = file_handler.readlines()
        n = len(lines)
        file_handler.close()
        for line in lines:
            line = line.strip().split(" ")
            image_path = line[0].split("\\")
            image = cv.imread(line[0], 0)
            processed_image = processing_object.apply(image)

            new_path = processed_train_path + "\\" + image_path[-2] + "\\" + image_path[-1]
            directory_name = processed_train_path + "\\" + image_path[-2]
            try:
                os.mkdir(directory_name)
            except FileExistsError:
                pass
            cv.imwrite(new_path, processed_image)

        file_handler = open(validation_path_folder, "r")
        lines = file_handler.readlines()
        n = len(lines)
        file_handler.close()
        for line in lines:
            line = line.strip().split(" ")
            image_path = line[0].split("\\")
            image = cv.imread(line[0], 0)
            processed_image = processing_object.apply(image)

            new_path = processed_validation_path + "\\" + image_path[-2] + "\\" + image_path[-1]
            directory_name = processed_validation_path + "\\" + image_path[-2]
            try:
                os.mkdir(directory_name)
            except FileExistsError:
                pass
            cv.imwrite(new_path, processed_image)

    def test_model(self, file_path):
        predicted_true = 0
        predicted_false = 0
        total_samples = 0

        processing_object = cv.createCLAHE()

        file_handler = open(file_path, "r")
        lines = file_handler.readlines()
        n = len(lines)
        file_handler.close()
        for line in lines:
            line = lines[total_samples].strip().split(" ")
            image_path = line[0]
            label = int(line[1])

            image = cv.imread(image_path, 0)
            scale_percent = 25
            width = int(image.shape[1] * scale_percent / 100)
            height = int(image.shape[0] * scale_percent / 100)
            dim = (width, height)

            resized = cv.resize(image, dim)
            processed_image = processing_object.apply(resized)
            processed_image = np.array(processed_image)
            data = convert_to_tensor(np.expand_dims(processed_image, 0), dtype=tensorflow.float32)

            predicted = self.predict(data)
            if predicted == self.label_key[label]:
                predicted_true += 1
            else:
                predicted_false += 1
            total_samples += 1

        print(f"Accuracy: {predicted_true / total_samples * 100}%")

    def train_model(self):
        callback = tensorflow.keras.callbacks.EarlyStopping(monitor='accuracy',
                                                            min_delta=0,
                                                            patience=20,
                                                            verbose=1,
                                                            mode='auto')
        processing_object = cv.createCLAHE()

        for j in range(1, 4):
            self.empty_containers()
            self.load_data((j-1) * 1000 + 1, j * 1000)
            self.split_data()

            train_dataset_x = []
            validation_dataset_x = []
            size = len(self.x_train)
            for i in range(size):
                # train_dataset_x.append(np.expand_dims(self.read_image(self.x_train[i], (200, 200), "rgb"), axis=0))
                image = cv.imread(self.x_train[i], 0)
                scale_percent = 25
                width = int(image.shape[1] * scale_percent / 100)
                height = int(image.shape[0] * scale_percent / 100)
                dim = (width, height)

                resized = cv.resize(image, dim)
                processed_image = processing_object.apply(resized)
                processed_image = np.array(processed_image)

                train_dataset_x.append(processed_image)

            size = len(self.x_test)
            for i in range(size):
                image = cv.imread(self.x_test[i], 0)
                scale_percent = 25
                width = int(image.shape[1] * scale_percent / 100)
                height = int(image.shape[0] * scale_percent / 100)
                dim = (width, height)

                resized = cv.resize(image, dim)
                processed_image = processing_object.apply(resized)
                processed_image = np.array(processed_image)

                validation_dataset_x.append(processed_image)

                # validation_dataset_x.append(np.expand_dims(self.read_image(self.x_test[i], (200, 200), "rgb"), axis=0))

            train_dataset_x = np.array(train_dataset_x)
            validation_dataset_x = np.array(validation_dataset_x)

            self.model.fit(train_dataset_x,
                           self.y_train,
                           epochs=40,
                           callbacks=[callback],
                           verbose=1)

            train_score = "_train_" + str(self.model.evaluate(train_dataset_x, self.y_train)) + str("_")
            validation_score = "_valid_" + str(self.model.evaluate(validation_dataset_x, self.y_test))

            name = str(f"model{j}" + train_score + validation_score + ".h5")
            self.model.save(name)

    def load_model(self, path, compile_flag):
        self.model = load_model(path, compile=compile_flag)

    def predict(self, image):
        return self.label_key[np.argmax(self.model.predict(image))]
