import heapq
import math


class Node:
    def __init__(self, x, y, g, h, parent):
        self.x = x
        self.y = y
        self.g = g
        self.h = h
        self.parent = parent

    def f(self):
        return self.g + self.h
    
    def __str__(self):
        return f'({self.x}, {self.y}), g={self.g}, h={self.h}'
    
    def __lt__(self, other):
        return self.f() > other.f()


class Grid:
    def __init__(self, numRows, numCols):
        self.numRows = numRows
        self.numCols = numCols
        self.grid = [[0 for _ in range(numCols)] for _ in range(numRows)]

    def setObstacle(self, row, col):
        self.grid[row][col] = 1

    def isObstacle(self, row, col):
        return self.grid[row][col] == 1


class AStar:
    def __init__(self, grid, start_x, start_y, goal_x, goal_y):
        self.grid = grid
        self.scale_coeff = grid.numRows * grid.numCols
        self.start = Node(start_x, start_y, 0, self.calculate_heuristic(start_x, start_y, goal_x, goal_y), None)
        self.goal = Node(goal_x, goal_y, 0, 0, None)
        self.open_set = []
        self.path = None
        heapq.heappush(self.open_set, (self.start.f(), self.start))

    def calculate_heuristic(self, x1, y1, x2, y2):
        return self.scale_coeff * math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

    def find_neighbors(self, current, goal):
        neighbors = []
        dx = [1, -1, 0, 0]
        dy = [0, 0, 1, -1]

        for i in range(4):
            x, y = current.x + dx[i], current.y + dy[i]
            if x >= 0 and x < self.grid.numRows and y >= 0 and y < self.grid.numCols and not self.grid.isObstacle(x, y):
                g = current.g + 1
                h = self.calculate_heuristic(x, y, goal.x, goal.y)
                neighbor = Node(x, y, g, h, current)
                neighbors.append(neighbor)
        return neighbors

    def findPath(self):
        while self.open_set:
            _, current = heapq.heappop(self.open_set)

            if current.x == self.goal.x and current.y == self.goal.y:
                return self.reconstruct_path(current)

            neighbors = self.find_neighbors(current, self.goal)
            for neighbor in neighbors:
                self.evaluate_neighbor(neighbor, current)

    def evaluate_neighbor(self, neighbor, current):
        if neighbor.f() < current.f():
            neighbor.parent = current
            neighbor.g = current.g + 1
            neighbor.h = self.calculate_heuristic(neighbor.x, neighbor.y, self.goal.x, self.goal.y)
            heapq.heappush(self.open_set, (neighbor.f(), neighbor))

    def reconstruct_path(self, node):
        path = []
        while node:
            path.append((node.x, node.y))
            node = node.parent
        self.path = list(reversed(path))
        return self.path
    
    def getPath(self):
        return self.path
