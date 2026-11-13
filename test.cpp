#include<fstream>
#include<iostream>
using namespace std;

int main(){
    ifstream file("123.txt", ios::binary);
    if(!file.is_open()){
        cout<<"File didn't opened nigga";
        return 1;
    }

    int frequency[256] = {0};
    char ch;
    while(file.get(ch)){
        unsigned char byte = static_cast<unsigned char>(ch); //convert the signed char representation to unsigned char representation named byte
        frequency[byte]++;

    }

    for (int i = 0; i < 256; i++) {
    if (frequency[i] > 0) {
        cout << static_cast<char>(i) << " : " << frequency[i] << endl;
    }
 }
    

    
}