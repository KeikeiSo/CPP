#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <cstdlib>
#include <time.h>

using namespace std;

const int rows = 5;
const int cols = 5;
static int seed = time(nullptr);

// signatures
void checkPassword();
void convertPhonetic();
void fillMatrix(int matrix[rows][cols]);
void printMatrix(int matrix[rows][cols]);
void multiplyMatrices(int matrix_left[rows][cols], 
int matrix_right[rows][cols],
int matrix_result[rows][cols]);

// Q1
void checkPassword() {
    // get the password
    string password;
    cout << "Please enter your password:" << endl;
    cin >> password;
    
    bool wrong = false;
    //Make sure that the password is at least 8 characters long.
    int l = password.length();
    if (l < 8) {
        cout << "Password should be at least 8 characters long." << endl;
        wrong = true;
    }

    // use map to store the chars and their #occurrances 
    map<char, int> m;
    for (int i = 0; i < l; i++) {
        m.emplace(password[i], 0);
    }

    // initialize a boolean to check for count
    bool morethan2 = false;
    // initialize a boolean to check for number
    bool number = false;
    // initialize a boolean to check for *, # or $
    bool spec = false;
    for (int i = 0; i < l; i++) {
        char c = password[i];
        int count = m[c] + 1;
        // check if same char occur more than twice
        if (count > 2) {
            morethan2 = true;
        }
        m[c] = count;

        // turn char into ascii code
        int k = (int)c;
        // check if it is a number
        if (47 < k && k < 58) {
            number = true;
        }
        // check if it is one of *, # or $
        else if (k == 35 || k == 36 || k == 42) {
            spec = true;
        }
    }
    
    // Make sure that each character does not occur more than 2 times in the word
    if (morethan2){
        cout << "Each character can be used at most twice." << endl;
        wrong = true;
    }
    // Make sure the password contains at least one number
    if (!number){
        cout << "There should be a number." << endl;
        wrong = true;
    }
    // Make sure the password contains at least one of *, # or $.
    if (!spec){
        cout << "There should be a *, # or $." << endl;
        wrong = true;
    }

    // Otherwise, print message stating password accepted
    if (!wrong){
        cout << "Your password has been accepted." << endl;
    }
    return;

}

// Q2
void convertPhonetic() {
    string word;
    cout << "Please enter a word: ";
    cin >> word;
    // initialize string array
    string nato[26] = { "Alfa", "Bravo", "Charlie", "Delta", 
    "Echo", "Foxtrot", "Golf", "Hotel", "India", "Juliett", 
    "Kilo", "Lima", "Mike", "November", "Oscar", "Papa", 
    "Quebec", "Romeo", "Sierra", "Tango", "Uniform", 
    "Victor", "Whiskey", "X-ray", "Yankee", "Zulu" };
    // print out the first converted character
    int first = (int)word[0];
    if (isalpha(first)) {
        cout << nato[toupper(first)-65];
    } else {
        cout << (char)first;
    }
    // print out the rest converted characters
    for (int i = 1; i < word.size(); i++) {
        int k = (int)word[i];
        if (isalpha(k)) {
            // print the corresponding nato phonetic alphabet
            cout << " " << nato[toupper(k)-65];
        } else {
            cout << (char)k;
        }
    }
    cout << endl;
}

// Q3.1
void fillMatrix(int matrix[rows][cols]) {
    // make sure the seed is different everytime
    seed ++;
    srand(seed);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // generate random number from 0 to 25
            matrix[i][j] = rand() % 26;
        }
    }
}

// Q3.2
void printMatrix(int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        // print the first element of the row
        int curr = matrix[i][0];
        if (curr > 9){
            cout << curr;
        } else {
            cout << curr << " ";
        }
        // print the rest of the row
        for (int j = 1; j < cols; j++) {
            curr = matrix[i][j];
            if (curr > 9){
                cout << "|" << curr;
            } else {
                cout << "|" << curr << " ";
            }
        }
        cout << endl << "--------------" << endl;
    }
    cout << endl;
}

// Q3.3
// helper static variables
static int rownum = 0;
static int colnum = 0;

void multiplyMatrices(int matrix_left[rows][cols],
int matrix_right[rows][cols],
int matrix_result[rows][cols]) {
    // similar to for each column, iterate through row 
    if (rownum > 4){
        rownum = 0;
        colnum++;
    }
    if (colnum > 4){
        return;
    }

    // using the formula "C[i][j] = sum of A[i][k] * B[k][j] for k = 1 to n"
    int val = 0;
    for (int i = 0; i < 5; i++){
        val += ((matrix_left[rownum][i]) * (matrix_right[i][colnum]));
    }
    matrix_result[rownum][colnum] = val;

    rownum++;
    multiplyMatrices(matrix_left, matrix_right, matrix_result);
}

int main() {
    checkPassword();
    convertPhonetic();
    int matrix[rows][cols];
    int matrix2[rows][cols];
    int matrix_result[rows][cols];
    fillMatrix(matrix);
    fillMatrix(matrix2);
    printMatrix(matrix);
    multiplyMatrices(matrix, matrix2, matrix_result);
    printMatrix(matrix_result);
    return 0;
}
