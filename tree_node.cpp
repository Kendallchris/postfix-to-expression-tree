/**
 * Title: Lab 4: tree_node.cpp
 * Author: Chris Kendall
 * Date: 18 May 2023
 */

#include "tree_node.h"
#include <cmath>

using std::invalid_argument;

/**
 * Default Constructor.
 */
TreeNode::TreeNode(): _left(nullptr), _right(nullptr) {

}

/**
 * Constructor. Constructs the TreeNode, given data, left, and right parameters.
 * @param data the string data to be stored in _data
 * @param left the TreeeNode pointer to be stored as _left child
 * @param right the TreeeNode pointer to be stored as _right child
 */
TreeNode::TreeNode(const string& data, TreeNode* left, TreeNode* right): _data(data), _left(left), _right(right) {

}

/**
 * Copy constructor. In order to process potential child nodes, this is a recursive method that will call itself
 * on child nodes until it reached nullptr indicating the end of the tree.
 * @param other the TreeNode to be copied to the newly constructed TreeNode
 */
TreeNode::TreeNode(const TreeNode &other): _data(other._data) {
    if (other._left != nullptr) {
        this->_left = new TreeNode(*other._left);
    } else {
        this->_left = nullptr;
    }

    if (other._right != nullptr) {
        this->_right = new TreeNode(*other._right);
    } else {
        this->_right = nullptr;
    }
}

/**
 * Copy Assignment Operator.
 * @param other the TreeNode for 'this' to be set to.
 * @return the TreeNode 'this' which now = 'other'
 */
TreeNode &TreeNode::operator=(const TreeNode& other) {
    if (this != &other) {
        delete _left;
        delete _right;

        _data = other._data;
        if (other._left) {
            _left = new TreeNode(*other._left);
        } else {
            _left = nullptr;
        }
        if (other._right) {
            _right = new TreeNode(*other._right);
        } else {
            _right = nullptr;
        }
    }

    return *this;
}

/**
 * Destructor (not really needed here..."default")
 */
TreeNode::~TreeNode() = default;

/**
 * Data Accessor.
 * @return _data, the string containing the data of the Treenode
 */
string TreeNode::getData() const {
    return _data;
}

/**
 * Left child Accessor.
 * @return _left, the left child TreeNode
 */
TreeNode *TreeNode::getLeftChild() const {
    return _left;
}

/**
 * Right child Accessor.
 * @return _right, the right child TreeNode
 */
TreeNode *TreeNode::getRightChild() const {
    return _right;
}

/**
 * This method evaluates a single node, returning its evaluation as a double.
 * @return a double representation of the value evaluated in the node
 */
double TreeNode::Evaluate() const {
    // Operand nodes just return their value.
    if (!isOperator(_data)) {
        return stod(_data);
    }

    double leftValue = 0;
    double rightValue = 0;

    if (_left) {
        leftValue = _left->Evaluate();
    }

    if (_right) {
        rightValue = _right->Evaluate();
    }

    // Determine which operation to perform based on the value in _data
    switch (_data[0]) {
        case '+':
            return leftValue + rightValue;
        case '-':
            return leftValue - rightValue;
        case '*':
            return leftValue * rightValue;
        case '/':
            // check for division by zero.
            if (rightValue == 0) {
                throw invalid_argument("Division by zero");
            }
            return leftValue / rightValue;
        case '^':
            return pow(leftValue, rightValue);
        default:
            throw invalid_argument("Invalid operator");
    }
}

/**
 * This is a helper method mainly for StepByStepHelper in tree.cpp. It is the same as the isOperator method in tree.h.
 * @param op the string to be compared to all acceptable operators
 * @return true if is an accepted operator, false otherwise
 */
bool TreeNode::isOperator(const string& op) {
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^") {
        return true;
    }
    return false;

}
