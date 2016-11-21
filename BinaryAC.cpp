#include "Headers/AC.h"
#include "Headers/transform.h"
#include "Headers/define.h"
#include "Headers/client.h"
#include "Headers/patternmatch.h"
#include "Headers/mythread.h"

using namespace std;





TreeNode *getNewNode() {
    int i;
    TreeNode* tnode=(TreeNode*)malloc(sizeof(TreeNode));
    tnode->fail=NULL;
    tnode->par=NULL;
    tnode->patterTag=0;
    for(i = 0; i < ASCII; i ++)
        tnode->next[i]=NULL;
    return tnode;
}



int  nodeToQueue(Tree root,queue<Tree> &myqueue) {
    int i;
    for (i = 0; i < ASCII; i++)
    {
        if (root->next[i]!=NULL)
            myqueue.push(root->next[i]);
    }
    return 0;
}


Tree buildingTree() {
    int i,j;
    Tree root=getNewNode();
    Tree tmp1=NULL,tmp2=NULL;
    for(i = 0; i < patternnum; i ++)
    {
        tmp1=root;
        for(j = 0; j < strlen(pattern[i]); j += 2)
        {
            int pos = hexchar2int(pattern[i][j], pattern[i][j+1]);
            if(tmp1->next[pos]==NULL)
            {
                tmp2=getNewNode();
                string s = pattern[i];
                strcpy(tmp2->inputchar, s.substr(j, j+1).c_str());
                tmp2->par=tmp1;
                tmp1->next[pos]=tmp2;
                tmp1=tmp2;
            }
            else {
                tmp1=tmp1->next[pos];
            }
        }
        tmp1->patterTag=1;
        tmp1->patterNo=i;
    }
    return root;
}



int buildingFailPath(Tree root) {
    int i;
    char inputchar[2];
    queue<Tree> myqueue;
    root->fail=root;
    for(i=0; i < ASCII; i++)
    {
        if (root->next[i]!=NULL)
        {
            nodeToQueue(root->next[i],myqueue);
            root->next[i]->fail=root;
        }
    }

    Tree tmp=NULL,par=NULL;
    while(!myqueue.empty())
    {
        tmp=myqueue.front();
        myqueue.pop();
        nodeToQueue(tmp,myqueue);
        strcpy(inputchar, tmp->inputchar);
        par=tmp->par;

        while(true)
        {
            int pos = hexchar2int(inputchar[0], inputchar[1]);
            if(par->fail->next[pos]!=NULL)
            {
                tmp->fail=par->fail->next[pos];
                break;
            }
            else
            {
                if(par->fail==root)
                {
                    tmp->fail=root;
                    break;
                }
                else
                    par=par->fail->par;
            }
        }
    }
    return 0;
}


bool ifleafnode(Tree tmp) {
    for (int i = 0; i < ASCII; i ++) {
        if (tmp->next[i]  != NULL) {
            return false;
        }
    }
    return true;
}

void followfailure(Tree tmp, int i, Tree root) {
    int k = 0;
    while(tmp->fail != root) {
        tmp = tmp->fail;
        if (ifleafnode(tmp) && k == 0) {
            return;
        }
        if (tmp->patterTag == 1) {
            cout<<i-strlen(pattern[tmp->patterNo])+1<<'\t'<<tmp->patterNo<<'\t'<<pattern[tmp->patterNo]<<endl;
        }
        k ++;
    }
}



int searchAC(Tree root, char* str, int len, int successnum[]) {
    TreeNode *tmp=root;
    int i=0;
    while(i < len)
    {
        int pos = hexchar2int(str[i], str[i+1]);
        if (pos < 0 || pos >= ASCII) {
            i += 2;
            continue;
        }
        //cout << i << " : " << pos << endl;
        if (tmp->next[pos]!=NULL)
        {
            tmp=tmp->next[pos];
            Tree temp = tmp;
            followfailure(tmp, i, root);
            tmp = temp;
            if(tmp->patterTag == 1) {
                //cout << "*****************************" << endl;
                cout<< i-strlen(pattern[tmp->patterNo])+1 << '\t' << tmp->patterNo << '\t' << pattern[tmp->patterNo] << endl;
                successnum[tmp->patterNo] ++;
                //cout << " 1: " << tmp->patterNo << endl;
                //cout << pattern[tmp->patterNo] << " : " << successnum[tmp->patterNo] << endl;
            }
            i += 2;
        }
        else
        {
            if(tmp == root) {
                i += 2;
            }
            else
            {
                tmp = tmp->fail;
                if(tmp -> patterTag==1) {
                    cout<< i-strlen(pattern[tmp->patterNo])+1 << '\t' << tmp->patterNo << '\t' << pattern[tmp->patterNo] << endl;
                    successnum[tmp->patterNo] ++;
                    //cout << " 2: " << tmp->patterNo << endl;
                    //cout << pattern[tmp->patterNo] << " : " << successnum[tmp->patterNo] << endl;
                }
            }
        }
    }
    while(tmp!=root)
    {
        tmp = tmp->fail;
        if(tmp->patterTag==1) {
            cout<<i-strlen(pattern[tmp->patterNo])+1<<'\t'<<tmp->patterNo<<'\t'<<pattern[tmp->patterNo]<<endl;
            successnum[tmp->patterNo] ++;
            //cout << " 3: " <<tmp->patterNo << endl;
            //cout << pattern[tmp->patterNo] << " : " << successnum[tmp->patterNo] << endl;
        }
    }
    return 0;
}


int destory(Tree tree) {
    if(tree==NULL)
        return 0;
    queue<Tree> myqueue;
    TreeNode *tmp=NULL;

    myqueue.push(tree);
    tree=NULL;
    while(!myqueue.empty())
    {
        tmp=myqueue.front();
        myqueue.pop();

        for (int i = 0; i < ASCII; i++)
        {
            if(tmp->next[i]!=NULL)
                myqueue.push(tmp->next[i]);
        }
        free(tmp);
    }
    return 0;
}

void startmatch(char *buff, int len, int successnum[]) {
    //char a[] = "sdmfhsgnshejfgnihaofhsrnihaoSDMFHSGNSHEJFGNIHAOFHSRNIHAO";
    Tree root=buildingTree();
    buildingFailPath(root);
    cout<< "pattern : " << pattern[0] << " "<<pattern[1]<<" "<<pattern[2]<<" "<<pattern[3]<<" "<<endl <<endl;
    cout<< "result :" << endl << "position\t" << "NO.\t" << "pattern" <<endl;
    searchAC(root, buff, len, successnum);
    destory(root);

    for (int i = 0; i <= patternnum-1; i ++) {
        cout << pattern[i] << " : " << successnum[i] << endl;
    }
}

bool judge(int successnum[]) {
    cout << "success[0] : " << successnum[0] << endl;
    if (successnum[0] * 1 >= 1 || successnum[3] * 1 >= 1) {
        return true;
    }
    else {
        return false;
    }
}
