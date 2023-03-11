import gzip
import pickle
import numpy as np
import random as rnd
import pandas as pd


def initialize_random(perceptron, biases):
    n = len(perceptron)
    m = len(perceptron[0])
    for i in range(0, n):
        for j in range(0, m):
            perceptron[i][j] = rnd.random()

    n = len(biases)
    for i in range(0, n):
        biases[i] = rnd.random()


def main():
    with gzip.open('mnist.pkl.gz', 'rb') as fd:
        train_set, valid_set, test_set = pickle.load(fd, encoding='latin')

    perceptron = np.zeros([10, 784])
    biases = np.zeros(10)
    initialize_random(perceptron, biases)
    learning_rate = 0.005
    iteration_number = 100
    all_classified = False

    training_images = np.array(train_set[0])
    training_labels = np.array(train_set[1])
    total_training = len(training_images)

    p = np.random.permutation(total_training)
    training_images = training_images[p]
    training_labels = training_labels[p]

    while iteration_number > 0:

        for j in range(0, total_training):
            input_image = np.array(training_images[j])
            label = training_labels[j]

            label1 = [-1 for i in range(0, 10)]
            label1[label] = 1
            label1 = np.array(label1)
            label1.reshape(10, 1)

            z = np.zeros([10, 1])
            z = np.sum(perceptron * input_image, axis=1)
            z = np.sum([biases, z], axis=0)

            dif_output = label1 - z

            biases += dif_output * learning_rate

            factor = input_image * learning_rate

            product = dif_output.reshape(10, 1) * factor.reshape(1, 784)

            perceptron = np.sum([product, perceptron], axis=0)

        iteration_number = iteration_number - 1

    winner = 0
    test_images = np.array(valid_set[0])
    test_labels = np.array(valid_set[1])
    n = len(test_labels)
    for j in range(0, n):
        input_image = np.array(test_images[j])
        label = test_labels[j]
        all_input = np.zeros(10)
        for i in range(0, 10):
            z = np.dot(input_image, perceptron[i]) + biases[i]
            all_input[i] = z

        max_index = np.argmax(all_input)
        if max_index == label:
            winner = winner + 1

    validation_accuracy = winner / n

    winner = 0
    test_images = np.array(train_set[0])
    test_labels = np.array(train_set[1])
    n = len(test_labels)
    for j in range(0, n):
        input_image = np.array(test_images[j])
        label = test_labels[j]
        all_input = np.zeros(10)
        for i in range(0, 10):
            z = np.dot(input_image, perceptron[i]) + biases[i]
            all_input[i] = z

        max_index = np.argmax(all_input)
        if max_index == label:
            winner = winner + 1
    training_accuracy = winner / n

    dataframe = pd.read_excel("Results.xlsx", sheet_name="Sheet1")
    data_columns = ['Index', 'Training', 'Validation']
    final = pd.DataFrame(dataframe)

    final_list = [len(final.index) + 1, training_accuracy, validation_accuracy]
    dataframe.loc[len(final.index)] = final_list
    dataframe.to_excel('Results.xlsx', columns=data_columns, index=False)


main()
