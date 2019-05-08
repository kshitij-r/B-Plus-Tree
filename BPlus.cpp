//Name - Kshitij Raj
//UFID - 13584965

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <vector>
#include <tuple>
#include <string>
#include <cstring>
#include <ostream>
#include <regex>
#include <sstream>
#include <cstdlib>
using namespace std;

int order;

class Node
{
public:         //Declaration of all relevant Node Structures done below
    string greeting;
    int nNodes; //     Number of Nodes in the Tree
    Node *parentNode;
    Node *next;
    Node *prev;
    vector<tuple<double, string>> record;
    vector<double> keys;
    vector<Node *> children;
    bool isleaf;

    Node(bool leaf = true)
    {
        nNodes = 0;
        parentNode = NULL;
        next = NULL;
        prev = NULL;
        greeting = "alive";
        isleaf = leaf;
    }
};

Node *rootNode = new Node(); //Declaring the Root Node

//Sorting the node's children
bool childSort(const Node *a, const Node *b)
{
    return get<0>(a->record.at(0)) < get<0>(b->record.at(0));
}

//Sorting the Keys of concerned nodes
bool keySort(const Node *a, const Node *b)
{ 
    return a->keys.at(0) < b->keys.at(0);
}

void splitNode(Node *curNode) //Function to split nodes while operations
{
    if (curNode->isleaf) //If current node is a leaf node, Direct declaration
    {
        Node *rNode = new Node(); //Making a new node
        rNode->prev = curNode;   //Adjusting relevant node pointers
        
        if (curNode->next == NULL)
        {
            curNode->next = rNode;
        }
        
        else
        {
            rNode->next = curNode->next;
            rNode->next->prev = rNode;
            curNode->next = rNode;
        }

        int x = order / 2;  //order is divided equally
        for (int i = curNode->record.size() / 2; i < curNode->record.size(); i++)
            rNode->record.push_back(curNode->record.at(i));
        rNode->nNodes = rNode->record.size();
        int z = curNode->record.size() - curNode->record.size() / 2;
        curNode->record.erase(curNode->record.begin() + curNode->record.size() / 2, curNode->record.end());
        curNode->nNodes = curNode->record.size();
        
        if (curNode->parentNode == NULL) //Check if the Parent node of the current node is NULL
        {
            Node *pNode = new Node(false);
            pNode->children.push_back(curNode);
            pNode->children.push_back(rNode);
            curNode->parentNode = pNode;
            rNode->parentNode = pNode;
            pNode->keys.push_back(get<0>(rNode->record.at(0)));
            curNode->parentNode->nNodes++;
            rootNode = pNode;
        }

        else 
             //If the Parent node of the current node is not null
             //Split the nodes and then merge the secong half of the split node
             //with the parent node of current node. 
             //On merging, if the capacity exceeds the order, Split and merge upwards recursively.
            {
            rNode->parentNode = curNode->parentNode;

            curNode->parentNode->keys.push_back(get<0>(rNode->record.at(0)));

            sort(curNode->parentNode->keys.begin(), curNode->parentNode->keys.end());

            curNode->parentNode->children.push_back(rNode);

            sort(curNode->parentNode->children.begin(), curNode->parentNode->children.end(), childSort);
            curNode->parentNode->nNodes++;

            if (curNode->parentNode->keys.size() > order - 1)
            {
                curNode->parentNode->nNodes--;
                splitNode(curNode->parentNode);
            }
        }
    }
    
    else   //Dealing with internal nodes
    { 
        Node *rNode = new Node(false); //Internal Node
        int x = order / 2;    //Halving the order
        for (int i = curNode->keys.size() / 2; i < curNode->keys.size(); i++)
            rNode->keys.push_back(curNode->keys.at(i));
        curNode->keys.erase(curNode->keys.begin() + curNode->keys.size() / 2, curNode->keys.end());
        rNode->nNodes = rNode->record.size();
        curNode->nNodes = curNode->record.size();

        // The respective children need to be distributed equally
        if (curNode->children.size() % 2)
        {
            for (int i = curNode->children.size() / 2 + 1; i < curNode->children.size(); i++)
            {
                rNode->children.push_back(curNode->children.at(i));
                curNode->children.at(i)->parentNode = rNode;
            }
            curNode->children.erase(curNode->children.begin() + curNode->children.size() / 2 + 1, curNode->children.end());
        }
       
        else
        {
            for (int i = curNode->children.size() / 2; i < curNode->children.size(); i++)
            {
                rNode->children.push_back(curNode->children.at(i));
                curNode->children.at(i)->parentNode = rNode;
            }
            curNode->children.erase(curNode->children.begin() + curNode->children.size() / 2, curNode->children.end());
        }
        

        if (curNode->parentNode == NULL)
        {
            Node *pNode = new Node(false);
            pNode->children.push_back(curNode);
            pNode->children.push_back(rNode);
            curNode->parentNode = pNode;
            rNode->parentNode = pNode;
            pNode->keys.push_back(rNode->keys.at(0));
            rNode->keys.erase(rNode->keys.begin());
            rNode->nNodes--;
            curNode->parentNode->nNodes++;
            rootNode = pNode;
        }
        else
        {
            rNode->parentNode = curNode->parentNode;
            curNode->parentNode->keys.push_back(rNode->keys.at(0));
            rNode->keys.erase(rNode->keys.begin());
            sort(curNode->parentNode->keys.begin(), curNode->parentNode->keys.end());
            curNode->parentNode->children.push_back(rNode);
            curNode->parentNode->nNodes++;

            sort(curNode->parentNode->children.begin(), curNode->parentNode->children.end(), keySort);

            if (curNode->parentNode->keys.size() > order - 1)
            {
                curNode->parentNode->nNodes--;
                splitNode(curNode->parentNode);
            }
        }
    }
}

//Function to insert nodes into the tree 

void insertNode(Node *curNode, double k, string val)
{   
    //Check is current node is a leaf node or not. If it is a leaf
    //node, do not traverse down the tree and insert right here.
    
    if (curNode->isleaf)
    {
        curNode->record.push_back(make_tuple(k, val));
        sort(curNode->record.begin(), curNode->record.end());

        curNode->nNodes++;

        //Split the Node(may or may not be root) if its current size
        //is one less than the order.
       
        if (curNode->record.size() > order - 1)
        {
            curNode->nNodes--;
            splitNode(curNode);  //Function to split Nodes
        }

    }
    //If the current node is not a leaf or Root node, traverse to the
    //appropriate insertion point of the new node based on key,
    //starting from the root node.
    
    else
    {
        int flag = 0;
        for (int i = 0; i < curNode->keys.size(); i++)
        {   
            //Check whether new key is greater or less than key of current node
           
            if (k < curNode->keys.at(i)) 
            {
                flag = 1;
                insertNode(curNode->children.at(i), k, val);
                break;
            }
        }
       
        if (flag == 0)
        {
            insertNode(curNode->children.at(curNode->children.size() - 1), k, val);
        }
    }
}

void go(Node *curNode)
{   
    //If current node is not a leaf node, go to its child at 0.
    //It will direct you to the lower level list and we check again.
    
    if (!curNode->isleaf)
    {
        go(curNode->children.at(0));
    }
    else
    {
        while (curNode != NULL)
        {
            for (int i = 0; i < curNode->record.size(); i++)
                cout << get<0>(curNode->record.at(i)) << endl;
            curNode = curNode->next;
        }
    }
}

//Function to Search in a B+ Tree

void search(Node *curNode, double k, ofstream &o_file)
{
    //First check if the current node is a leaf node or not.
    //If not a leaf node, traverse to the node with the appropriate key
   
    if (!curNode->isleaf)
    {
        int flag = 0;
        for (int i = 0; i < curNode->keys.size(); i++)
        {
            if (k < curNode->keys.at(i))
            {
                flag = 1;
                //Search among the children for the relevant value 
                //corresponding to the given key
                search(curNode->children.at(i), k, o_file);
                break;
            }
        }
       
        if (flag == 0)
        {
            search(curNode->children.at(curNode->children.size() - 1), k, o_file);
        }
    }
    
    else
    {
        int found = 0;
        for (int i = 0; i < curNode->record.size(); i++)
        {
            if (k == get<0>(curNode->record.at(i)))
            {
                o_file << get<1>(curNode->record.at(i))<<endl;

                if (i + 1 < curNode->record.size() && k != get<0>(curNode->record.at(i + 1)))
                {   
                    //If a single value is present in the key
                    o_file << endl;
                }
                else if (i + 1 < curNode->record.size() && k == get<0>(curNode->record.at(i + 1)))
                {
                    // If the block contains multiple values with the same key
                    while (i + 1 < curNode->record.size() && k == get<0>(curNode->record.at(i + 1)))
                    {   
                        //Write to the output file
                        o_file << "," << get<1>(curNode->record.at(++i));
                    }
                    o_file << endl;
                }

                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            o_file << "Null" << endl;
        }
    }
}
//Function to Search in a range in a B+ Tree
void search(Node *curNode, double k, double l, ofstream &o_file)
{
    if (!curNode->isleaf)
    {
        int flag = 0;
        for (int i = 0; i < curNode->keys.size(); i++)
        {
            if (k < curNode->keys.at(i))
            {
                flag = 1;
                search(curNode->children.at(i), k, l, o_file);
                break;
            }
        }
        if (flag == 0)
        {
            search(curNode->children.at(curNode->children.size() - 1), k, l, o_file);
        }
    }
    else
    {
        int x = 0;

        for (int i = 0; i < curNode->record.size(); i++)
        {
            if (k <= get<0>(curNode->record.at(i)))
            {
                x = i;
                break;
            }
        }

        int found = 0;
        int flag = 0;

        while (flag == 0 && curNode != NULL)
        {
            for (int i = x; i < curNode->record.size(); i++)

            //Searching within a Range in the B+ Tree 
            {
                if (get<0>(curNode->record.at(i)) >= k && get<0>(curNode->record.at(i)) <= l)
                {
                    if (found == 0)
                    //Writing the value of the first node in the search key for search in range operation
                        o_file << get<1>(curNode->record.at(i))<<",";
                    else
                        //Writing the value of the second node in the search key for search in range operation
                        o_file << get<1>(curNode->record.at(i));
                    found = 1;
                }
                else
                {
                    flag = 1;
                    break;
                }
            }

            curNode = curNode->next;
            x = 0;
        }

        if (found == 0)
        {
            o_file << "Null" << endl;
        }
        else
            o_file << endl;
    }
}

int main(int argc, char *argv[])
{
    //int z;
    //Take the input file as argument from the command line
    ifstream file(argv[1]);

    //If file could not be opened, return with error.
    if (file.fail())
    {
        exit(1);
    }

    //Create an output file to write the results to.
    ofstream o_file("output_file.txt");

    string line;
    string string1;
    int one = 1;
    while (getline(file, line)) //Parse the input file
    {

        int i = 0;
        int x = 0;
        int y = 0;
        double a = 0;
        double b = 0;
        string temp = "";
        string temp2 = "";
        string s1;

        //To get the value of the order of the tree, scan the first line
        //and read the required int value of the order
        if (one == 1)
        {
            //cout << "yaha hai line " << line << endl;
            s1 = line.substr(11, 1);
            order = stoi(s1);     //Initializing the value to Order of the tree
            //cout << "yaha bhi hai line" << endl;
            one = 2;
        }
        
        //If the first 6 characters of the input line are 'INSERT', extract the data
        //and do required processing to make it ready for storing in the node.
        if (line.substr(0, 6) == "Insert")
        {
            if (line.substr(7, 1) == "-") //Dealing with data with negative key value
            {
                for (i = 8; line.substr(i, 1) != ","; i++)
                {
                    x++;
                }
                temp = line.substr(8, x);
                a = -stod(temp);
            }
            else
            //Dealing with data with positive key value
            {
                for (i = 7; line.substr(i, 1) != ","; i++)
                {
                    x++;
                }
                temp = line.substr(7, x);
                a = stod(temp);
            }

            y = i + 1;
            x = 0;

            for (i = y; line.substr(i, 1) != ")"; i++)
            {
                x++;
            }

            temp2 = line.substr(y, x);
            insertNode(rootNode, a, temp2);
        }

        //If the first character of the input line is 'S',
        //and do required processing to make it ready for searching in the tree.
        if (line.substr(0, 1) == "S")
        {
            if (line.substr(7, 1) == "-") //Dealing with data with negative key value
            {
                for (i = 8; line.substr(i, 1) != "," && line.substr(i, 1) != ")"; i++)
                {
                    x++;
                }
                temp = line.substr(8, x);
                a = -stod(temp);
            }
            else //Dealing with data with positive key value
            {
                for (i = 7; line.substr(i, 1) != "," && line.substr(i, 1) != ")"; i++)
                {
                    x++;
                }
                temp = line.substr(7, x);
                a = stod(temp);
            }
            //These lines of code in this section are just to extract the data from the input file
            //in proper format as given in the Project Question.
            //All these lines have been written according to the characters in the sample input in the 
            //Project Question.

            if (line.substr(i, 1) == ",")
            {
                y = i + 1;
                x = 0;
                if (line.substr(y, 1) == "-")
                {
                    y = y + 1;
                    for (i = y; line.substr(i, 1) != ")"; i++)
                    {
                        x++;
                    }
                    temp2 = line.substr(y, x);
                    b = -stod(temp2);
                }
                else
                {
                    for (i = y; line.substr(i, 1) != ")"; i++)
                    {
                        x++;
                    }
                    temp2 = line.substr(y, x);
                    b = stod(temp2);
                }
                search(rootNode, a, b, o_file);
            }
            else
            {
                search(rootNode, a, o_file);
            }
        }
    }
    //Close all open files
    file.close();
    o_file.close();

    return 0;
}
