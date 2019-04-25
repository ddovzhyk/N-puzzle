#pragma once
#include <iostream>
#include <vector>

typedef std::vector< std::vector<int> > Matrix;

class Node
{

public:
    Node() = delete;
    Node(Matrix matrix, int moveCount = 0, int heuristicValue = 0, unsigned int size = 0, const Node *parent = nullptr);
    Node(const Node &obj);

    Matrix getMatrix() const;
    int getSize() const;

    int getHeuristicValue() const;
    int getMoveCount() const;

    int getIndex0_i() const;
    int getIndex0_j() const;

    const Node *getParent() const;

    bool operator ==(const Node &node2) const;
    friend bool operator != (const Node &node1, const Node &node2);
    friend bool operator < (const Node &node1, const Node &node2);
    friend std::ostream& operator<< (std::ostream &out, const Node& node);

    static bool s_greedySearch;

private:
    Matrix m_matrix;
    unsigned int m_size;

    int m_moveCount;
    int m_heuristicValue;

    int m_index0_i = 0;
    int m_index0_j = 0;

    const Node *m_parent;
};
