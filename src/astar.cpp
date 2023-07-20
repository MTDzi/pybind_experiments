#include <iostream>
#include <vector>
#include <queue>
#include <future>
#include <cmath>
#include <algorithm>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


struct Node {
    int x, y;
    double g, h;
    Node* parent;

    Node(int x, int y, double g, double h, Node* parent) : x(x), y(y), g(g), h(h), parent(parent) {}

    double f() const {
        return g + h;
    }
};

struct CompareNodes {
    bool operator()(const Node* a, const Node* b) const {
        return a->f() > b->f();
    }
};

class Grid {
public:
    int numRows, numCols;

    Grid(int numRows, int numCols) : numRows(numRows), numCols(numCols) {
        grid.resize(numRows, std::vector<int>(numCols, 0));
    }

    void setObstacle(int row, int col) {
        grid[row][col] = 1;
    }

    bool isObstacle(int row, int col) const {
        return grid[row][col] == 1;
    }

private:
    std::vector<std::vector<int>> grid;
};

class AStar {
public:
    AStar(const Grid& grid, int startX, int startY, int goalX, int goalY)
        : grid(grid),
          scalingCoeff(grid.numRows * grid.numCols),
          start(startX, startY, 0, calculateHeuristic(startX, startY, goalX, goalY), nullptr),
          goal(goalX, goalY, 0, 0, nullptr) {}

    void findPath() {
        std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openSet;
        openSet.push(&start);

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            if (current->x == goal.x && current->y == goal.y) {
                reconstructPath(current);
                return;
            }

            neighbors.clear();
            findNeighbors(current, goal);
            std::vector<std::future<void>> futures;

            for (const auto& neighbor : neighbors) {
                futures.emplace_back(std::async(std::launch::async, &AStar::evaluateNeighbor, this, neighbor, current, &openSet));
            }

            for (auto& future : futures) {
                future.wait();
            }
        }
    }

    void reconstructPath(Node* current) {
        path.clear();
        while (current != nullptr) {
            path.push_back(std::make_pair(current->x, current->y));
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
    }

    std::vector<std::pair<int, int>> getPath() const {
        return path;
    }

private:
    const Grid& grid;
    int scalingCoeff;
    Node start;
    Node goal;
    std::vector<std::pair<int, int>> path;
    std::vector<Node*> neighbors;
    std::mutex openSetMutex; // Mutex to protect access to the openSet

    double calculateHeuristic(int x1, int y1, int x2, int y2) const {
        return scalingCoeff * std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }

    void findNeighbors(Node* current, const Node& goal) {
        // Left
        if (current->x > 0) {
            Node* neighbor = new Node(current->x - 1, current->y, current->g + 1, calculateHeuristic(current->x - 1, current->y, goal.x, goal.y), current);
            if (!grid.isObstacle(neighbor->x, neighbor->y)) {
                neighbors.push_back(neighbor);
            }
        }

        // Right
        if (current->x < grid.numCols - 1) {
            Node* neighbor = new Node(current->x + 1, current->y, current->g + 1, calculateHeuristic(current->x + 1, current->y, goal.x, goal.y), current);
            if (!grid.isObstacle(neighbor->x, neighbor->y)) {
                neighbors.push_back(neighbor);
            }
        }

        // Up
        if (current->y > 0) {
            Node* neighbor = new Node(current->x, current->y - 1, current->g + 1, calculateHeuristic(current->x, current->y - 1, goal.x, goal.y), current);
            if (!grid.isObstacle(neighbor->x, neighbor->y)) {
                neighbors.push_back(neighbor);
            }
        }

        // Down
        if (current->y < grid.numRows - 1) {
            Node* neighbor = new Node(current->x, current->y + 1, current->g + 1, calculateHeuristic(current->x, current->y + 1, goal.x, goal.y), current);
            if (!grid.isObstacle(neighbor->x, neighbor->y)) {
                neighbors.push_back(neighbor);
            }
        }
    }

    void evaluateNeighbor(Node* neighbor, Node* current, std::priority_queue<Node*, std::vector<Node*>, CompareNodes>* openSet) {
        std::lock_guard<std::mutex> lock(openSetMutex);
        if (neighbor->f() < current->f()) {
            neighbor->parent = current;
            neighbor->g = current->g + 1;
            neighbor->h = calculateHeuristic(neighbor->x, neighbor->y, goal.x, goal.y);
            openSet->push(neighbor);
        }
    }
};


namespace py = pybind11;

PYBIND11_MODULE(astar, m) {

    py::class_<Grid>(m, "Grid")
        .def(py::init<int, int>())
        .def("setObstacle", &Grid::setObstacle)
        .def("isObstacle", &Grid::isObstacle);
    
    py::class_<AStar>(m, "AStar")
        .def(py::init<const Grid &, int, int, int, int>())
        .def("findPath", &AStar::findPath)
        .def("getPath", &AStar::getPath);
}