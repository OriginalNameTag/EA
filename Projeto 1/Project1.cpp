#include <iostream>
#include <vector>

using namespace std;

int generated = 0;

struct Node{
    vector <vector<int>> qr;
    Node* left;
    Node* right;
};


void generate_qrs(vector<vector<int>> qr);


void print_qr(int size_qr, vector<vector<int>> qr){

    cout << "+";
    for(int i=0; i<size_qr; ++i){
        cout << "-";
    }
    cout << "+\n";
    
    for(size_t i=0; i< qr.size(); ++i){
        cout << "|";
        for(size_t j=0; j< qr[i].size(); ++j){
            if(qr[i][j] == 1){
                cout << " ";
            }
            else{
                cout << "#";
            }
        }
        cout << "|\n"; 
    }

    cout << "+";
    for(int i=0; i<size_qr; ++i){
        cout << "-";
    }
    cout << "+\n";
    

    return;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    vector <int> lb; //Lines
    vector <int> cb; //Columns
    vector <int> lt; //Transitions in line
    vector <int> ct; //Transitions in column
    vector <int> qb; //Black cells in each quadrant
    vector <int> db; //Black cells in each diagonal

    vector <vector <int>> qr;

    int number_qrs;
    cin >> number_qrs;

    int size_qr, aux;
    //Read number of given inputs at the start
    for(int i=0; i<number_qrs; ++i){
        //Read size of Lines anc Columns
        cin >> size_qr;
        //Read lines
        for(int j=0; j<size_qr; ++j){
            cin >> aux;
            lb.push_back(aux);            
        }
        //Read Columns
        for(int j=0; j<size_qr; ++j){
            cin >> aux;
            cb.push_back(aux);            
        }
        //Read Transitions in Lines
        for(int j=0; j<size_qr; ++j){
            cin >> aux;
            lt.push_back(aux);            
        }
        //Read Transitions in Columns
        for(int j=0; j<size_qr; ++j){
            cin >> aux;
            ct.push_back(aux);            
        }
        //Read Black cells in each quadrant
        for(int j=0; j<4; ++j){
            cin >> aux;
            qb.push_back(aux);
        }
        //Read black cells in each diagonal
        for(int j=0; j<2; ++j){
            cin >> aux;
            db.push_back(aux);
        }
        qr.push_back(lb);
        qr.push_back(cb);
        qr.push_back(lt);
        qr.push_back(ct);
        qr.push_back(qb);
        qr.push_back(db);

        //Process QR
        generate_qrs(qr);
        

        //We clear the input lines 
        qr.clear();
        lb.clear();
        cb.clear();
        lt.clear();
        ct.clear();
        qb.clear();
        db.clear();
    }
    
    return 0;
}

void generate_qrs(vector <vector<int>> qr){

    
    return;
}

void generate_lb(vector<int> lb, vector<int> lt){
    


    return;
}

