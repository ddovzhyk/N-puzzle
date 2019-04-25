#pragma once

#include "Node.h"
#include <string>

enum HeuristicFunctionType {
    MANHATTAN_DISTANCE,
    SQUARE_MISSPLACE,
    EUCLIDIAN_DISTANCE,
    UNIFORM_COST
};

template <>
struct std::hash<Node>
{
    size_t
    operator()(const Node & obj) const
    {
        std::string str;
        Matrix m = obj.getMatrix();
        for (int i = 0; i < obj.getSize(); ++i) {
            for (int j = 0; j < obj.getSize(); ++j) {
                 str += std::to_string(m[i][j]);
            }
        }

        return std::hash<std::string>()(str);
    }
};

class Algorithm
{

public:
    Algorithm() = delete;
    Algorithm(const unsigned int squareSize, HeuristicFunctionType type);

    void runAStarAlgorithm(Matrix inputMatrix);
    Matrix getSpiralFilledMatrix();

private:

    int calcManhattanDistanceFunc(const Matrix &matrix);
    int calcSquareMissplaceFunc(const Matrix &matrix);
    int calcEuclidianDistanceFunc(const Matrix &matrix);

    int calculateHeuristicValue(const Matrix &matrix);

    void findCellInSolution(int number, int &x, int &y);
    void swapElements(Matrix &matrix, int i1, int j1, int i2, int j2);

private:
    const unsigned int m_squareSize;
    const HeuristicFunctionType m_heuristicFunctionType;
    const Matrix m_solutionMatrix;
};



