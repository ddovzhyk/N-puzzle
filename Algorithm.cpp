#include "Algorithm.h"

#include <vector>
#include <list>
#include <queue>
#include <unordered_set>
#include <cmath>

Algorithm::Algorithm(const unsigned int squareSize, HeuristicFunctionType type)
    : m_squareSize(squareSize)
    , m_heuristicFunctionType(type)
    , m_solutionMatrix(getSpiralFilledMatrix())
{
}

Matrix Algorithm::getSpiralFilledMatrix()
{
    const int higherValue = m_squareSize * m_squareSize;

    int n = 0, startRowIndex = 0, startColumnIndex = 0,
            endRowIndex = m_squareSize, endColumnIndex = m_squareSize;

    Matrix matrix(m_squareSize, std::vector<int>(m_squareSize));

    while (startRowIndex < endRowIndex && startColumnIndex < endColumnIndex) {
        // Print the first row from the remaining rows
        for (int i = startColumnIndex; i < endColumnIndex; ++i) {
            matrix[startRowIndex][i] = ++n % higherValue;
        }
        startRowIndex++;

        // Print the last column from the remaining columns
        for (int i = startRowIndex; i < endRowIndex; ++i) {
            matrix[i][endColumnIndex - 1] = ++n % higherValue;
        }
        endColumnIndex--;

        // Print the last row from the remaining rows
        if ( startRowIndex < endRowIndex) {
            for (int i = endColumnIndex - 1; i >= startColumnIndex; --i) {
                matrix[endRowIndex - 1][i] = ++n % higherValue;
            }
            endRowIndex--;
        }

        // Print the first column from the remaining columns
        if (startColumnIndex < endColumnIndex) {
            for (int i = endRowIndex - 1; i >= startRowIndex; --i) {
                matrix[i][startColumnIndex] = ++n % higherValue;
            }
            startColumnIndex++;
        }
    }

    return matrix;
}

void Algorithm::findCellInSolution(int number, int &x, int &y)
{
    Matrix matrix = m_solutionMatrix;
    for (int i = 0; i < m_squareSize; ++i) {
        for (int j = 0; j < m_squareSize; ++j) {
            if (matrix[i][j] == number) {
                x = i;
                y = j;
                return ;
            }
        }
    }
}

int Algorithm::calcManhattanDistanceFunc(const Matrix &matrix)
{
    int value = 0;

    for (int i1 = 0; i1 < m_squareSize; ++i1) {
        for (int j1 = 0; j1 < m_squareSize; ++j1) {
            int i2, j2;
            findCellInSolution(matrix[i1][j1], i2, j2);
            value += abs(i2 - i1) + abs(j2 - j1);
        }
    }

    return value;
}

int Algorithm::calcSquareMissplaceFunc(const Matrix &matrix)
{
    int value = 0;

    for (int i1 = 0; i1 < m_squareSize; ++i1) {
        for (int j1 = 0; j1 < m_squareSize; ++j1) {
            int i2, j2;
            findCellInSolution(matrix[i1][j1], i2, j2);
            if (i1 != i2 || j1 != j2) {
                ++value;
            }
        }
    }

    return value;
}

int Algorithm::calcEuclidianDistanceFunc(const Matrix &matrix)
{
    int value = 0;

    for (int i1 = 0; i1 < m_squareSize; ++i1) {
        for (int j1 = 0; j1 < m_squareSize; ++j1) {
            int i2, j2;
            findCellInSolution(matrix[i1][j1], i2, j2);
            value += static_cast<int>(sqrt((i2 - i1)*(i2 - i1) + (j2 - j1)*(j2 - j1)));
        }
    }

    return value;
}

int Algorithm::calculateHeuristicValue(const Matrix &matrix)
{
    switch (m_heuristicFunctionType) {
        case HeuristicFunctionType::MANHATTAN_DISTANCE:
            return calcManhattanDistanceFunc(matrix);
        case HeuristicFunctionType::SQUARE_MISSPLACE:
            return calcSquareMissplaceFunc(matrix);
        case HeuristicFunctionType::EUCLIDIAN_DISTANCE:
            return calcEuclidianDistanceFunc(matrix);
        case HeuristicFunctionType::UNIFORM_COST:
            return 0;
    }
    return 0;
}

void Algorithm::runAStarAlgorithm(Matrix inputMatrix)
{
    unsigned int maxMemoryComplexity = 1;
    std::priority_queue<Node> openList;
    std::unordered_set<Node> closeList;

    const Node *lastElem = nullptr;

    openList.push(Node(inputMatrix, 0, 0, m_squareSize, nullptr));

    while (true) {
        std::pair<std::unordered_set<Node>::iterator, bool> pair = closeList.insert(openList.top());
        if (!pair.second) {
            openList.pop();
            continue;
        }
        openList.pop();

        auto iter = pair.first;
        const Node &currentNode = *iter;
        lastElem = &currentNode;

        if (currentNode.getMatrix() == m_solutionMatrix) {
            break;
        }

        const int index0_i = currentNode.getIndex0_i();
        const int index0_j = currentNode.getIndex0_j();

        if (index0_i - 1 >= 0) {
            Matrix newMatrix = currentNode.getMatrix();
            swapElements(newMatrix, index0_i, index0_j, index0_i - 1, index0_j);
            int heuristicValue = calculateHeuristicValue(newMatrix);
            Node tmpNode(newMatrix, iter->getMoveCount() + 1, heuristicValue, m_squareSize, &(*iter));

            if (closeList.find(tmpNode) == closeList.end()) {
                openList.push(tmpNode);
            }
        }

        if (index0_j - 1 >= 0) {
            Matrix newMatrix = currentNode.getMatrix();
            swapElements(newMatrix, index0_i, index0_j, index0_i, index0_j - 1);
            int heuristicValue = calculateHeuristicValue(newMatrix);
            Node tmpNode(newMatrix, iter->getMoveCount() + 1, heuristicValue, m_squareSize, &(*iter));

            if (closeList.find(tmpNode) == closeList.end()) {
                openList.push(tmpNode);
            }
        }

        if (index0_i + 1 < m_squareSize) {
            Matrix newMatrix = currentNode.getMatrix();
            swapElements(newMatrix, index0_i, index0_j, index0_i + 1, index0_j);
            int heuristicValue = calculateHeuristicValue(newMatrix);
            Node tmpNode(newMatrix, iter->getMoveCount() + 1, heuristicValue, m_squareSize, &(*iter));

            if (closeList.find(tmpNode) == closeList.end()) {
                openList.push(tmpNode);
            }
        }

        if (index0_j + 1 < m_squareSize) {
            Matrix newMatrix = currentNode.getMatrix();
            swapElements(newMatrix, index0_i, index0_j, index0_i, index0_j + 1);
            int heuristicValue = calculateHeuristicValue(newMatrix);
            Node tmpNode(newMatrix, iter->getMoveCount() + 1, heuristicValue, m_squareSize, &(*iter));

            if (closeList.find(tmpNode) == closeList.end()) {
                openList.push(tmpNode);
            }
        }

        unsigned int memComplex = closeList.size() + openList.size();
        maxMemoryComplexity = memComplex > maxMemoryComplexity ? memComplex : maxMemoryComplexity;
    }

    std::cout << "Sequence of states:" << std::endl;

    const Node *p = lastElem;
    std::list< const Node * > solutionList;

    while (p) {
        solutionList.push_front(p);
        p = p->getParent();
    }

    for (const Node * node : solutionList) {
        std::cout << *node;
        std::cout << std::endl;
    }

    std::cout << "Complexity in time: " << closeList.size() << std::endl;
    std::cout << "Complexity in size: " << maxMemoryComplexity << std::endl;
    std::cout << "Number of moves: " << lastElem->getMoveCount() << std::endl;
}

void Algorithm::swapElements(Matrix &matrix, int i1, int j1, int i2, int j2)
{
    int n = matrix[i1][j1];
    matrix[i1][j1] = matrix[i2][j2];
    matrix[i2][j2] = n;
}
