#include "Headers/define.h"
#include "Headers/client.h"
#include "Headers/patternmatch.h"
#include "Headers/mythread.h"

using namespace std;


PTreeNode *getNewNode()
{
    int i;
    PTreeNode* tnode=(PTreeNode*)malloc(sizeof(PTreeNode));
    tnode->fail=NULL;
    tnode->par=NULL;
    tnode->patterTag=0;
    for(i = 0; i < ASCII; i ++)
        tnode->next[i]=NULL;
    return tnode;
}



int  nodeToQueue(PTree root,queue<PTree> &myqueue)
{
    int i;
    for (i = 0; i < ASCII; i++)
    {
        if (root->next[i]!=NULL)
            myqueue.push(root->next[i]);
    }
    return 0;
}


PTree buildingTree()
{
    int i,j;
    PTree root=getNewNode();
    PTree tmp1=NULL,tmp2=NULL;
    for(i = 0; i < patternnum; i ++)
    {
        tmp1=root;
        for(j = 0; j < strlen(pattern[i]); j++)
        {
            if(tmp1->next[pattern[i][j]-'\0']==NULL)
            {
                tmp2=getNewNode();
                tmp2->inputchar=pattern[i][j];
                tmp2->par=tmp1;
                tmp1->next[pattern[i][j]-'\0']=tmp2;
                tmp1=tmp2;
            }
            else
                tmp1=tmp1->next[pattern[i][j]-'\0'];
        }
        tmp1->patterTag=1;
        tmp1->patterNo=i;
    }
    return root;
}


int buildingFailPath(PTree root)
{
    int i;
    char inputchar;
    queue<PTree> myqueue;
    root->fail=root;
    for(i=0; i < ASCII; i++)
    {
        if (root->next[i]!=NULL)
        {
            nodeToQueue(root->next[i],myqueue);
            root->next[i]->fail=root;
        }
    }

    PTree tmp=NULL,par=NULL;
    while(!myqueue.empty())
    {
        tmp=myqueue.front();
        myqueue.pop();
        nodeToQueue(tmp,myqueue);

        inputchar=tmp->inputchar;
        par=tmp->par;

        while(true)
        {
            if(par->fail->next[inputchar-'\0']!=NULL)
            {
                tmp->fail=par->fail->next[inputchar-'\0'];
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


int searchAC(PTree root, char* str, int len, int successnum[])
{
    PTreeNode *tmp=root;
    int i=0;
    while(i < len)
    {
        int pos = str[i]-'\0';
        if (pos < 0 || pos >= ASCII) {
            i ++;
            continue;
        }
        //cout << i << " : " << pos << endl;
        if (tmp->next[pos]!=NULL)
        {
            tmp=tmp->next[pos];
            if(tmp->patterTag == 1) {
                //cout << "*****************************" << endl;
                cout<< i-strlen(pattern[tmp->patterNo])+1 << '\t' << tmp->patterNo << '\t' << pattern[tmp->patterNo] << endl;
                successnum[tmp->patterNo] ++;
                //cout << " 1: " << tmp->patterNo << endl;
                //cout << pattern[tmp->patterNo] << " : " << successnum[tmp->patterNo] << endl;
            }
            i++;
        }
        else
        {
            if(tmp == root)
                i++;
            else
            {
                tmp=tmp->fail;
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


int destory(PTree tree)
{
    if(tree==NULL)
        return 0;
    queue<PTree> myqueue;
    PTreeNode *tmp=NULL;

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

void startmatch(char *buff, int len, int successnum[])
{
    //char a[] = "sdmfhsgnshejfgnihaofhsrnihaoSDMFHSGNSHEJFGNIHAOFHSRNIHAO";
    PTree root=buildingTree();
    buildingFailPath(root);
    cout<< "pattern : " << pattern[0]<<" "<<pattern[1]<<" "<<pattern[2]<<" "<<pattern[3]<<" "<<endl <<endl;
    cout<< "result :" << endl << "position\t" << "NO.\t" << "pattern" <<endl;
    searchAC(root, buff, len, successnum);
    destory(root);

    for (int i = 0; i <= patternnum-1; i ++) {
        cout << pattern[i] << " : " << successnum[i] << endl;
    }
}

bool judge(int successnum[]) {
    cout << "success[0] : " << successnum[0] << endl;
    if (successnum[0] * 1 >= 1) {
        return true;
    }
    else {
        return false;
    }
}
