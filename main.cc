// Samir Musali [#20491904]
// C&O 487, Assignment 1, Problem 1:
// Breaking ciphertext.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//The average frequency of English letters in the average English text with 10002 letters:
int nfreq[26] = {8167, 1492, 2782, 4253, 12702, 2228, 2015, 6094, 6966, 153, 772, 4025, 2406, 6749, 7507, 1929, 95, 5987, 6327, 9056, 2758, 978, 2360, 150, 1974, 74};
int nsum = 99999 /*sum of frequencies above*/, nalp = 26 /*the number of English letters*/;

//Ciphertext without whitespace:
string adjusting(string cipher){
    string result = "";
    for (int i = 0; i < cipher.size(); i++){
        char tra = cipher[i];
        if (tra != ' ') result = result + tra;
    }
    return result;
}

//Dividing into blocks:
vector<string> divideinto(string cipher, int blocksize){
    int l = cipher.length();
    int q = l/blocksize;
    int r = l%blocksize;
    vector<string> result;
    string buf = "";
    string adbuf = cipher.substr(l-r,r);
    for (int i = 0; i < q; i++){
        int j = blocksize * i;
        int k = j + blocksize;
        string block = "";
        for (int t = j; t < k; t++){block = block + cipher[t];}
        result.push_back(block);
    }
    if (r != 0) {
        adbuf.append(buf);
        result.push_back(adbuf);
    }
    return result;
}

//Little helpers:
int gcd(int a, int b){
    if (a == 1 || b == 1) return 1;
    if (a == 0) return b;
    if (b == 0) return a;
    int c = a%b;
    return gcd(b,c);
}
int max_vec(vector<int> v){
    int result = v[0];
    for (int i = 0; i < v.size(); i++) if (v[i] >= result) result = v[i];
    return result;
}

//Right Shifting (Inefficient :( ):
string rightshift(string inp, int times){
    int len = inp.size(), required = times%len;
    string first = inp.substr(0,len-required), second = inp.substr(len-required,required), result = second + first;
    return result;
}

//Comparing two strings, and finding the number of the same letters in the same position:
int ncomp(string first, string second){
    int count = 0;
    int lf = first.length();
    int ls = second.length();
    int min = 0;
    if (ls < lf) {min = ls;}
    else {min=lf;}
    for (int i = 0; i < min; i++) if (first[i] == second[i]) count++;
    return count;
}

//Vector of containing ncomp values after 1,...,times shifting:
vector<int> guessarr(string cipher, int times){
    string ciphert = adjusting(cipher);
    vector<int> result;
    for (int i = 1; i <= times; i++){
        string buf = rightshift(ciphert,i);
        int buff = ncomp(ciphert,buf);
        result.push_back(buff);
    }
    return result;
}

//Keylength:
int keyl(string cipher, int times){
    vector<int> guess = guessarr(cipher,times);
    int result = 0;
    int max = max_vec(guess), bound = (max * 9)/10;
    vector<int> buff;
    for (int i = 0; i < guess.size(); i++) if (guess[i] >= bound) buff.push_back(i + 1);
    result = buff[0];
    for (int i = 1; i < buff.size(); i++) result = gcd(result,buff[i]);
    return result;
}

//Subtracting chars:
char bigcsub(char in1, char in2){
    if (in1 >= 48 && in1 <= 57){return in1;}
    if (in2 >= 48 && in2 <= 57){return in2;}
    int inp1 = 0, inp2 = 0;
    for (int i = 0; i < alphabet.size(); i++){
        if (alphabet[i] == in1) inp1 = i;
        if (alphabet[i] == in2) inp2 = i;
    }
    int res = (inp1 - inp2);
    if (res < 0) res += alphabet.size();
    return alphabet[res];
}

//Subtracting strings:
string bigssub(string in1, string in2){
    int l1 = in1.size(), l2 = in2.size(), min = 0, max = 0;
    if (l1 < l2){min = l1; max = l2;}
    else {min = l2; max = l1;}
    string result = "";
    for (int i = 0; i < min; i++) result = result + bigcsub(in1[i],in2[i]);
    return result;
}

//String consisting of the 'colth' letter of the strings in sp:
string columnth(vector<string> sp, int col){
    string out = "";
    for (int i = 0; i < sp.size(); i++) {
        string buf = sp[i];
        out = out + buf[col];
    }
    return out;
}

//Applying columnth to cipher in the given blocksize:
vector<string> coll(string cipher, int blocksize){
    vector<string> result;
    vector<string> buff = divideinto(cipher,blocksize);
    for (int i = 0; i < blocksize; i++){
        string res = columnth(buff,i);
        result.push_back(res);
    }
    return result;
}

//Little helper:
int count(char c, string s){
    int result = 0;
    for (int i = 0; i < s.size(); i++) if (s[i] == c) result++;
    return result;
}

//Frequency vector of letters in the position k of the strings in collvec:
vector<int> colkfreq(vector<string> collvec, int k){
    string buff = collvec[k];
    vector<int> result;
    for (int i = 0; i < alphabet.size(); i++) result.push_back(count(alphabet[i],buff));
    return result;
}

//Finding the residual between given and the nfreq:
int findresidual(vector<int> given){
    int min = 0, l = given.size();
    if (nalp < l) {min = nalp;}
    else {min = l;}
    int counted = 0;
    for (int  i = 0; i < l; i++) counted += given[i];
    vector<int> norm;
    for (int i = 0; i < nalp; i++) norm.push_back((given[i] * nsum)/counted);
    int result = 0;
    for (int i = 0; i < min; i++) result += (nfreq[i] - norm[i]) * (nfreq[i] - norm[i]);
    return result;
}

//Vector shifting:
vector<int> vshift(vector<int> given, char dir, int num){
    int l = given.size();
    int rounded = num%l;
    vector<int> result;
    if (dir == 'l') {
        for(int i = rounded; i < l; i++) result.push_back(given[i]);
        if (rounded != 0) {
            for (int i = 0; i < rounded; i++) result.push_back(given[i]);
        }
    }
    if (dir == 'r') {
        for (int i = l - rounded; i < l; i++) result.push_back(given[i]);
        if (rounded != l) {
            for (int i = 0; i < l - rounded; i++) result.push_back(given[i]);
        }
    }
    return result;
}

//Vector of residuals:
vector<int> vresidual(vector<int> given){
    int min = 0, l = given.size();
    vector<int> result;
    if (l < nalp) {min = l;}
    else {min = nalp;}
    for (int i = 0; i < min; i++){
        vector<int> buf = vshift(given,'l',i);
        int res = findresidual(buf);
        result.push_back(res);
    }
    return result;
}

//Finding the key letter:
char findletter(vector<int> given){
    int min = 0, l = given.size();
    char result = 'A'; int res = 0;
    int minvalue = given[0];
    if (l < nalp) {min = l;}
    else {min = nalp;}
    for (int i = 0; i < min; i++) if (given[i] < minvalue) minvalue = given[i];
    for (int i = 0; i < min; i++) if (given[i] == minvalue) {res = i; break;}
    result = alphabet[res];
    return result;
}

//Key:
string findkey(string ciphert, int blocksize){
    string result = "", cipher = adjusting(ciphert);
    vector<string> cll = coll(cipher,blocksize);
    for (int i = 0; i < blocksize; i++){
        vector<int> cfre = colkfreq(cll,i);
        vector<int> vres = vresidual(cfre);
        char bbf = findletter(vres);
        result = result + bbf;
    }
    return result;
}

//Decryption:
string decrypt(string ciphert, string key){
    string cipher = adjusting(ciphert);
    string plain = "";
    int keylength = key.size();
    vector<string> divcip = divideinto(cipher,keylength);
    for (int i = 0; i < divcip.size(); i++){
        string buff = bigssub(divcip[i],key);
        plain = plain + buff;
    }
    return plain;
}

//Main job:
int main(void){
    cout << "Welcome!" << endl;
    string shifting = "Yes";
    cout << "Enter the ciphertext: " << endl; string ciphertext = "";
    getline(cin,ciphertext); cout << endl; int recom = (ciphertext.size()*55)/100;
    while (shifting == "Yes"){
        cout << "Shifting number below is recommended to be <=" << recom << "." << endl;
        cout << "Enter the number how many times you want to shift: "; int n; cin >> n; cout << endl;
        vector<int> shiftresult = guessarr(ciphertext,n);
        int maxsr = max_vec(shiftresult);
        cout << "Here are the highest frequencies of repetition:" << endl;
        for (int i = 0; i < shiftresult.size(); i++) if (shiftresult[i] >= (maxsr * 9)/10) cout << shiftresult[i] << " <= " << i + 1 << endl; cout << endl;
        int keyln = keyl(ciphertext,n);
        cout << "According to the indices of highest frequencies above, the possible keylength: " << keyln << endl; cout << endl;
        cout << "Do you want to change the number? Type 'Yes' if you want: "; cin >> shifting; cout << endl;
    }
    cout << "Guess the keylength: "; int bs; cin >> bs; cout << endl;
    cout << "Your key is: "; string key = findkey(ciphertext,bs); cout << key << endl; cout << endl;
    cout << "And here is the plaintext:" << endl; string plain = decrypt(ciphertext,key); cout << plain << endl;
    cout << endl; cout << "Have a nice day!" << endl; cout << "Samir Musali [#20491904]" << endl;
}
