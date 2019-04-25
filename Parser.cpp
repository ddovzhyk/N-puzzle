#include "Parser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

Parser::Parser()
    : m_matrix()
    , m_squareSize(-1)
{
    srand(time(0));
}

void Parser::generateRandomMatrix(unsigned int size)
{
    Matrix matrix(size, std::vector<int>(size));
    int n = 0;

    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            matrix[i][j] = n;
            n++;
        }
    }

    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            int index_i = rand() % matrix.size();
            int index_j = rand() % matrix.size();

            int tmp = matrix[i][j];
            matrix[i][j] = matrix[index_i][index_j];
            matrix[index_i][index_j] = tmp;
        }
    }

    m_matrix = std::move(matrix);
}

std::vector<int> Parser::matrixToVector(const Matrix &matrix, const int squareSize)
{
    std::vector<int> vector;

    int startRowIndex = 0, startColumnIndex = 0,
            endRowIndex = squareSize, endColumnIndex = squareSize;

    while (startRowIndex < endRowIndex && startColumnIndex < endColumnIndex) {
        // Print the first row from the remaining rows
        for (int i = startColumnIndex; i < endColumnIndex; ++i) {
            vector.push_back(matrix[startRowIndex][i]);
        }
        startRowIndex++;

        // Print the last column from the remaining columns
        for (int i = startRowIndex; i < endRowIndex; ++i) {
            vector.push_back(matrix[i][endColumnIndex - 1]);
        }
        endColumnIndex--;

        // Print the last row from the remaining rows
        if ( startRowIndex < endRowIndex) {
            for (int i = endColumnIndex - 1; i >= startColumnIndex; --i) {
                vector.push_back(matrix[endRowIndex - 1][i]);
            }
            endRowIndex--;
        }

        // Print the first column from the remaining columns
        if (startColumnIndex < endColumnIndex) {
            for (int i = endRowIndex - 1; i >= startRowIndex; --i) {
                vector.push_back(matrix[i][startColumnIndex]);
            }
            startColumnIndex++;
        }
    }

    return vector;
}

int Parser::isSolvable()
{
    std::vector<int> inputVector = matrixToVector(m_matrix, m_squareSize);

    int vectorSize = m_squareSize * m_squareSize;
    int inputInv = 0;
    int inputRowEmptyCell = 0;

    for (int i = 0; i < vectorSize - 1; ++i) {
        if (inputVector[i] == 0) {
            inputRowEmptyCell = i + 1;
        }
        for (int j = i + 1; j < vectorSize; ++j) {
            if (inputVector[i] != 0 && inputVector[j] != 0 && inputVector[i] > inputVector[j]) {
                ++inputInv;
            }
        }
    }

    if (m_squareSize % 2 == 0) {
        inputInv += inputRowEmptyCell;
    }

    return inputInv % 2 == 0;
}

bool Parser::readFile(const std::string &path)
{
    if (path == "") {
        m_squareSize = 3;
        do {
            generateRandomMatrix(m_squareSize);
        } while (!isSolvable());

        return true;
    }

    std::ifstream inputFile(path);
    std::string line;
    std::string tmpStr;

    if (!inputFile) {
        std::cerr << "ERROR: Cannot open file!" << std::endl;
        return false;
    }
    try {
        // parse line with size of square
        while (std::getline(inputFile, line)) {
            // cut comment which starts with '#'
            size_t p = line.find_first_of('#');
            if (p != std::string::npos) {
                line.erase(p);
            }

            if (line.empty()) {
                continue;
            }

            std::istringstream iss(line);

            if (iss >> tmpStr) {
                if (isValidNumber(tmpStr)) {
                    m_squareSize = std::stoi(tmpStr);
                } else {
                    std::cerr << "ERROR: Size of square is not a number!" << std::endl;
                    return false;
                }

                if (iss >> tmpStr) {
                    std::cerr << "ERROR: Presence of extra character after the size!" << std::endl;
                    return false;
                }
                break;
            }
        }

        if (m_squareSize == -1) {
            std::cerr << "ERROR: No size of square!" << std::endl;
            return false;
        }

        if (m_squareSize < 2) {
            std::cerr << "ERROR: Size of square must be greater than 1!" << std::endl;
            return false;
        }

        m_matrix = std::move(Matrix(m_squareSize, std::vector<int>(m_squareSize)));

        int index_i = 0;

        // parse cells with numbers
        while (std::getline(inputFile, line) && index_i < m_squareSize) {
            int index_j = 0;

            // cut comment which starts with '#'
            size_t p = line.find_first_of('#');
            if (p != std::string::npos) {
                line.erase(p);
            }

            if (line.empty()) {
                continue;
            }

            std::istringstream iss(line);

            while ((iss >> tmpStr) && (index_j < m_squareSize)) {
                if (isValidNumber(tmpStr)) {
                    m_matrix[index_i][index_j] = std::stoi(tmpStr);

                    // the upper number could be sizeOfSquare^2 - 1
                    if (m_matrix[index_i][index_j] >= m_squareSize * m_squareSize) {
                        std::cerr << "ERROR: Too large number in row: " << index_i + 1 << "!" << std::endl;
                        return false;
                    }

                    if (findForNumberInMatrix(index_i, index_j)) {
                        std::cerr << "ERROR: Found double of numbers!" << std::endl;
                        return false;
                    }
                } else {
                    std::cerr << "ERROR: Incorrect numbers in row: " << index_i + 1 << "!" << std::endl;
                    return false;
                }

                ++index_j;
            }

            if (iss >> tmpStr) {
                std::cerr << "ERROR: Presence of extra character after the numbers in row: " << index_i + 1 << "!" << std::endl;
                return false;
            }

            if (index_j < m_squareSize) {
                std::cerr << "ERROR: Not enough numbers in row: " << index_i + 1 << "!" << std::endl;
                return false;
            }

            ++index_i;
        }

        if (index_i < m_squareSize) {
            std::cerr << "ERROR: Not enough rows with numbers!" << std::endl;
            return false;
        }

        while (std::getline(inputFile, line)) {
            // cut comment which starts with '#'
            size_t p = line.find_first_of('#');
            if (p != std::string::npos) {
                line.erase(p);
            }

            std::istringstream iss(line);

            if (iss >> tmpStr) {
                std::cerr << "ERROR: Presence of extra character after all numbers!" << std::endl;
                return false;
            }
        }

        if (!isSolvable()) {
            std::cerr << "ERROR: Input puzzle is not solvable!" << std::endl;
            return false;
        }
    } catch (std::bad_alloc) {
        std::cerr << "ERROR: Too large size of square, cannot allocate memory!" << std::endl;
        return false;
    } catch (...) {
        std::cerr << "ERROR: Uknown error!" << std::endl;
        return false;
    }

    return true;
}

bool Parser::isValidNumber(std::string &str) const
{
    // check for digits
    if (str.find_first_not_of("0123456789") != std::string::npos) {
        return false;
    }

    // remove '0' at start
    while (str.size() > 1 && str[0] == '0') {
        str.erase(0, 1);
    }

    // check for upper limit
    if ((str.size() == 10 && str > "2147483647") || str.size() > 10) {
        return false;
    }

    return true;
}

bool Parser::findForNumberInMatrix(int &index_i, int &index_j) const
{
    int number = m_matrix[index_i][index_j];

    // find for elem in the same row
    for (int i = index_i, j = index_j - 1; j >= 0; --j) {
        if (m_matrix[i][j] == number) {
            return true;
        }
    }

    // find for elem in the previous part of matrix
    for (int i = index_i - 1; i >= 0; --i) {
        for (int j = m_squareSize - 1; j >= 0; --j) {
            if (m_matrix[i][j] == number) {
                return true;
            }
        }
    }

    return false;
}

Matrix &Parser::getInputMatrix()
{
    return m_matrix;
}

unsigned int Parser::getSquareSize() const
{
    return static_cast<unsigned int>(m_squareSize);
}
