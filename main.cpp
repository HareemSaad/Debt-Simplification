#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int Top=-1;

void push(int* Stack, int value);
void PrintStack(int* Stack);
int pop();
void ClearStack(int* Stack);

int DefineSize()
{
    int Size1;
    cout << "Enter the number of participants you want to add: " ;
    cin >> Size1;
    return Size1;
}

const int Size = DefineSize();

struct Vertex
{
    string Name;
    //int Index = 0;
    int Owes = 0;
    int Asset = 0;
    int NetChange = 0;
    int Status = -1;
};

struct Edge
{
    int Weight = 0;
};

void ResetStatus (Vertex* Vertices);

int SearchVertex(string Value, Vertex* Vertices)
{
    int Index,flag=0;
    for(int i=0; i<Size; i++)
    {
        if (Value == Vertices[i].Name)
        {
            Index = i;
            flag =1;
            return Index;
        }
    }
    if(flag==0)
    {
        cout << "Value Doesn't Exist" <<endl;
        return -1;
    }
}

void AddEdge(int Vertex1Index, int Vertex2Index, int weight, Edge *Graph, Vertex* Vertices)
{
    if (Vertex1Index==-1 || Vertex2Index==-1)
    {
        return ;
    }
    (*((Graph + Vertex1Index*Size) + Vertex2Index)).Weight += weight;
    // Vertex1Index is Row
    //Vertex2Index is column
    Vertices[Vertex1Index].Asset = Vertices[Vertex1Index].Asset + weight;
    Vertices[Vertex2Index].Owes = Vertices[Vertex2Index].Owes +  weight;
    Vertices[Vertex1Index].NetChange =  Vertices[Vertex1Index].Owes - Vertices[Vertex1Index].Asset;
    Vertices[Vertex2Index].NetChange =  Vertices[Vertex2Index].Owes - Vertices[Vertex2Index].Asset;
}

int OutDegree (int Vertex1Index, Edge *Graph)
{
    if (Vertex1Index==-1)
    {
        cout << endl;
        return 0;
    }
    int Counter = 0;
    for(int i=0; i<Size; i++)
    {
        if ( (*((Graph + Vertex1Index*Size) + i)).Weight != 0)
        {
            Counter = Counter + 1;
        }
    }
    return Counter;
}

void FindPath (int Vertex1Index, int Vertex2Index, Vertex* Vertices, Edge *Graph, int x, int* Stack)
{
    //cout << "\nV1: " << Vertex1Index << "\t" << "V2: " << Vertex2Index << endl;
    if (Vertex1Index==-1 || Vertex2Index==-1)
    {
        return ;
    }
    push(Stack, Vertex1Index);
    Vertices[Vertex1Index].Status = 1;
    if (Vertex1Index == Vertex2Index)
    {
        return;
    }
    else
    {
        for(int i=0; i<Size; i++)
        {
            if ((*((Graph + Vertex1Index*Size) + i)).Weight==0)
            {
                continue;
            }

            if (Vertices[i].Status == 1)
            {
                continue;
            }

            if (Vertex1Index==x && i==Vertex2Index )
            {
                continue;
            }

            FindPath(i, Vertex2Index, Vertices, Graph, x, Stack);

            if (Stack[Top]==Vertex2Index)
            {
                break;
            }
        }
        if (Stack[Top]==Vertex1Index)
        {
            pop();
        }
        return;
    }
}

void SimplifyDebt (Vertex* Vertices, Edge* Graph, int* Stack)
{
    int i=0;
    bool Flag = 1;
    while (i<Size) //i is row
    {
        Flag = 1;

        if (OutDegree(i, Graph) == 0) //if vertex is not connected we move to next vertex of the array
        {
            i = i + 1;
            continue;
        }
        else
        {
            for (int x = 0; x<Size; x++) //x is coloumn
            {
                // Graph [i][x] or the original edge
                if ((*((Graph + i*Size) + x)).Weight != 0) //to skip over edges that are 0
                {
                    Flag = 1;
                    while (Flag!=0)//repeat until more paths exists
                    {
                        ResetStatus(Vertices);
                        ClearStack(Stack);
                        FindPath(i, x, Vertices, Graph, i, Stack); //find path

                        if (Top == -1) //if path is empty skip it
                        {
                                Flag = 0;
                                continue;
                        }

                        // Find minimum Weight in path
                        int MinimumWeight = 0, Index;

                        for (int z =1; z<=Top; z++)//start from index 1 of stack as index 0 will be equal to i
                        {
                            if (z==1) //for the first iteration
                            {
                                Index = i; //make index equal to i
                                MinimumWeight = (*((Graph + Index*Size) + Stack[z])).Weight; //make minimumweight equal to the first weight in path
                            }
                            else //for every other iteration
                            {
                                if ((*((Graph + Index*Size) + Stack[z])).Weight < MinimumWeight) //if the next weight is < minimumweight
                                    MinimumWeight = (*((Graph + Index*Size) + Stack[z])).Weight; //update minimumweight
                            }
                            Index = Stack[z]; //make index equal to stack[z] so the starting point of the next iteration is the endpoint of the current iteration
                        }

                        //subtract min weight from all weights in the path. the edge with the min weight will become zero hence remove its self

                        for (int z =1; z<=Top; z++)
                        {
                            if (z==1)
                            {
                                Index = i;
                            }

                            (*((Graph + Index*Size) + Stack[z])).Weight = (*((Graph + Index*Size) + Stack[z])).Weight - MinimumWeight;

                            Index = Stack[z];
                        }

                        //add min weight to original edge [i][x]
                        (*((Graph + i*Size) + x)).Weight = (*((Graph + i*Size) + x)).Weight + MinimumWeight;
                    }
                }
            }
        }
        i = i + 1;
    }
}

int main()
{
    Vertex Vertices [Size];

    int Stack[Size];

    bool Option = 1;
    cout << "Enter the participants: " << endl;
    for (int i = 0; i < Size; i++)
    {
        cout << "Enter participant # " << i+1 << ": ";
        cin >> Vertices[i].Name;
    }

    Edge Graph [Size][Size];

    string Value, Value1;
    int weight, Count = 0;
    bool Flag = 1;

    while (Flag == 1)
    {
        cout << "\nWho lent the money?: ";
        cin >> Value;
        cout << "To whom?: ";
        cin >> Value1;
        cout << "How much money was lent? ";
        cin >> weight;
        AddEdge(SearchVertex(Value, Vertices),SearchVertex(Value1, Vertices),weight, *Graph, Vertices);

        cout << "\nDo more transactions exist?\nIf yes, press 1\nIf no, press 0\nOpt: ";
        cin >> Flag ;

    }
    cout << endl << "Your transactions are... " << endl;
    for (int i = 0; i<Size; i++)
    {
        for (int j = 0; j<Size ; j++)
        {
            if (Graph[i][j].Weight != 0)
            {
                cout << Vertices[i].Name << " lent " << Vertices[j].Name << " $" << Graph[i][j].Weight << endl;
                Count += 1;
            }
        }
    }
    cout << "You have a total of " << Count << " transactions." << endl;
    cout << "\nReducing number of transactions.... " << endl;

    cout << endl;
    for (int i = 0; i<Size ; i++)
    {
        cout << i << "\t";
        for (int j = 0; j<Size ; j++)
        {
            cout << Graph[i][j].Weight << "\t";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl << "Asset\t\t";
    for (int j = 0; j<Size ; j++)
    {
        cout << Vertices[j].Asset << "\t";
    }
    cout << endl << "Owes\t\t";
    for (int j = 0; j<Size ; j++)
    {
        cout << Vertices[j].Owes << "\t";
    }
    cout << endl << "Net Change\t" ;
    for (int j = 0; j<Size ; j++)
    {
        cout << Vertices[j].NetChange << "\t";
    }
    cout << endl;

    SimplifyDebt(Vertices, *Graph, Stack);

    Count = 0;
    cout << "\nAfter simplification" << endl;
    for (int i = 0; i<Size; i++)
    {
        for (int j = 0; j<Size ; j++)
        {
            if (Graph[i][j].Weight != 0)
            {
                cout << Vertices[j].Name << " owes " << Vertices[i].Name << " $" << Graph[i][j].Weight << endl;
                Count += 1;
            }
        }
    }
    cout  << Count << " transactions are made." << endl;

    cout << endl;
    for (int i = 0; i<Size ; i++)
    {
        cout << i << "\t";
        for (int j = 0; j<Size ; j++)
        {
            cout << Graph[i][j].Weight << "\t";
        }
        cout << endl;
    }

//    cout << endl;
//    for (int i = 0; i<Size ; i++)
//    {
//        cout << i << "\t";
//        for (int j = 0; j<Size ; j++)
//        {
//            cout << Graph[i][j].Weight << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//    cout << endl << "Net Change\t";
//    for (int j = 0; j<Size ; j++)
//    {
//        cout << Vertices[j].NetChange << "\t";
//    }


}

////Example 2
//int main()
//{
//    int Count = 0;
//
//    int Stack[Size];
//
//    Vertex Vertices [Size];
//
//    Vertices[0].Name = "Amy";
//    Vertices[1].Name = "Ben";
//    Vertices[2].Name = "Celia";
//    Vertices[3].Name = "Darcy";
//    Vertices[4].Name = "Ellis";
//    Vertices[5].Name = "Flynn";
//
//    Edge Graph [Size][Size];
//
//    //cout << "\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << 5 << endl;
//    AddEdge(0,1,10, *Graph, Vertices);
//    AddEdge(0,2,20, *Graph, Vertices);
//    AddEdge(0,3,10, *Graph, Vertices);
//    AddEdge(0,5,50, *Graph, Vertices);
//    AddEdge(1,2,20, *Graph, Vertices);
//    AddEdge(1,4,10, *Graph, Vertices);
//    AddEdge(2,4,60, *Graph, Vertices);
//    AddEdge(2,5,30, *Graph, Vertices);
//    AddEdge(3,5,40, *Graph, Vertices);
//    AddEdge(4,5,30, *Graph, Vertices);
//
//    cout << endl;
//    for (int i = 0; i<Size ; i++)
//    {
//        cout << i << "\t";
//        for (int j = 0; j<Size ; j++)
//        {
//            cout << Graph[i][j].Weight << "\t";
//        }
//        cout << endl;
//    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].Asset << "\t";
////    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].Owes << "\t";
////    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].NetChange << "\t";
////    }
//
//    cout << endl << "\nYour transactions are... " << endl;
//    for (int i = 0; i<Size; i++)
//    {
//        for (int j = 0; j<Size ; j++)
//        {
//            if (Graph[i][j].Weight != 0)
//            {
//                cout << Vertices[i].Name << " lent " << Vertices[j].Name << " $" << Graph[i][j].Weight << endl;
//                Count += 1;
//            }
//        }
//    }
//    cout << "\nYou have a total of " << Count << " transactions." << endl;
//    cout << "\nReducing number of transactions.... " << endl;
//
//
//    SimplifyDebt(Vertices, *Graph, Stack);
//
//    Count = 0;
//    cout << "\nAfter simplification" << endl;
//    for (int i = 0; i<Size; i++)
//    {
//        for (int j = 0; j<Size ; j++)
//        {
//            if (Graph[i][j].Weight != 0)
//            {
//                cout << Vertices[j].Name << " owes " << Vertices[i].Name << " $" << Graph[i][j].Weight << endl;
//                Count += 1;
//            }
//        }
//    }
//    cout  << "\n" << Count << " transactions are made." << endl;
//
//    cout << endl;
//    for (int i = 0; i<Size ; i++)
//    {
//        cout << i << "\t";
//        for (int j = 0; j<Size ; j++)
//        {
//            cout << Graph[i][j].Weight << "\t";
//        }
//        cout << endl;
//    }
//
//
//
////    cout << "\n\n\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << 5 << endl;
////   cout << endl;
////    for (int i = 0; i<Size ; i++)
////    {
////        cout << i << "\t";
////        for (int j = 0; j<Size ; j++)
////        {
////            cout << Graph[i][j].Weight << "\t";
////        }
////        cout << endl;
////    }
////    cout << endl;
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].NetChange << "\t";
////    }
//}

////Example 3 - long 11
//int main()
//{
//    int Count = 0;
//
//    int Stack[Size];
//
//    Vertex Vertices [Size];
//
//    Vertices[0].Name = "Amy";
//    Vertices[1].Name = "Ben";
//    Vertices[2].Name = "Celia";
//    Vertices[3].Name = "Darcy";
//    Vertices[4].Name = "Ellis";
//    Vertices[5].Name = "Flynn";
//    Vertices[6].Name = "Glenn";
//    Vertices[7].Name = "Harry";
//    Vertices[8].Name = "Ian";
//    Vertices[9].Name = "Jackie";
//    Vertices[10].Name = "Kelsey";
//
//    Edge Graph [Size][Size];
//
//    AddEdge(0,2,30, *Graph, Vertices);
//    AddEdge(0,6,20, *Graph, Vertices);
//    AddEdge(0,7,10, *Graph, Vertices);
//    AddEdge(1,0,80, *Graph, Vertices);
//    AddEdge(1,2,90, *Graph, Vertices);
//    AddEdge(3,1,120, *Graph, Vertices);
//    AddEdge(4,5,10, *Graph, Vertices);
//    AddEdge(6,8,30, *Graph, Vertices);
//    AddEdge(6,10,20, *Graph, Vertices);
//    AddEdge(7,6,60, *Graph, Vertices);
//    AddEdge(7,10,10, *Graph, Vertices);
//    AddEdge(7,8,20, *Graph, Vertices);
//    AddEdge(8,9,50, *Graph, Vertices);
//    AddEdge(9,10,60, *Graph, Vertices);
//
//
////    cout << endl;
////    for (int i = 0; i<Size ; i++)
////    {
////        cout << i << "\t";
////        for (int j = 0; j<Size ; j++)
////        {
////            cout << Graph[i][j].Weight << "\t";
////        }
////        cout << endl;
////    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].Asset << "\t";
////    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].Owes << "\t";
////    }
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].NetChange << "\t";
////    }
//
//    cout << endl << "\nYour transactions are... " << endl;
//    for (int i = 0; i<Size; i++)
//    {
//        for (int j = 0; j<Size ; j++)
//        {
//            if (Graph[i][j].Weight != 0)
//            {
//                cout << Vertices[i].Name << " lent " << Vertices[j].Name << " $" << Graph[i][j].Weight << endl;
//                Count += 1;
//            }
//        }
//    }
//    cout << "\nYou have a total of " << Count << " transactions." << endl;
//    cout << "\nReducing number of transactions.... " << endl;
//
//
//    SimplifyDebt(Vertices, *Graph, Stack);
//    ClearStack(Stack);
//
//    int newCount = 0;
//    cout << "\nAfter simplification" << endl;
//    for (int i = 0; i<Size; i++)
//    {
//        for (int j = 0; j<Size ; j++)
//        {
//            if (Graph[i][j].Weight != 0)
//            {
//                cout << Vertices[j].Name << " owes " << Vertices[i].Name << " $" << Graph[i][j].Weight << endl;
//                newCount += 1;
//            }
//        }
//    }
//    cout  << "\n" << newCount << " transactions are made." << endl;
//    cout  << "\n" << Count - newCount << " transactions are removed." << endl;
//
////    cout << endl;
////    for (int i = 0; i<Size ; i++)
////    {
////        cout << i << "\t";
////        for (int j = 0; j<Size ; j++)
////        {
////            cout << Graph[i][j].Weight << "\t";
////        }
////        cout << endl;
////    }
////
////
////
////    cout << "\n\n\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << 5 << endl;
////   cout << endl;
////    for (int i = 0; i<Size ; i++)
////    {
////        cout << i << "\t";
////        for (int j = 0; j<Size ; j++)
////        {
////            cout << Graph[i][j].Weight << "\t";
////        }
////        cout << endl;
////    }
////    cout << endl;
////    cout << endl;
////    for (int j = 0; j<Size ; j++)
////    {
////        cout << Vertices[j].NetChange << "\t";
////    }
//}

////Example 1
//int main()
//{
//    Vertex Vertices [Size];
//
//    int Stack[Size];
//
//    Vertices[0].Name = "A";
//    Vertices[1].Name = "B";
//    Vertices[2].Name = "C";
//    Vertices[3].Name = "D";
//    Vertices[4].Name = "E";
//    Vertices[5].Name = "F";
//
//    Edge Graph [Size][Size];
//
//    cout << "\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << 5 << endl;
//    AddEdge(0,1,50, *Graph, Vertices);
//    AddEdge(0,2,20, *Graph, Vertices);
//    AddEdge(0,3,10, *Graph, Vertices);
//    AddEdge(0,5,10, *Graph, Vertices);
//    AddEdge(1,2,20, *Graph, Vertices);
//    AddEdge(1,4,10, *Graph, Vertices);
//    AddEdge(2,4,60, *Graph, Vertices);
//    AddEdge(2,5,30, *Graph, Vertices);
//    AddEdge(3,5,40, *Graph, Vertices);
//    AddEdge(4,5,30, *Graph, Vertices);
//
//    cout << endl;
//    for (int i = 0; i<Size ; i++)
//    {
//        cout << i << "\t";
//        for (int j = 0; j<Size ; j++)
//        {
//            cout << Graph[i][j].Weight << "\t";
//        }
//        cout << endl;
//    }
//    cout << endl;
//    for (int j = 0; j<Size ; j++)
//    {
//        cout << Vertices[j].Asset << "\t";
//    }
//    cout << endl;
//    for (int j = 0; j<Size ; j++)
//    {
//        cout << Vertices[j].Owes << "\t";
//    }
//    cout << endl;
//    for (int j = 0; j<Size ; j++)
//    {
//        cout << Vertices[j].NetChange << "\t";
//    }
//
//   SimplifyDebt(Vertices, *Graph, Stack);
//    cout << "\n\n\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << 5 << endl;
//   cout << endl;
//    for (int i = 0; i<Size ; i++)
//    {
//        cout << i << "\t";
//        for (int j = 0; j<Size ; j++)
//        {
//            cout << Graph[i][j].Weight << "\t";
//        }
//        cout << endl;
//    }
//    cout << endl;
//    cout << endl;
//    for (int j = 0; j<Size ; j++)
//    {
//        cout << Vertices[j].NetChange << "\t";
//    }
//}

void push(int* Stack, int value)
{
    Top++;
    if(Top==Size)
    {
        printf("\nstack overflow");
        return;
    }
    //cout << "v:" << value << endl;
    Stack[Top]=value;
}

void PrintStack(int* Stack)
{
    printf(" {");
    for(int c=0; c<Top+1; c++)
    {
        cout << Stack[c];
        if (c<Top)
        {
            printf(" , ");
        }
    }
    printf("}");
}

int pop()
{
    if (Top==-1)
    {
        printf("\nstack underflow");
        return -1;
    }
    Top--;
}

void ClearStack(int* Stack)
{
    if (Top==-1)
    {
        return;
    }
    while (Top!=-1)
    {
        Stack[Top] = 0;
        Top--;
    }
}

void ResetStatus (Vertex* Vertices)
{
    for (int i = 0; i<Size; i++)
    {
        Vertices[i].Status = 0;
    }
}
