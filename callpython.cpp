#include "Headers/define.h"
#include "Headers/callpython.h"
using namespace std;

string transform(string s) {

    Py_Initialize();
    if (!Py_IsInitialized()) {
        return "can't initialized!";
    }
    char temp1[s.size()+2] = {'\0'};
    char temp2[s.size()+2] = {'\0'};

    strcpy(temp1, s.c_str());
    cout << temp1 << endl;

    for (int i = 0, j = 0; i <= s.size()-1; i ++) {
        if (temp1[i] != '\n' && temp1[i] != '"' && temp1[i] != '\r' && temp1[i] != ' ') {
            if (temp1[i] >= 'A' && temp1[i] <= 'Z') {
                temp2[j ++] = temp1[i] - ('A' - 'a');
            }
            else {
                temp2[j ++] = temp1[i];
            }
        }
    }

    s = temp2;
    cout << s << endl;

    char ss[65536] = {'\0'};
    sprintf(ss, //"#-*- coding: UTF-8 -*-\n"
            "import binascii\n"
            "s = \"%s\"\n"
            "ss = binascii.b2a_hex(s.decode(\"gb2312\").encode(\"utf-8\"))\n"
            "print ss\n"
            "f = open('temp.txt', 'w')\n"
            "f.write(ss)\n"
            "f.close()\n"
            , s.c_str());

    cout << ss << endl;

    PyRun_SimpleString(ss);

    string ret = "";

    ifstream in("temp.txt");

    while (!in.eof()) {
        char ch = '\0';
        in >> ch;
        ret += ch;
    }

    in.close();
    cout << ret << endl;


    Py_Finalize();
    return ret ;
}
