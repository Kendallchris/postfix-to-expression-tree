/**
 * Title: Lab 4: tree_node.h
 * Author: Chris Kendall
 * Date: 18 May 2023
 */

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <string>

using std::string;

// HINT: A Tree Node has three parts: data, left and right. You will need to think about what is going to be the type of data.
//      Remember that you will be storing numbers (operands), operators, evaluation values, etc.

class TreeNode {
    string _data;
    TreeNode* _left;
    TreeNode* _right;
    static bool isOperator(const string& token);
public:
    TreeNode();
    TreeNode(const string& data, TreeNode* left = nullptr, TreeNode* right = nullptr);
    TreeNode(const TreeNode& other);
    TreeNode& operator=(const TreeNode& other);
    ~TreeNode();
    string getData() const;
    TreeNode* getLeftChild() const;
    TreeNode* getRightChild() const;
    double Evaluate() const;
};


#endif //POSTFIX_TO_EXPRESSION_TREE_ASSIGNMENT_TREE_NODE_H
