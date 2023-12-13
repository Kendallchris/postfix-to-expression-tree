#include "tree.h"

#include <iostream>
#include <string>
#include <sstream>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

bool DoubleEquals(double a, double b, double epsilon = 1e-6);

int main(int argc, char* argv[]) {
    bool verbose = false, extra = false;
    if (argc == 2 && string(argv[1]) == "-v")
        verbose = true;
    else if (argc == 2 && string(argv[1]) == "-x"){
        extra = true;
        verbose = true;
    }

    const size_t TESTS = 10;
    const string expressions[TESTS] = {
            "2.1 4.5 +",
            "12 3 /",
            "2.5 3.5 + 4.0 8 - *",
            "1 2 + 3 *",
            "125 5.0 / 3.0 *",
            "7 5 + 5 2 - /",
            "1 7 2 - 5 1 - * 5 + 2 / +",
            "4 3 + 2 5 * 1 / -",
            "6 5.2 2.2 - * 2 5 ^ +",
            "2 2 2 3 3 * + + +"
    };
    const double evaluationValues[TESTS] = {
            6.6,
            4.0,
            -24,
            9.0,
            75.0,
            4.0,
            13.5,
            -3.0,
            50.0,
            15.0
    };
    const string expectedTraversals[TESTS][3] = {
            {
                    {"2.1 + 4.5"},
                    {"+ 2.1 4.5"},
                    {"2.1 4.5 +"}
            },
            {
                    {"12 / 3"},
                    {"/ 12 3"},
                    {"12 3 /"}
            },
            {
                    {"2.5 + 3.5 * 4.0 - 8"},
                    {"* + 2.5 3.5 - 4.0 8"},
                    {"2.5 3.5 + 4.0 8 - *"}
            },
            {
                    {"1 + 2 * 3"},
                    {"* + 1 2 3"},
                    {"1 2 + 3 *"}
            },
            {
                    {"125 / 5.0 * 3.0"},
                    {"* / 125 5.0 3.0"},
                    {"125 5.0 / 3.0 *"}
            },
            {
                    {"7 + 5 / 5 - 2"},
                    {"/ + 7 5 - 5 2"},
                    {"7 5 + 5 2 - /"}
            },
            {
                    {"1 + 7 - 2 * 5 - 1 + 5 / 2"},
                    {"+ 1 / + * - 7 2 - 5 1 5 2"},
                    {"1 7 2 - 5 1 - * 5 + 2 / +"}
            },
            {
                    {"4 + 3 - 2 * 5 / 1"},
                    {"- + 4 3 / * 2 5 1"},
                    {"4 3 + 2 5 * 1 / -"}
            },
            {
                    {"6 * 5.2 - 2.2 + 2 ^ 5"},
                    {"+ * 6 - 5.2 2.2 ^ 2 5"},
                    {"6 5.2 2.2 - * 2 5 ^ +"}
            },
            {
                    {"2 + 2 + 2 + 3 * 3"},
                    {"+ 2 + 2 + 2 * 3 3"},
                    {"2 2 2 3 3 * + + +"}
            },
    };

    stringstream expectedEvaluationTrees[TESTS] = {
            stringstream (
                    "{\"value\":6.60, \"operator\":\"+\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":2.10, \"operator\":\"#\", \"operand\":true, \"id\":1, \"parent\":0}\n"
                    "{\"value\":4.50, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":0}"
            ),
            stringstream (
                    "{\"value\":4.00, \"operator\":\"/\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":12.00, \"operator\":\"#\", \"operand\":true, \"id\":1, \"parent\":0}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":0}"
            ),
            stringstream (
                    "{\"value\":-24.00, \"operator\":\"*\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":6.00, \"operator\":\"+\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":2.50, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":3.50, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":1}\n"
                    "{\"value\":-4.00, \"operator\":\"-\", \"operand\":false, \"id\":4, \"parent\":0}\n"
                    "{\"value\":4.00, \"operator\":\"#\", \"operand\":true, \"id\":5, \"parent\":4}\n"
                    "{\"value\":8.00, \"operator\":\"#\", \"operand\":true, \"id\":6, \"parent\":4}"
            ),
            stringstream (
                    "{\"value\":9.00, \"operator\":\"*\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":3.00, \"operator\":\"+\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":1.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":1}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":4, \"parent\":0}"
            ),
            stringstream (
                    "{\"value\":75.00, \"operator\":\"*\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":25.00, \"operator\":\"/\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":125.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":1}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":4, \"parent\":0}"
            ),
            stringstream (
                    "{\"value\":4.00, \"operator\":\"/\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":12.00, \"operator\":\"+\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":7.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":1}\n"
                    "{\"value\":3.00, \"operator\":\"-\", \"operand\":false, \"id\":4, \"parent\":0}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":5, \"parent\":4}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":6, \"parent\":4}"
            ),
            stringstream (
                    "{\"value\":13.50, \"operator\":\"+\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":1.00, \"operator\":\"#\", \"operand\":true, \"id\":1, \"parent\":0}\n"
                    "{\"value\":12.50, \"operator\":\"/\", \"operand\":false, \"id\":2, \"parent\":0}\n"
                    "{\"value\":25.00, \"operator\":\"+\", \"operand\":false, \"id\":3, \"parent\":2}\n"
                    "{\"value\":20.00, \"operator\":\"*\", \"operand\":false, \"id\":4, \"parent\":3}\n"
                    "{\"value\":5.00, \"operator\":\"-\", \"operand\":false, \"id\":5, \"parent\":4}\n"
                    "{\"value\":7.00, \"operator\":\"#\", \"operand\":true, \"id\":6, \"parent\":5}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":7, \"parent\":5}\n"
                    "{\"value\":4.00, \"operator\":\"-\", \"operand\":false, \"id\":8, \"parent\":4}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":9, \"parent\":8}\n"
                    "{\"value\":1.00, \"operator\":\"#\", \"operand\":true, \"id\":10, \"parent\":8}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":11, \"parent\":3}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":12, \"parent\":2}"
            ),
            stringstream (
                    "{\"value\":-3.00, \"operator\":\"-\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":7.00, \"operator\":\"+\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":4.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":1}\n"
                    "{\"value\":10.00, \"operator\":\"/\", \"operand\":false, \"id\":4, \"parent\":0}\n"
                    "{\"value\":10.00, \"operator\":\"*\", \"operand\":false, \"id\":5, \"parent\":4}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":6, \"parent\":5}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":7, \"parent\":5}\n"
                    "{\"value\":1.00, \"operator\":\"#\", \"operand\":true, \"id\":8, \"parent\":4}"
            ),
            stringstream (
                    "{\"value\":50.00, \"operator\":\"+\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":18.00, \"operator\":\"*\", \"operand\":false, \"id\":1, \"parent\":0}\n"
                    "{\"value\":6.00, \"operator\":\"#\", \"operand\":true, \"id\":2, \"parent\":1}\n"
                    "{\"value\":3.00, \"operator\":\"-\", \"operand\":false, \"id\":3, \"parent\":1}\n"
                    "{\"value\":5.20, \"operator\":\"#\", \"operand\":true, \"id\":4, \"parent\":3}\n"
                    "{\"value\":2.20, \"operator\":\"#\", \"operand\":true, \"id\":5, \"parent\":3}\n"
                    "{\"value\":32.00, \"operator\":\"^\", \"operand\":false, \"id\":6, \"parent\":0}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":7, \"parent\":6}\n"
                    "{\"value\":5.00, \"operator\":\"#\", \"operand\":true, \"id\":8, \"parent\":6}"
            ),
            stringstream (
                    "{\"value\":15.00, \"operator\":\"+\", \"operand\":false, \"id\":0, \"parent\":-1}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":1, \"parent\":0}\n"
                    "{\"value\":13.00, \"operator\":\"+\", \"operand\":false, \"id\":2, \"parent\":0}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":3, \"parent\":2}\n"
                    "{\"value\":11.00, \"operator\":\"+\", \"operand\":false, \"id\":4, \"parent\":2}\n"
                    "{\"value\":2.00, \"operator\":\"#\", \"operand\":true, \"id\":5, \"parent\":4}\n"
                    "{\"value\":9.00, \"operator\":\"*\", \"operand\":false, \"id\":6, \"parent\":4}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":7, \"parent\":6}\n"
                    "{\"value\":3.00, \"operator\":\"#\", \"operand\":true, \"id\":8, \"parent\":6}"
            ),

    };
    const size_t evaluationStepsCount[] = {3, 3, 7, 5, 5, 7, 13, 9, 9, 9};

    Tree* et = nullptr;
    double passed = 0, stepsPassed;
    for (size_t i = 0; i < TESTS; i++ ){
        string e = expressions[i];
        string traversals[3];
        string producedEvaluationStep, expectedEvaluationStep;
        stringstream output;
        double evaluation;
        et = new Tree;
        bool ok = et->BuildTree(e);
        if (ok){
            traversals[INORDER] = et->Traverse(INORDER);
            traversals[PREORDER] = et->Traverse(PREORDER);
            traversals[POSTORDER] = et->Traverse(POSTORDER);
            evaluation = et->Evaluate();

            if (verbose){
                cout << "Input Expression: " << e << endl;
                cout << "\tTraversals" << endl;
                cout << "\t\tYour INORDER:        " << traversals[INORDER] << endl;
                cout << "\t\tExpected INORDER:    " << expectedTraversals[i][INORDER] << endl;
                cout << "\t\tYour PREORDER:       " << traversals[PREORDER] << endl;
                cout << "\t\tExpected PREORDER:   " << expectedTraversals[i][PREORDER] << endl;
                cout << "\t\tYour POSTORDER:      " << traversals[POSTORDER] << endl;
                cout << "\t\tExpected POSTORDER:  " << expectedTraversals[i][POSTORDER] << endl;
                cout << endl;
                cout << "\tEvaluation" << endl;
                cout << "\t\tYour Evaluation:     " << evaluation << endl;
                cout << "\t\tExpected Evaluation: " << evaluationValues[i] << endl;
            }else{

            }
            for (int t = 0; t < 3; t++){
                if (traversals[t] == expectedTraversals[i][t])
                    passed++;
            }

            if (DoubleEquals(evaluation, evaluationValues[i]))
                passed+=2;

            et->StepByStepEvaluation(output);
            if (verbose) cout << endl << "\tEvaluation Steps" << endl;
            stepsPassed = 0;
            while (getline(output, producedEvaluationStep) && getline(expectedEvaluationTrees[i], expectedEvaluationStep)){
                // while (output >> producedEvaluationStep && expectedEvaluationTrees[i] >> expectedEvaluationStep){
                if (verbose){
                    cout << "\t\tYour Step:     " << producedEvaluationStep << endl;
                    cout << "\t\tExpected Step: " << expectedEvaluationStep;
                }
                if (producedEvaluationStep == expectedEvaluationStep)
                    stepsPassed++;
                else {
                    if (verbose) cout << "\t<<" << string(10, '*');
                }
                cout << endl;
            }
            if (verbose){
                cout << "\tSteps passed: " << stepsPassed << " Total steps: " << evaluationStepsCount[i] << endl;
                cout << string(60, '-') << endl << endl;
            }
            passed += (stepsPassed / evaluationStepsCount[i]) * 5;
            if (extra){
                cout << string(50, '-') << endl;
                et->StepByStepEvaluation(cout);
                cout << endl << string(50, '=') << endl << endl;
                et->StepByStepEvaluation(cout,true);
                cout << string(50, '-') << endl;
            }
        }else{
            cerr << "Error building tree" << endl;
            delete et;
            exit(1);
        }
        delete et;
    }

    cout << (verbose ? "Passed Tests: ": "") << passed << endl;


    return 0;
}

bool DoubleEquals(double a, double b, double epsilon){
    return abs(a - b) < epsilon;
}