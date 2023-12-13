/**
 * Title: Lab 4: tree.cpp
 * Author: Chris Kendall
 * Date: 18 May 2023
 */

#include "tree.h"
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <iomanip>

using std::stringstream;
using std::stack;
using std::setprecision;
using std::fixed;

/**
 * Copy constructor of the class. You do not have to implement this method. I would be good exercise, but it is not expected.
 * @param other
 */
Tree::Tree(const Tree &other) = default;

/**
 * Copy assignment operator of the class. You do not have to implement this method. I would be good exercise, but it is not expected.
 * @param rhs
 * @return
 */
const Tree &Tree::operator=(const Tree& rhs) {
    return rhs;
}

/**
 * Default constructor, it only sets the _root to nullptr
 */
Tree::Tree(): _root(nullptr) {

}

/**
 * Destructor. This destructor will recursively delete all nodes in the tree, starting from the root.
 * It does this using the recursive helper method deleteNode.
 */
Tree::~Tree() {
    deleteNode(_root);
}

/**
 * This is one of the most important methods of this assignment. The method takes in a string representing a postfix arithmetic expression.
 * Given this expression the method will build the expression tree. If the method is unable to do it, then it should
 * return false, if it was to successfully create the tree then it should return true. A hint for this method: use a stack.
 * @param postfixExpression the string expression to be stored in a tree
 * @return true if successful in creating the tree. False otherwise
 */
bool Tree::BuildTree(const string& postfixExpression) {
    stack<TreeNode*> stk;

    // Split postfixExpression by spaces
    stringstream ss(postfixExpression);
    string token;
    while (getline(ss, token, ' ')) {
        if (isOperator(token)) {  // operator
            if (stk.size() < 2) {
                return false;
            }
            TreeNode* node2 = stk.top(); stk.pop();
            TreeNode* node1 = stk.top(); stk.pop();
            TreeNode* node = new TreeNode(token, node1, node2);
            stk.push(node);
        } else {  // operand
            stk.push(new TreeNode(token));
        }
    }

    if (stk.size() != 1) {
        return false;
    }
    _root = stk.top();
    return true;
}

/**
 * This method returns a string with the elements of the tree separated by spaces. The elements will be added to this string according to the
 * type of traversal "requested" by the parameter. Calls recursive helper methods based on the traversal type.
 * @param traversalType the enum indicating the type of traversal to be used
 * @return a string representation of the expression stored in the tree. Returns "invalid traversal type" if out of bounds enum
 */
string Tree::Traverse(TraversalType traversalType) const {
    string result;
    if (traversalType == PREORDER) {
        PreorderTraversal(_root, result);
        // trim trailing space
        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }
    } else if (traversalType == INORDER) {
        InorderTraversal(_root, result);
        // trim trailing space
        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }
    } else if (traversalType == POSTORDER) {
        PostorderTraversal(_root, result);
        // trim trailing space
        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }
    } else {
        return "invalid traversal type";
    }
    return result;
}

/**
 * This method evaluates the expression tree. It calls the Travers method with the POSTORDER parameter, storing
 * its returned string representation of a postorder expression and processes it using a stack. It will return
 * the double resulting from the evaluation.
 * @return the double evaluation of the expression stored in the implied tree parameter. Returns 0 if an error
 * occurs.
 */
double Tree::Evaluate() const {
    string str = this->Traverse(POSTORDER);
    stack<double> myStack;
    string op;

    // traverse through the postfix expression
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        // handle spaces
        if (isspace(c)) {
            // if op has value, push to stack and clear op value
            if (!op.empty()) {
                myStack.push(stod(op));
                op.clear();
            }
            // handle letter inputs
        } else if (isalpha(c)) {
            return 0;
            // if reaches this point it is not a space or letter - checking now if it is a number
        } else if (isalnum(c) || c == '.') {
            op += c;
            // assuming no bad inputs the char must now be an operator
            // If the next character is not a digit or a dot, push the number to the stack
            if (i + 1 == str.size() || (!isdigit(str[i + 1]) && str[i + 1] != '.')) {
                myStack.push(stod(op));
                op.clear();
            }
        } else {
            // store the top 2 double values from the stack into temp variables and remove them from the stack
            double b = myStack.top(); myStack.pop();
            double a = myStack.top(); myStack.pop();

            // depending on the symbol present, handle arithmetic
            // since properly formatted postfix, don't need to worry about precedence of symbols
            switch (c) {
                case '+':
                    myStack.push(a + b); break;
                case '-':
                    myStack.push(a - b); break;
                case '*':
                    myStack.push(a * b); break;
                case '/':
                    // make sure it is not an undefined operation
                    if (b == 0) {
                        return 0; // error
                    }
                    myStack.push(a / b);
                    break;
                case '^':
                    myStack.push(pow(a, b));
                    break;
                    // if it's not one of these symbols (it should be b/c no bad inputs) error by default
                default:
                    return 0; // error
            }
        }
    }

    // if there are still contents in stack after operations,etc. have been used up throw error
    if (myStack.size() != 1) {
        return 0; // error
    }

    return myStack.top();
}

/**
 * This method will show expression tree with its evaluation values. Each evaluation step is represented by a JSON formatted line.
 * The output parameter is where these strings are going to be sent to. The useLevel parameter is used when a _leveled` output is desired.
 * This method relies heavily of the StepByStepHelper() method.
 * @param output the ostream for the strings to be outputted to
 * @param useLevel bool whether or not a _leveled` output is desired
 */
void Tree::StepByStepEvaluation(ostream& output, bool useLevel) const {
    // handle empty tree call
    if (_root == nullptr) {
        return;
    }
    int level = 0;
    int idCounter = 0;
    int parentId = -1;
    StepByStepHelper(output, _root, useLevel, level, idCounter, parentId);
}

/**
 * A helper method that deletes a given node and all its descendants. It does this by recursively deleting the left and
 * right of each node before deleting the node itself.
 * @param node TreeNode* to the node to be deleted
 */
void Tree::deleteNode(TreeNode* node) {
    if (node != nullptr) {
        deleteNode(node->getLeftChild());
        deleteNode(node->getRightChild());
        delete node;
    }
}

/**
 * Helper method to check whether the passed string is an operator or not.
 * @param token string value to be compared to expected operator values
 * @return true if 'token' is an operator, false otherwise
 */
bool Tree::isOperator(const string& token)const {
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
        return true;
    }
    return false;
}

/**
 * This is a helper recursive method to process the Preorder traversal. It will process the tree until all nodes are processed
 * and then return a string representation of all of the nodes' data.
 * @param node The TreeNode* to the node of the tree to be traversed
 * @param result a string representation of all of the nodes' data
 */
void Tree::PreorderTraversal(TreeNode *node, string &result) const {
    // base case
    if (node == nullptr) {
        return;
    }
    result += node->getData() + " ";
    PreorderTraversal(node->getLeftChild(), result);
    PreorderTraversal(node->getRightChild(), result);
}


/**
 * This is a helper recursive method to process the Inorder traversal. It will process the tree until all nodes are processed
 * and then return a string representation of all of the nodes' data.
 * @param node The TreeNode* to the node of the tree to be traversed
 * @param result a string representation of all of the nodes' data
 */
void Tree::InorderTraversal(TreeNode *node, string &result) const {
    if (node == nullptr) {
        return;
    }
    InorderTraversal(node->getLeftChild(), result);
    result += node->getData() + " ";
    InorderTraversal(node->getRightChild(), result);
}

/**
 * This is a helper recursive method to process the Postorder traversal. It will process the tree until all nodes are processed
 * and then return a string representation of all of the nodes' data.
 * @param node The TreeNode* to the node of the tree to be traversed
 * @param result a string representation of all of the nodes' data
 */
void Tree::PostorderTraversal(TreeNode *node, string &result) const {
    if (node == nullptr) {
        return;
    }
    PostorderTraversal(node->getLeftChild(), result);
    PostorderTraversal(node->getRightChild(), result);
    result += node->getData() + " ";
}

/**
 * This method is a helper method that does the heavy lifting for StepByStepEvaluation(). It is a recursive method that
 * outputs (to the specified ostream) the evaluation at each node in a .JSON format.
 * @param output the output stream to out to
 * @param node the TreeNode* to be evaluated
 * @param useLevel bool whether or not to use leveling, representing hierarchy, in the output
 * @param level the int counter of what level the node is at. For determining how many tabs to use when
 * 'useLevel' is true.
 * @param idCounter the int counter of the "ID" of the current node (for the step that we are at)
 * @param parentId the int to the ID of the previous node. This is incremented when the method is called again.
 */
void Tree::StepByStepHelper(ostream& output, TreeNode* node, bool useLevel, int level, int& idCounter, int parentId) const {
    if (node == nullptr) {
        return;
    }

    string operatorChar;
    if (isOperator(node->getData())) {
        operatorChar = node->getData();
    } else {
        operatorChar = "#";
    }
    if (useLevel) {
        for (int i = 0; i < level; i++) {
            output << "\t";
        }
    }

    int currentId = idCounter++;

    output << "{\"value\":";
    if (operatorChar == "#") {
        output << stod(node->getData());
    } else {
        // Compute the value for operator nodes.
        double value = node->Evaluate();
        output << fixed << setprecision(2) << value;
    }
    output << R"(, "operator":")" << operatorChar << "\"";
    output << ", \"operand\":";
    if (operatorChar == "#") {
        output << "true";
    } else {
        output << "false";
    }
    output << ", \"id\":" << currentId;
    output << ", \"parent\":" << parentId;
    output << "}\n";

    StepByStepHelper(output, node->getLeftChild(), useLevel, level + 1, idCounter, currentId);
    StepByStepHelper(output, node->getRightChild(), useLevel, level + 1, idCounter, currentId);

}
