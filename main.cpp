#include <iostream>
#include <fstream>
using namespace std;
class Crypto {
    public:
        int key;
        string plaintext;
    //优化加密逻辑，大小写单独加密，并解决出现非字母符号问题
        string Encrypt(string plaintext) {
            key=(key%26+26)%26;//限定key范围1-25且为正
            /*for (int i=0;i<plaintext.length();i++) {
                if (plaintext[i]>='A'&&plaintext[i]<='Z') {
                    if ((plaintext[i]+=key) > 'Z') plaintext[i]-=26;
                }else if (plaintext[i]>='a'&&plaintext[i]<='z') {
                    if ((plaintext[i]+=key) > 'z') plaintext[i]-=26;
                }
            }*/
            for (char &ch : plaintext) {
                if (ch >= 'A' && ch <= 'Z') {
                    ch = 'A' + ((ch - 'A' + key) % 26+26)%26;//这样写就不会出现越界的问题,对26取余保证是正数
                } else if (ch >= 'a' && ch <= 'z') {
                    ch = 'a' + ((ch - 'a' + key) % 26+26)%26;
                }
            }
            return plaintext;
        }
        string Decrypt(string plaintext) {
            key=(key%26+26)%26;//限定key范围1-25且为正
            /*for (int i=0;i<plaintext.length();i++) {
                if (plaintext[i]>='A'&&plaintext[i]<='Z') {
                    if ((plaintext[i]-=key) < 'A') plaintext[i]+=26;
                }else if (plaintext[i]>='a'&&plaintext[i]<='z') {
                    if ((plaintext[i]-=key) < 'a') plaintext[i]+=26;
                }
            }*/
            for (char &ch : plaintext) {
                if (ch>='A' && ch<='Z') {
                    ch = 'A' + ((ch - 'A' - key)%26+26)%26;
                } else if (ch>='a' && ch <='z') {
                    ch = 'a' + ((ch - 'a' - key)%26+26)%26;
                }
            }
            return plaintext;
        }
};
class FileHandler {
public:
    string file;
    string readFile() {
        //string filecontent;
        ifstream infile;
        infile.open(file);
        //infile>>filecontent;
        if (!infile) return {};
        string filecontent, line;
        while (getline(infile, line)) {//依然使用getline
            filecontent += line + '\n'; // 保留换行符
        }
        return filecontent;
    }
    void writeFile(string filecontent) {
        ofstream outfile;
        outfile.open(file);
        outfile<<filecontent;
        outfile.flush();
        outfile.close();
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