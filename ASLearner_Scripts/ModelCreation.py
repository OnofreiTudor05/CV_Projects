import numpy as np
import pandas
from pathlib import Path
import os.path
import matplotlib.pyplot as plt
import tensorflow as tf
import seaborn as sns
from time import perf_counter
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from IPython.display import Markdown, display
from keras.models import save_model, load_model
from keras.utils.vis_utils import plot_model
import matplotlib as mpl


models = {
    "DenseNet121": {"model": tf.keras.applications.DenseNet121, "perf": 0},
    "DenseNet169": {"model": tf.keras.applications.DenseNet169, "perf": 0},
    "DenseNet201": {"model": tf.keras.applications.DenseNet201, "perf": 0},
    "EfficientNetB0": {"model": tf.keras.applications.EfficientNetB0, "perf": 0},
    "EfficientNetB1": {"model": tf.keras.applications.EfficientNetB1, "perf": 0},
    "EfficientNetB2": {"model": tf.keras.applications.EfficientNetB2, "perf": 0},
    "EfficientNetB3": {"model": tf.keras.applications.EfficientNetB3, "perf": 0},
    "EfficientNetB4": {"model": tf.keras.applications.EfficientNetB4, "perf": 0},
    "EfficientNetB5": {"model": tf.keras.applications.EfficientNetB5, "perf": 0},
    "EfficientNetB6": {"model": tf.keras.applications.EfficientNetB6, "perf": 0},
    "EfficientNetB7": {"model": tf.keras.applications.EfficientNetB7, "perf": 0},
    "InceptionResNetV2": {"model": tf.keras.applications.InceptionResNetV2, "perf": 0},
    "InceptionV3": {"model": tf.keras.applications.InceptionV3, "perf": 0},
    "ResNet101": {"model": tf.keras.applications.ResNet101, "perf": 0},
    "ResNet101V2": {"model": tf.keras.applications.ResNet101V2, "perf": 0},
    "ResNet152": {"model": tf.keras.applications.ResNet152, "perf": 0},
    "ResNet152V2": {"model": tf.keras.applications.ResNet152V2, "perf": 0},
    "ResNet50": {"model": tf.keras.applications.ResNet50, "perf": 0},
    "ResNet50V2": {"model": tf.keras.applications.ResNet50V2, "perf": 0},
    "VGG16": {"model": tf.keras.applications.VGG16, "perf": 0},
    "VGG19": {"model": tf.keras.applications.VGG19, "perf": 0},
    "Xception": {"model": tf.keras.applications.Xception, "perf": 0},
    "MobileNet": {"model": tf.keras.applications.MobileNet, "perf": 0},
    "MobileNetV2": {"model": tf.keras.applications.MobileNetV2, "perf": 0},
    "MobileNetV3Large": {"model": tf.keras.applications.MobileNetV3Large, "perf": 0},
    "MobileNetV3Small": {"model": tf.keras.applications.MobileNetV3Small, "perf": 0},
    "NASNetMobile": {"model": tf.keras.applications.NASNetMobile, "perf": 0},
}


class Model:
    def __init__(self, dataset_path):
        self.dataset_path = dataset_path
        self.file_paths = self.get_image_paths_from_directory()
        self.train_dataframe = None
        self.test_dataframe = None
        self.train_generator = None
        self.test_generator = None
        self.train_images = None
        self.validation_images = None
        self.test_images = None
        self.model = None

    def get_image_paths_from_directory(self):
        directory = Path(self.dataset_path)
        file_paths = list(directory.glob(r'**/*.jpg'))
        return file_paths

    def get_dataframe_from_paths(self):
        total_files = len(self.file_paths)
        labels = [str(self.file_paths[index]).split("\\")[-2] for index in range(total_files)]

        filepath_dataframe = pandas.Series(self.file_paths, name='Filepath').astype(str)
        labels_dataframe = pandas.Series(labels, name='Label')

        dataframe = pandas.concat([filepath_dataframe, labels_dataframe], axis=1)
        dataframe = dataframe.sample(frac=1, random_state=0).reset_index(drop=True)
        return dataframe

    def split_dataset(self, dataframe, fraction, test_fraction):
        self.train_dataframe, self.test_dataframe = train_test_split(dataframe.sample(frac=fraction),
                                                                     test_size=test_fraction,
                                                                     random_state=0)

    def create_generator(self):
        self.train_generator = tf.keras.preprocessing.image.ImageDataGenerator(
            preprocessing_function=tf.keras.applications.mobilenet_v2.preprocess_input,
            validation_split=0.1
        )

        self.test_generator = tf.keras.preprocessing.image.ImageDataGenerator(
            preprocessing_function=tf.keras.applications.mobilenet_v2.preprocess_input
        )

        self.train_images = self.train_generator.flow_from_dataframe(
            dataframe=self.train_dataframe,
            x_col='Filepath',
            y_col='Label',
            target_size=(224, 224),
            color_mode='rgb',
            class_mode='categorical',
            batch_size=32,
            shuffle=True,
            seed=0,
            subset='training',
            #         rotation_range=30, # Uncomment to use data augmentation
            #         zoom_range=0.15,
            #         width_shift_range=0.2,
            #         height_shift_range=0.2,
            #         shear_range=0.15,
            #         horizontal_flip=True,
            #         fill_mode="nearest"
        )

        self.validation_images = self.train_generator.flow_from_dataframe(
            dataframe=self.train_dataframe,
            x_col='Filepath',
            y_col='Label',
            target_size=(224, 224),
            color_mode='rgb',
            class_mode='categorical',
            batch_size=32,
            shuffle=True,
            seed=0,
            subset='validation',
            #         rotation_range=30, # Uncomment to use data augmentation
            #         zoom_range=0.15,
            #         width_shift_range=0.2,
            #         height_shift_range=0.2,
            #         shear_range=0.15,
            #         horizontal_flip=True,
            #         fill_mode="nearest"
        )

        self.test_images = self.test_generator.flow_from_dataframe(
            dataframe=self.test_dataframe,
            x_col='Filepath',
            y_col='Label',
            target_size=(224, 224),
            color_mode='rgb',
            class_mode='categorical',
            batch_size=32,
            shuffle=False
        )

    def create_model(self, pretrained_model):
        kwargs = {'input_shape': (224, 224, 3),
                  'include_top': False,
                  'weights': 'imagenet',
                  'pooling': 'avg'}

        pretrained_model = pretrained_model(**kwargs)
        pretrained_model.trainable = False

        inputs = pretrained_model.input

        x = tf.keras.layers.Dense(128, activation='relu')(pretrained_model.output)
        x = tf.keras.layers.Dense(128, activation='relu')(x)

        outputs = tf.keras.layers.Dense(29, activation='softmax')(x)

        self.model = tf.keras.Model(inputs=inputs, outputs=outputs)

        self.model.compile(
            optimizer='adam',
            loss='categorical_crossentropy',
            metrics=['accuracy']
        )

    def fit_model(self, name, save_file_path):
        start_time = perf_counter()
        history = self.model.fit(self.train_images, validation_data=self.validation_images, epochs=1, verbose=0)
        end_time = perf_counter()

        fit_time = end_time - start_time
        duration = round(fit_time, 2)
        models[name]['perf'] = duration
        print(f"{name:20} trained in {duration} sec")
        val_acc = history.history['val_accuracy']
        models[name]['val_acc'] = [round(v, 4) * 100 for v in val_acc]

        results = {
            "Model Name": name,
            "Accuracy": models[name]['val_acc'][-1],
            "Training time(sec)": models[name]['perf']
        }

        dataframe_results = pandas.DataFrame([results],
                                             columns=['Model Name', 'Accuracy', 'Training time(sec)'])

        if os.path.exists(save_file_path):
            dataframe_results.to_csv(save_file_path, mode='a', index=False, header=False)
        else:
            open(save_file_path, 'w')
            dataframe_results.to_csv(save_file_path, mode='a', index=False, header=True)

    @staticmethod
    def print_markDown(string):
        display(Markdown(string))

    @staticmethod
    def display_dataset_information(dataframe):
        print(f'Number of pictures in the dataset: {dataframe.shape[0]}\n')
        print(f'Number of different labels: {len(dataframe.Label.unique())}\n')
        print(f'Labels: {dataframe.Label.unique()}')

        dataframe.head(5)

        vc = dataframe['Label'].value_counts()
        plt.figure(figsize=(20, 5))
        sns.barplot(x=sorted(vc.index), y=vc, palette="rocket")
        plt.title("Number of pictures of each category", fontsize=15)
        plt.show()

        fig, axes = plt.subplots(nrows=4, ncols=6, figsize=(15, 7),
                                 subplot_kw={'xticks': [], 'yticks': []})

        for i, ax in enumerate(axes.flat):
            ax.imshow(plt.imread(dataframe.Filepath[i]))
            ax.set_title(dataframe.Label[i], fontsize=15)
        plt.tight_layout(pad=0.5)
        plt.show()

    @staticmethod
    def display_results_after_phase(file_path):
        dataframe = pandas.read_csv(file_path)
        dataframe.sort_values(by='Accuracy', ascending=False, inplace=True)
        dataframe.reset_index(inplace=True, drop=True)

        fig, ax = plt.subplots(figsize=(15, 10))
        sns.set(font_scale=1.5)
        plt.xlim(0, 100)
        ax.grid(zorder=-1)
        ax.set_axisbelow(True)
        bar_chart = sns.barplot(x='Accuracy', y='Model Name', data=dataframe, palette="RdYlGn_r")
        bar_chart.set_ylabel("Model Name", fontsize=24)
        bar_chart.set_xlabel("Accuracy(%)", fontsize=24)

        ax.set_xticks(range(0, 100, 5))
        ax.xaxis.label.set_color('blue')
        ax.yaxis.label.set_color('blue')

        plt.tight_layout()
        plt.show()
        mpl.rcParams.update(mpl.rcParamsDefault)

        fig, ax = plt.subplots(figsize=(15, 10))
        maximum_time = int(dataframe['Training time(sec)'].max()) + 5
        maximum_time = maximum_time - maximum_time % 5
        step = maximum_time // 20
        step = step - step % 5 + 5

        sns.set(font_scale=1.5)
        plt.xlim(0, 130)
        ax.grid(zorder=-1)
        ax.set_axisbelow(True)
        bar_chart = sns.barplot(x='Training time(sec)', y='Model Name', data=dataframe, palette="RdYlGn_r")
        bar_chart.set_ylabel("Model Name", fontsize=24)
        bar_chart.set_xlabel("Training time(sec)", fontsize=24)

        ax.set_xticks(range(0, maximum_time, step))
        ax.xaxis.label.set_color('blue')
        ax.yaxis.label.set_color('blue')

        plt.tight_layout()
        plt.show()

    def fit_final_model(self, pretrained_model, dataframe, save_final_path):
        self.split_dataset(dataframe, 1, 0.1)
        self.create_generator()

        self.create_model(eval("tf.keras.applications." + pretrained_model))

        start_time = perf_counter()

        history = self.model.fit(self.train_images,
                                 validation_data=self.validation_images,
                                 epochs=5,
                                 callbacks=[
                                     tf.keras.callbacks.EarlyStopping(
                                         monitor='val_loss',
                                         patience=1,
                                         restore_best_weights=True)]
                                 )
        end_time = perf_counter()

        fig, axes = plt.subplots(2, 1, figsize=(15, 10))
        ax = axes.flat

        pandas.DataFrame(history.history)[['accuracy', 'val_accuracy']].plot(ax=ax[0])
        ax[0].set_title("Accuracy", fontsize=15)
        ax[0].set_ylim(0, 1.1)

        pandas.DataFrame(history.history)[['loss', 'val_loss']].plot(ax=ax[1])
        ax[1].set_title("Loss", fontsize=15)
        plt.show()

        predictions = self.model.predict(self.test_images)
        predictions = np.argmax(predictions, axis=1)

        labels = self.train_images.class_indices
        labels = dict((v, k) for k, v in labels.items())
        predictions = [labels[k] for k in predictions]

        y_test = list(self.test_dataframe.Label)
        accuracy = accuracy_score(y_test, predictions)

        save_model(self.model, f"{pretrained_model}_{str(accuracy * 100)[0:4]}.h5")

        duration = end_time - start_time
        results = {
            "Model Name": pretrained_model,
            "Accuracy": accuracy * 100,
            "Training time(sec)": duration
        }

        dataframe_results = pandas.DataFrame([results],
                                             columns=['Model Name', 'Accuracy', 'Training time(sec)'])

        computed_confusion_matrix = confusion_matrix(y_test, predictions, normalize='true')
        plt.figure(figsize=(17, 12))
        sns.heatmap(computed_confusion_matrix,
                    annot=True,
                    xticklabels=sorted(set(y_test)),
                    yticklabels=sorted(set(y_test)),
                    cbar=False)
        plt.title('Normalized Confusion Matrix', fontsize=23)
        plt.xticks(fontsize=12, rotation=45)
        plt.yticks(fontsize=12)
        plt.show()

        if os.path.exists(save_final_path):
            dataframe_results.to_csv(save_final_path, mode='a', index=False, header=False)
        else:
            open(save_final_path, 'w')
            dataframe_results.to_csv(save_final_path, mode='a', index=False, header=True)

    @staticmethod
    def show_model(model_path):
        model = load_model(model_path)
        model.summary()


def main():
    model_name = "MobileNet"
    model_object = Model(r'E:/Facultate/3/Bachelor/Kaggle_dataset/asl_alphabet_train/')
    # model_object.show_model("MobileNet_99.4.h5", "MobileNetArchitecture.png")
    # dataset = model_object.get_image_paths_from_directory()
    # dataframe = model_object.get_dataframe_from_paths()
    # model_object.fit_final_model(model_name, dataframe, "SecondTest.csv")

    # model_object.display_dataset_information(dataframe)

    # model_object.split_dataset(dataframe, 0.05, 0.2)
    # model_object.create_generator()
    # model_object.create_model(models[model_name]["model"])
    # model_object.fit_model(model_name, "FirstTest.csv")

    # model_object.display_results_after_phase("_training_exploration/FirstTest.csv")
    model_object.show_model("MobileNet_99.4.h5")


main()
