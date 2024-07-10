/*
* Project 3 -- Tree-Based Encryption
* Filename: main.cpp
* Student name: Aiden Grajo
*
* Last Changed: 2/29/2024
* This file contains the implementation of the main
* Tree-Based Encryption function.
*
*/

#include <iostream>
#include "bst-student-proj3.h"
using namespace std;

/*main
 * Parameters:
 *     none
 *
 * return value: 0
 *
 * Takes input from cin, then performs different operations
 * based off one of the following commands:
 * i:insert
 * r: remove
 * e: encrypt path
 * d: decrypt path
 * p: print codebook
 * q: quit
 */

int main() {
    EncryptionTree<string> codebook;
    char cmd = '1';
    string operand;

    while(cmd != 'q') {
        cin >> cmd;
        cin.ignore();

        if (cmd == 'i') {
            cin >> operand;
            cin.ignore();

            codebook.insert(operand);
        }
        else if (cmd == 'r') {
            cin >> operand;
            cin.ignore();

            codebook.remove(operand);
        }
        else if (cmd == 'e') {
            string ciphertext;

            cin.ignore();
            cin >> operand;
            cin.ignore();

            if (operand[operand.size() - 1] == '\'') {
                string firstNLast = operand.substr(0, operand.size() - 1);
                ciphertext += codebook.encrypt(firstNLast);
            }
            else {
                ciphertext += codebook.encrypt(operand) + " ";
            }

            while (operand[operand.size() - 1] != '\'') {
                cin >> operand;
                cin.ignore();

                if (operand[operand.size() - 1] == '\'') {
                    string last = operand.substr(0, operand.size() - 1);
                    ciphertext += codebook.encrypt(last);
                }
                else {
                    ciphertext += codebook.encrypt(operand) + " ";
                }
            }
            cout << ciphertext << endl;
        }
        else if (cmd == 'd') {
            cin.ignore();
            do {
                cin >> operand;
                cin.ignore();

                if (codebook.decrypt(operand)) {
                    if (operand[operand.size() - 1] == '\'') {
                        string last = operand.substr(0, operand.size() - 1);
                        cout << *codebook.decrypt(last);
                    }
                    else {
                        cout << *codebook.decrypt(operand);
                    }
                }
                else {
                    cout << "?";
                }

                if (operand[operand.size() - 1] != '\'') {
                    cout << " ";
                }

            } while (operand[operand.size() - 1] != '\'');
            cout << endl;

        }
        else if (cmd == 'p') {
            codebook.printPreorder();
        }
    }

    return 0;
}
