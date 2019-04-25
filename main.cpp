#include "Parser.h"
#include "Node.h"
#include "Algorithm.h"
#include <iostream>
#include <queue>
#include <algorithm>

bool parseArgs(int argc, char **argv, HeuristicFunctionType &heuristicType, std::string &fileName)
{
    const std::string errorString =  "usage: ./npuzzle [option] heuristic [file]\n"
                                     "  option:\n"
                                     "    -g       Use greedy search algorithm\n"
                                     "    -u       Use uniform-cost search algorithm. No heuristic option must be!\n"
                                     "  heuristic:\n"
                                     "    -m       Use Manhattan distance heuristic function\n"
                                     "    -p       Use such heuristic function - number of cells that are not in correct place\n"
                                     "    -e       Use Euclidean distance heuristic function\n";

    /*
 *      ./npuzzle -g -m file
 *      ./npuzzle -g -m
 *      ./npuzzle -g -p file
 *      ./npuzzle -g -p
 *      ./npuzzle -g -e file
 *      ./npuzzle -g -e
 *      ./npuzzle -u    file
 *      ./npuzzle -u
 *      ./npuzzle    -m file
 *      ./npuzzle    -m
 *      ./npuzzle    -p file
 *      ./npuzzle    -p
 *      ./npuzzle    -e file
 *      ./npuzzle    -e
 */

    if (argc < 2 || argc > 4) {
        std::cerr << errorString;
        return false;
    }

    if (strcmp(argv[1], "-g") == 0) {
        if (argc == 3 || argc == 4) {
            std::string hOption = argv[2];

            if (hOption == "-m") {
                heuristicType = HeuristicFunctionType::MANHATTAN_DISTANCE;
            } else if (hOption == "-p") {
                heuristicType = HeuristicFunctionType::SQUARE_MISSPLACE;
            } else if (hOption == "-e") {
                heuristicType = HeuristicFunctionType::EUCLIDIAN_DISTANCE;
            } else {
                std::cerr << errorString;
                return false;
            }

            fileName = (argc == 4) ? argv[3] : "";

            Node::s_greedySearch = true;
            return true;
        } else {
            std::cerr << errorString;
        }
    } else if (strcmp(argv[1], "-u") == 0) {
        if (argc == 2 || argc == 3) {
            fileName = (argc == 3) ? argv[2] : "";

            return true;
        } else {
            std::cerr << errorString;
        }
    } else {
        if (argc == 2 || argc == 3) {
            std::string hOption = argv[1];

            if (hOption == "-m") {
                heuristicType = HeuristicFunctionType::MANHATTAN_DISTANCE;
            } else if (hOption == "-p") {
                heuristicType = HeuristicFunctionType::SQUARE_MISSPLACE;
            } else if (hOption == "-e") {
                heuristicType = HeuristicFunctionType::EUCLIDIAN_DISTANCE;
            } else {
                std::cerr << errorString;
                return false;
            }

            fileName = (argc == 3) ? argv[2] : "";

            return true;
        } else {
            std::cerr << errorString;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    std::string fileName;
    HeuristicFunctionType heuristicType;

    if (!parseArgs(argc, argv, heuristicType, fileName)) {
        return -2;
    }

    Parser parser;
    if (!parser.readFile(fileName)) {
        return -1;
    }
    Algorithm algorithm(parser.getSquareSize(), heuristicType);

    algorithm.runAStarAlgorithm(parser.getInputMatrix());

    return 0;
}
