#include "Node.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool Node::s_greedySearch = false;

Node::Node(Matrix matrix, int moveCount, int heuristicValue, unsigned int size, const Node *parent)
    : m_matrix(matrix)
    , m_size(size)
    , m_moveCount(moveCount)
    , m_heuristicValue(heuristicValue)
    , m_index0_i(0)
    , m_index0_j(0)
    , m_parent(parent)
{
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_matrix[i][j] == 0) {
                m_index0_i = i;
                m_index0_j = j;
            }
        }
    }
}

Node::Node(const Node &obj)
{
    m_matrix = obj.m_matrix;
    m_size = obj.m_size;
    m_moveCount= obj.m_moveCount;
    m_heuristicValue = obj.m_heuristicValue;
    m_index0_i = obj.m_index0_i;
    m_index0_j = obj.m_index0_j;
    m_parent = obj.m_parent;
}

Matrix Node::getMatrix() const
{
    return m_matrix;
}

int Node::getSize() const
{
    return m_size;
}

int Node::getHeuristicValue() const
{
    return m_heuristicValue;
}

int Node::getMoveCount() const
{
    return m_moveCount;
}

int Node::getIndex0_i() const
{
    return m_index0_i;
}

int Node::getIndex0_j() const
{
    return m_index0_j;
}

const Node* Node::getParent() const
{
    return m_parent;
}

bool Node::operator ==(const Node &node2) const
{
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_matrix[i][j] != node2.m_matrix[i][j]) {
                return false;
            }
        }
    }

    return true;
}

bool operator != (const Node &node1, const Node &node2)
{
    return !(node1 == node2);
}

bool operator < (const Node &node1, const Node &node2)
{
    return Node::s_greedySearch
                ? node1.m_heuristicValue > node2.m_heuristicValue
                : node1.m_moveCount + node1.m_heuristicValue > node2.m_moveCount + node2.m_heuristicValue;
}

std::ostream& operator<< (std::ostream &out, const Node& node)
{
//    out << "Matrix:" << std::endl;
    for (int i = 0; i < node.getSize(); ++i) {
        for (int j = 0; j < node.getSize(); ++j) {
            out << node.m_matrix[i][j] << " ";
        }
        out << std::endl;
    }
//    out << "Move Count: " << node.m_moveCount << std::endl;
//    out << "Heuristic Value: " << node.m_heuristicValue << std::endl;
//    out << std::endl;

    return out;
}
