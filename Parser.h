#pragma once

#include <string>
#include <vector>

typedef std::vector< std::vector<int> > Matrix;

class Parser
{

public:
    Parser();

    bool readFile(const std::string &path);

    Matrix &getInputMatrix();
    unsigned int getSquareSize() const;

private:
    bool isValidNumber(std::string &str) const;
    bool findForNumberInMatrix(int &index_i, int &index_j) const;
    void generateRandomMatrix(unsigned int size);
    std::vector<int> matrixToVector(const Matrix &matrix, const int squareSize);
    int isSolvable();

private:
    Matrix m_matrix;
    int m_squareSize;

};
