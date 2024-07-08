import random
import matplotlib.pyplot as plt
from Visual_and_tools import draw_picture, draw_descend

class ACO(object):
    def __init__(self, ant_num: int, iteration: int, alpha: float, beta: float, rho: float,
                 q: float, dots, distance, pheromone):
        self.Q = q
        self.rho = rho
        self.alpha = alpha
        self.beta = beta
        self.ant_num = ant_num
        self.iteration = iteration
        self.dots = dots
        self.distance = distance
        self.pheromone = pheromone

        self.descend_dis = []

    def initialization(self):
        self.ant_paths = []
        ant_position = [random.randint(0, len(self.dots) - 1) for i in range(self.ant_num)]
        for i in range(len(ant_position)):
            self.ant_paths.append([ant_position[i]])
        self.ant_total_distance = [0.0] * self.ant_num
    def update_pheromone(self):
        # 蚂蚁走完了，按照蚁周，给信息素更新
        for i in range(len(self.dots)):
            for j in range(len(self.dots)):
                if i != j:
                    self.pheromone[i][j] *= self.rho

        for i in range(len(self.ant_paths)):
            path = self.ant_paths[i]
            for j in range(len(path)-1):
                r = path[j]
                e = path[j+1]
                self.pheromone[r][e] += self.Q / self.ant_total_distance[i]
                self.pheromone[e][r] += self.Q / self.ant_total_distance[i]
    def run(self):
        best_distance = float('inf')
        best_path = []

        plt.ion()
        # 迭代次数
        for iter in range(self.iteration):
            self.initialization()
            visited = [[False] * len(self.dots) for _ in range(self.ant_num)]
            for i in range(len(visited)):
                visited[i][self.ant_paths[i][0]] = True
            # 每只蚂蚁同时走一步
            for step in range(len(self.dots)-1):
                # 每只
                for i in range(self.ant_num):
                    current_pos = self.ant_paths[i][-1]
                    index = []
                    probabilities = []
                    total_prob = 0.0
                    for j in range(len(self.dots)):  # 当前点到j点的所有倾向
                        if visited[i][j]:
                            continue
                        index.append(j)
                        probability = (self.pheromone[current_pos][j] ** self.alpha) * (
                                    (1.0 / self.distance[current_pos][j]) ** self.beta)  # 有个bug，万一random生成了两个下标一样的点
                        probabilities.append(probability)
                        total_prob += probability
                    probabilities = [p / total_prob for p in probabilities]

                    # 轮盘赌法选择下一步,然后更新访问状态、蚂蚁总距离、蚂蚁路径
                    cumulative_prob = [sum(probabilities[:i + 1]) for i in range(len(probabilities))]
                    score = random.random()
                    chosen_index = next(i for i, cp in enumerate(cumulative_prob) if cp >= score)
                    chosen_index = index[chosen_index]

                    visited[i][chosen_index] = True
                    self.ant_total_distance[i] += self.distance[current_pos][chosen_index]
                    self.ant_paths[i].append(chosen_index)

            # 回路，忘记算最后一个点回去了
            for i in range(len(self.ant_paths)):
                self.ant_paths[i].append(self.ant_paths[i][0])
            # 更新信息素并比较最优路径
            self.update_pheromone()
            for i in range(self.ant_num):
                if self.ant_total_distance[i] < best_distance:
                    best_path = [self.ant_paths[i][j] for j in range(len(self.ant_paths[i]))]
                    best_distance = self.ant_total_distance[i]
            self.descend_dis.append(best_distance)

            plt.cla()
            draw_picture(self.dots,self.ant_paths,iter,best_distance)
            plt.pause(0.1)

        plt.ioff()
        plt.show()
        draw_descend(self.descend_dis, self.iteration)