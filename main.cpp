#include <iostream>
#include <fstream>
using namespace std;
class Crypto {
    public:
        int key;
        string plaintext;
        string Encrypt(string plaintext) const {
            for (int i=0;i<plaintext.length();i++) {
                if (plaintext[i]>='A'&&plaintext[i]<='z') plaintext[i]+=key;
            }
            return plaintext;
        }
        string Decrypt(string plaintext) const {

            for (int i=0;i<plaintext.length();i++) {
                if (plaintext[i]>='A'&&plaintext[i]<='z') plaintext[i]-=key;
            }
            return plaintext;
        }
};
class FileHandler {
public:
    string file;
    string readFile() {
        string filecontent;
        ifstream infile;
        infile.open(file);
        infile>>filecontent;
        return filecontent;
    }
    void writeFile(string filecontent) {
        ofstream outfile;
        outfile.open(file);
        outfile<<filecontent;
    }
};
class Menu {
public:
    int menuNum;
    void menuInit() {
        cout<<"---菜单---"<<endl;
        cout<<"【1】文本加密"<<endl;
        cout<<"【2】文本解密"<<endl;
        cout<<"【3】文件加密"<<endl;
        cout<<"【4】文件解密"<<endl;
        cout<<"请输入选项：";
        cin>>menuNum;
        cin.ignore();
    }
};

int main() {
    int key;
    string originText;
    Crypto newCrypto;


    Menu Menu;
    Menu.menuInit();
    if (Menu.menuNum==1) {
        cout<<"请输入要加密的文本：";
        getline(cin,originText);
        cout<<"请输入密钥：";
        cin>>key;
        newCrypto.key=key;
        string encryptedText = newCrypto.Encrypt(originText);
        cout<<"加密结果："<<encryptedText<<endl;
        return 0;
    }

    if (Menu.menuNum==2) {
        cout<<"请输入要解密的文本：";
        getline(cin,originText);
        cout<<"请输入密钥：";
        cin>>key;
        newCrypto.key=key;
        string decryptedText = newCrypto.Decrypt(originText);
        cout<<"解密结果："<<decryptedText<<endl;
        return 0;
    }

    if (Menu.menuNum==3) {
        string fileAddr;
        cout<<"请输入要加密的文件路径：";
        getline(cin,fileAddr);
        cout<<"请输入密钥：";
        cin>>key;
        FileHandler newFile;
        newFile.file=fileAddr;
        originText = newFile.readFile();
        newCrypto.key=key;
        string encryptedText = newCrypto.Encrypt(originText);
        newFile.file="encrypted.txt";
        newFile.writeFile(encryptedText);
        cout<<"加密结果已保存至encrypted.txt"<<endl;
        return 0;
    }

    if (Menu.menuNum==4) {
        string fileAddr;
        cout<<"请输入要解密的文件路径：";
        getline(cin,fileAddr);
        cout<<"请输入密钥：";
        cin>>key;
        FileHandler newFile;
        newFile.file=fileAddr;
        originText = newFile.readFile();
        newCrypto.key=key;
        string decryptedText = newCrypto.Decrypt(originText);
        newFile.file="decrypted.txt";
        newFile.writeFile(decryptedText);
        cout<<"解密结果已保存至decrypted.txt"<<endl;
        return 0;
    }
}