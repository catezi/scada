#include "Headers/define.h"
#include "Headers/callpython.h"
using namespace std;

string transform(string s) {

    Py_Initialize();
    if (!Py_IsInitialized()) {
        return "can't initialized!";
    }
    char ss[65536] = {'\0'};
    sprintf(ss, "#-*- coding: UTF-8 -*-\n"
            "import binascii\n"
            "s = \"%s\"\n"
            "ss = binascii.b2a_hex(s.decode(\"gb2312\").encode(\"utf-8\"))\n"
            "print ss\n"
            "f = open('temp.txt', 'w')\n"
            "f.write(ss)\n"
            "f.close()\n"
            , s.c_str());

    cout << ss << endl;

    string ret = "";

    ifstream in("temp.txt");

    while (!in.eof()) {
        char ch = '\0';
        in >> ch;
        ret += ch;
    }

    in.close();
    cout << ret << endl;


    PyRun_SimpleString(ss);




    Py_Finalize();
    return ret;
}
