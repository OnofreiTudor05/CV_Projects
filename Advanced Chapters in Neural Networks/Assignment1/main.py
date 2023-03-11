import numpy as np
import matplotlib.pyplot as plt


def generate_points_from_interval(limits):
    x_list = np.random.uniform(limits[0][0], limits[0][1], 50)
    y_list = np.random.uniform(limits[1][0], limits[1][1], 50)
    z_list = np.full((50,), 1)
    points_list = np.stack((x_list, y_list, z_list), axis=-1)
    return points_list


def generate_classes(limits_for_class_1, limits_for_class_2):
    first_class = generate_points_from_interval(limits_for_class_1)
    second_class = generate_points_from_interval(limits_for_class_2)
    return first_class, second_class


def evaluate_value(first_class, second_class, axis_parameters):
    correct = 0
    for point in first_class:
        predicted_value = np.dot(point, axis_parameters)
        if predicted_value < 0:
            correct += 1
    for point in second_class:
        predicted_value = np.dot(point, axis_parameters)
        if predicted_value > 0:
            correct += 1
    return correct / 100


def improve(first_class, second_class, axis_parameters):
    neighbours = []
    for i in range(-1, 2, 1):
        for ii in range(-1, 2, 1):
            for iii in range(-1, 2, 1):
                if i == 0 and ii == 0 and iii == 0:
                    continue
                direction = np.array([i, ii, iii])
                neighbours.append(direction * 0.15)
                neighbours.append(direction * 1)
    neighbours = np.array(neighbours)
    best_neighbour = axis_parameters
    for neighbour in neighbours:
        new_element = axis_parameters + neighbour
        if evaluate_value(first_class, second_class, new_element) > \
                evaluate_value(first_class, second_class, best_neighbour):
            best_neighbour = np.array(new_element)
    return best_neighbour


def hill_climbing(first_class, second_class, axis_parameters, best_p):
    iteration_number = 1200
    axis_parameters = np.array(best_p)
    new_best = np.array(best_p)
    for i in range(iteration_number):
        local = False
        axis_parameters = np.array([np.random.uniform(), np.random.uniform(), np.random.uniform()])
        while not local:
            new_axis = improve(first_class, second_class, axis_parameters)
            if evaluate_value(first_class, second_class, new_axis) > \
                    evaluate_value(first_class, second_class, axis_parameters):
                axis_parameters = np.array(new_axis)
            else:
                local = True
        if evaluate_value(first_class, second_class, axis_parameters) > \
                evaluate_value(first_class, second_class, new_best):
            new_best = np.array(axis_parameters)
    return np.array(new_best)


if __name__ == '__main__':
    first_list, second_list = generate_classes([[0, 45], [0, 100]], [[55, 100], [0, 100]])
    axis_p = np.array([np.random.uniform(), np.random.uniform(), np.random.uniform()])

    best = hill_climbing(first_list, second_list, axis_p, axis_p)
    print(f"Accuracy: {evaluate_value(first_list, second_list, best) * 100}%")
    print(f"a = {best[0]} \nb = {best[1]} \nc = {best[2]}")

    fig = plt.figure()
    ax = fig.add_subplot(111)

    plt.title("Hill climbing")
    plt.plot(*zip(*first_list[:, 0:2]), marker='o', color='r', ls='')
    plt.plot(*zip(*second_list[:, 0:2]), marker='o', color='b', ls='')
    x = np.linspace(0, 100, 200)
    y = -best[0] / best[1] * x - best[2] / best[1]
    plt.plot(x, y, '-g', label='Separation')
    plt.xlim([0, 100])
    plt.ylim([0, 100])
    ax.set_aspect('equal', adjustable='box')
    plt.show()
