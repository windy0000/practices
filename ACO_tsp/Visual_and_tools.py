import math
import random
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('TkAgg')

def dis(dot1,dot2):
    return math.sqrt((dot1[0]-dot2[0])**2 + (dot1[1]-dot2[1])**2)
def initial(dot_num):
    dots = []
    distance = []
    for i in range(dot_num):
        x = random.randint(1, 100)
        y = random.randint(1, 100)
        dots.append([x, y])
    pheromone = np.ones((len(dots), len(dots)))
    for i in range(len(dots)):
        i_to = []
        for j in range(len(dots)):
            i_to.append(dis(dots[i], dots[j]))
        distance.append(i_to)
    return dots, distance, pheromone

def draw_picture(dots,ant_paths,iteration,min_cost):
    x = []
    y = []
    for i in range(len(dots)):
        x.append(dots[i][0])
        y.append(dots[i][1])
    plt.scatter(x,y)

    for i in range(len(ant_paths)):
        x = []
        y = []
        for j in range(len(dots)+1):
            x.append(dots[ant_paths[i][j]][0])
            y.append(dots[ant_paths[i][j]][1])
        plt.plot(x,y)
    plt.text(0, 0, f'iteration:{iteration+1} min_cost = {round(min_cost, 2)}', family='fantasy', fontsize=10,
             style='italic', color='mediumvioletred')

def draw_descend(dis,iteration):
    iter_list = [ i for i in range(iteration)]
    plt.plot(iter_list, dis, marker='.')
    plt.show()