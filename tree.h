/**
 * Title: Lab 4: tree.h
 * Author: Chris Kendall
 * Date: 18 May 2023
 */

#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
using std::string;
using std::ostream;


#include "tree_node.h"


enum TraversalType {INORDER, PREORDER, POSTORDER};


class Tree {
private:
    TreeNode* _root;
    Tree(const Tree& other);
    const Tree& operator=(const Tree& rhs);
    void PreorderTraversal(TreeNode* node, string& result) const;
    void InorderTraversal(TreeNode* node, string& result) const;
    void PostorderTraversal(TreeNode* node, string& result) const;
    void deleteNode(TreeNode* node);
    bool isOperator(const string& token)const;
    void StepByStepHelper(ostream& output, TreeNode* node, bool useLevel, int level, int& idCounter, int parentId) const;
public:
    Tree();
    ~Tree();
    bool BuildTree(const string& postfixExpression);
    string Traverse(TraversalType traversalType)const;
    double Evaluate()const;
    void StepByStepEvaluation(ostream& output, bool useLevel = false)const;
};

#endif //POSTFIX_TO_EXPRESSION_TREE_ASSIGNMENT_TREE_H
