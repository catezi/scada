#include "Headers/define.h"

string filepath;
int threadnum = 0;
vector<string> files;
vector<int> threadid;
vector<bool> threadfree;
vector<struct target> threadtarget;
vector<string> threadoutfile;
vector<long> threadtime;
pthread_mutex_t mutex;


int main(int argc, char* argv[])
{
    for (int i = 1; i <= argc-1; i ++) {
        string s = argv[i];
        if (s == "-n") {
            filepath = argv[++ i];
        }
        else if (s == "-t") {
            threadnum = atoi(argv[++ i]);
        }
        else {
            return 0;
        }
    }
    for (int i = 1; i <= threadnum; i ++) {
        threadid.push_back(i);
        threadfree.push_back(true);
        struct target temptar;
        threadtarget.push_back(temptar);
        threadoutfile.push_back(filepath+"/result/");
        threadtime.push_back(time((time_t*)NULL));
    }
    pthread_t id[threadnum];
    readFileList((char*)filepath.c_str(), &files);
    vector<string>::iterator it;
    for (it = files.begin(); it != files.end(); it ++) {
        ifstream in(filepath + *it);
        ofstream out(filepath + "result/" + *it);
        out.close();
        while (!in.eof()) {
            int i = 0;
            char line[256] = {'\0'};
            char host[20] = {'\0'};
            struct target tar;
            tar.port = 0;
            in.getline(line, 100);
            if (line[0] == '\0') {
                break;
            }
            for (i = 0; line[i] != '\t'; i ++) {
                host[i] = line[i];
            }
            tar.host = host;
            for (i += 2; line[i] != '\0'; i ++) {
                tar.port = tar.port*10 + (line[i] - '0');
            }
            cout << tar.host << endl;
            while (true) {
                int flag = 0;
                vector<int>::iterator threadidit;
                for (threadidit = threadid.begin(); threadidit != threadid.end(); threadidit ++) {
                    if (threadfree.at(*threadidit-1)) {
                        threadtarget.at(*threadidit-1) = tar;
                        threadfree.at(*threadidit-1) = false;
                        threadoutfile.at(*threadidit-1) = filepath + "result/" + *it;
                        int *paraid = (int*)malloc(sizeof(int));
                        *paraid = *threadidit;
                        threadtime.at(*threadidit-1) = time((time_t*)NULL);
                        int res = pthread_create(&id[*threadidit-1], NULL, mythread, (void*)paraid);
                        if (res) {
                            string s = "create the thread " + to_string(*threadidit) + " failed!";
                            cout << s << endl;
                        }
                        else {
                            flag = 1;
                            break;
                        }
                    }
                    else {
                        if (time((time_t*)NULL) - threadtime.at(*threadidit-1) >= 2) {
                            pthread_cancel(id[*threadidit-1]);
                            threadfree.at(*threadidit-1) = true;
                        }
                    }
                }
                if (flag == 1) {
                    break;
                }
                usleep(1000);
            }
        }
        in.close();

    }
    cout << "Hello world!" << endl;
    return 0;
}
