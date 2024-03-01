#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
// Globals
int keyMatrix[2][2] = {
    -1,
};
int inverseKeyMatrix[2][2] = {-1};
int alphaLength = 26;
string original;
//char alphabets[26] = {'#',};


int calcDeterminant()
{
    int a = keyMatrix[0][0];
    int b = keyMatrix[0][1];
    int c = keyMatrix[1][0];
    int d = keyMatrix[1][1];

    return ((a * d) - (b * c)) % alphaLength;
}

int mmi(int a, int m) // modular multiplicative inverse
{
    for (int i = 1; i < m; ++i)
    {
        if (((a * i) % m) == 1)
        {

            return i;
        }
    }
    return -1;
}
string onlyChar(string str)
{

    string chars;
    for (int i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122))

            chars += str[i];
    }
    return chars;
}
string toUpperCase(string str)
{
    string strUpper;
    for (int i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 65 && str[i] <= 90))

            strUpper += str[i];

        else if ((str[i] >= 97 && str[i] <= 122))
            strUpper += char (str[i] - 32);
    }
    return strUpper;
}
bool isChar(char ch){
    if((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
        return true;
    return false;

}
void displayKeyMatrix()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cout << keyMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void generateKeyMatrix()
{
    srand(static_cast<unsigned>(time(0)));
    //srand(time(NULL));
    keyMatrix[0][0] = rand() % alphaLength;
    keyMatrix[0][1] = rand() % alphaLength;
    keyMatrix[1][0] = rand() % alphaLength;
    keyMatrix[1][1] = rand() % alphaLength;

    // checking if the determinant is invertible modulo alphaLength
    int determinant = calcDeterminant();
    while (mmi(determinant, alphaLength) == -1)
    {
        // regenerate key matrix if the determinant is not invertible
        keyMatrix[0][0] = rand() % alphaLength;
        keyMatrix[0][1] = rand() % alphaLength;
        keyMatrix[1][0] = rand() % alphaLength;
        keyMatrix[1][1] = rand() % alphaLength;
        determinant = calcDeterminant();
    }
}

void generateInverseKeyMatrix()
{
    int determinant = calcDeterminant();
    int a = keyMatrix[0][0];
    int b = keyMatrix[0][1];
    int c = keyMatrix[1][0];
    int d = keyMatrix[1][1];

    inverseKeyMatrix[0][0] = (d);
    inverseKeyMatrix[0][1] = (-b);
    inverseKeyMatrix[1][0] = (-c);
    inverseKeyMatrix[1][1] = (a);

    // Apply the modular inverse to each element
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            inverseKeyMatrix[i][j] = (inverseKeyMatrix[i][j] * mmi(determinant, alphaLength)) % alphaLength;
        }
    }
    /*

    // Apply the modular inverse to each element
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            inverseMatrix[i][j] = (inverseMatrix[i][j] * mmi) % 26;
        }
    }*/
}
string encrypt(string plainText)
{
    string encryptedText;
    string pairs[100];
    string enPairs[100];
    original = plainText;
    // take only characters from the plain text
    plainText = onlyChar(plainText);
    // transform all the characters into uppercase
    plainText = toUpperCase(plainText);

    // convert plaintext into digraphs
    if (plainText.length() % 2 != 0)
        plainText += 'X';

    int n = 0;
    for (int i = 0; i < plainText.length(); i += 2)
    {
        pairs[n++] = plainText.substr(i, 2);
    }

    for (int i = 0; i < n; i++)
    {

        enPairs[i] += char((((keyMatrix[0][0] * (pairs[i][0] - 65)) + (keyMatrix[0][1] * (pairs[i][1] - 65))) % alphaLength) + 65);

        enPairs[i] += char((((keyMatrix[1][0] * (pairs[i][0] - 65)) + (keyMatrix[1][1] * (pairs[i][1] - 65))) % alphaLength) + 65);
    }

    for (int i = 0; i < n; i++)
    {

        encryptedText += enPairs[i];
    }

    return encryptedText;
}
void decrypt(string enText)
{
    string pairs[100];
    string dePairs[100];
    string decryptedText;
    string encryptedText;

    generateInverseKeyMatrix();

    // convert encrypted text into digraphs

    int n = 0;
    for (int i = 0; i < enText.length(); i += 2)
    {
        pairs[n++] = enText.substr(i, 2);
    }

    for (int i = 0; i < n; i++)
    {
        int x = ((inverseKeyMatrix[0][0] * (pairs[i][0] - 65)) + (inverseKeyMatrix[0][1] * (pairs[i][1] - 65)));
        int y = ((inverseKeyMatrix[1][0] * (pairs[i][0] - 65)) + (inverseKeyMatrix[1][1] * (pairs[i][1] - 65)));

        x %= alphaLength;
        y %= alphaLength;
        x = x < 0 ? 26 + x : x;
        y = y < 0 ? 26 + y : y;

        dePairs[i] += char(x + 65);
        dePairs[i] += char(y + 65);
    }

    for (int i = 0; i < n; i++)
    {

        decryptedText += dePairs[i];
    }
    int k = 0;
    for(int i = 0; i < original.length(); i++){
        if(isChar(original[i])){
            if((original[i] >= 65 && original[i] <= 90) ){
                original[i] = toupper(decryptedText[k++]);
            }else original[i] = tolower(decryptedText[k++]);
        }
    }

}

int main()
{
    ///////////////////////////////////////
    // fill up alphabets array with letters
    /*
    for (int i = 0; i < 26; i++)
    {
        alphabets[i] = char(i + 65);
    }
     */
    ///////////////////////////////////////
    cout << "HILL CIPHER" << endl;
    string plainText;
    // Generate an invertible key matrix
    generateKeyMatrix();
    cout<< "The key matrix : "<<endl;
    displayKeyMatrix();

    cout << "Enter plain text : ";
    getline(cin, plainText);
    string encryptedText = encrypt(plainText);
    cout << "Encrypted text : " << encryptedText << endl;

    decrypt(encryptedText);
    cout << "Decrypted text : " << original << endl;
}