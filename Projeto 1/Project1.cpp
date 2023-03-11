#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int generated = 0;

vector <int> lb; //Lines
vector <int> cb; //Columns
vector <int> lt; //Transitions in line
vector <int> ct; //Transitions in column
vector <int> qb; //Black cells in each quadrant
vector <int> db; //Black cells in each diagonal

vector<int> quadrants = {0,0,0,0};

bool validate(int size);

void preprocess(vector<vector<int>>&qr);
void generate_qrs(vector<vector<int>>& qr, int line, int collumn);

bool validateLineBlack(vector<int> line, int collumn, int lineb);
bool validateLineTransactions(vector<int> line, int linet, int x);

bool validateCollumnBlack(vector<vector<int>> qr, int line, int collumn);
bool validataCollumnTransactions(vector<vector<int>> qr, int collumnt);

bool checkLine(vector<vector<int>> qr, int line, int col, int nLine);
bool checkCol(vector<vector<int>> qr, int line, int col, int nCol);

bool checkQuadrants(vector<vector<int>> qr);


void print_qr(int size_qr, vector<vector<int>> qr){

	cout << "+";
	for(int i=0; i<size_qr; ++i){
			cout << "-";
	}
	cout << "+\n";
	
	for(int i=0; i<size_qr; ++i){
		cout << "|";
		for(int j=0; j<size_qr; ++j){
				if(qr[i][j] == 0){
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

			vector<vector<int>> qr(size_qr, vector<int>(size_qr, 0));
			//Validate input to check for defects and then we generate qr
			if(validate(size_qr)){
				preprocess(qr);
				//generate_qrs(qr, 0, 0);
			}
			

			if(generated == 0){
				cout << "DEFECT: No QR Code generated!\n";
			}
			else if(generated == 1){
				cout << "Valid case: 1 QR code decoded\n";
				print_qr(size_qr, qr);
			}
			else{
				cout << "INVALID: " << generated << " QR Codes generated\n";
			}

			//We clear the input lines and reset generated qrs
			qr.clear();
			lb.clear();
			cb.clear();
			lt.clear();
			ct.clear();
			qb.clear();
			db.clear();
			
			generated = 0;
	}
	
	return 0;
}

//stuff to add check bellow
bool validate(int size_qr){
	//lb, cb, lt, ct, qb, db

	//Missing some validations of input here?
	for(int i = 0; i< size_qr; ++i){
		if(lt[i] == 0 && ( lb[i]>0 && lb[i]<size_qr)){
			//cout<<"here1";
			return false;
		}
		if(ct[i] == 0 && ( cb[i]>0 && cb[i]<size_qr)){
			//cout<<"here2";
			return false;
		}
	}
	//We can't check if the number of blacks in each quadrant is right but we can do them all or 2 of them at a time
	int linesum = 0;
	int columnsum = 0;
	int qsum = qb[0] + qb[1] + qb[2] + qb[3];

	for(int i=0; i<size_qr; ++i){
		linesum += lb[i];
		columnsum += cb[i];
	}

	if(linesum != qsum || columnsum != qsum){
		//cout<<"here3";
		return false;
	}
	//Quadrant 1+2 1+4 or 3+2 3+4 
	//dont think we need floor cause it automatically rounds down?
	int lines21 = 0;
	int lines34 = 0;
	int columns23 = 0;
	int columns14 = 0;

	//only one loop needed for both of them
	for(int i = 0; i < size_qr/2; ++i){
		lines21 += lb[i];
		columns23 += cb[i];
	}
	for(int i = size_qr/2; i < size_qr; ++i){
		lines34 += lb[i];
		columns14 += ct[i];
	}

	if((qb[0]+qb[1]) != lines21 || (qb[1]+qb[2]) != columns23 || (qb[0]+qb[3]) != columns14 || (qb[2]+qb[3]) != lines34){
		//cout <<"here4";
		return false;
	}

	//Validate diagonals, if N is odd we can see if diagonals work together
	if(size_qr%2 == 1){
		if((db[0] == size_qr && db[1] == 0) || (db[0] == 0 && db [1] == size_qr)){
			return false;
		}

	}
	//cout <<"here5";
	return true;


}
//NEW CONDITION, also need to be put in validation
//if transactions = n - 1 and line or collumn == n/2+1 then you can fill it black white alterning until the end
//we can't see if the edges are black or white but we can fill the inside if lt = 1 and lb = n - 1

//so fun :s
//Intended to fill out the black cells in the matrix that are obivous given the input
//need to differ from 0 and 1 so we'll use 2 and 3 for whites and blacks most obvious example are diagonals=N or 0, same with transactions
//need to save filled cells to check quadrants?
void preprocess(vector<vector<int>>& qr){
	int size_qr = (int)qr.size();

	//Fill lines with black or white with accordance of transactions
	//assign() is broken, well it's only efficient to change a whole line, collumns for loop is better
	//fill() works as well??
	for(int i=0; i<size_qr; ++i){
		if(lb[i] == 0){
			qr[i].assign(size_qr, 2);
		}
		else if(lb[i] == size_qr){
			qr[i].assign(size_qr, 3);
		}
		else if(lt[i] == size_qr - 1 && lb[i] == size_qr/2 + 1){
			//To alter we see it it's odd or not
			for(int j=0; j<size_qr; ++j){
				if (j % 2 == 0) {
					qr[i][j] = 2;
				} 
				else {
					qr[i][j] = 3;
        		}
			}
		}
		//Fill inside besides edges
		else if(lt[i] == 1 && lb[i] == size_qr-1){
			fill(qr[i][1], qr[i][size_qr-1], 3);
		}
		else if(lt[i] == 1 && lb[i] == 1){
			fill(qr[i][1], qr[i][size_qr-1], 2);
		}
	}
	//collumns
	for(int i=0; i<size_qr; ++i){
		if(cb[i] == 0){
			for(int j=0; j<size_qr; ++j){
				qr[j][i] = 2;
			}
		}
		else if(cb[i] == size_qr){
			for(int j=0; j<size_qr; ++j){
				qr[j][i] = 3;
			}
		}
		else if(ct[i] == size_qr -1 && cb[i] == size_qr/2 + 1){
			for(int j=0; j<size_qr; ++j){
				if(j % 2 == 0){
					qr[j][i] == 2;
				}
				else{
					qr[j][i] == 3;
				}
			}
		}
	}

	//If Diagonal equals size of QR, then it'll be all black
	if(db[0] == size_qr){
		for(int i=0; i<size_qr; ++i){
			for(int j=0; j<size_qr; ++j){
				if(i==j){
					qr[i][j] = 3;
				}

			}
		}
	}
	//If Diagonal equals zero then it'll all be white
	else if(db[0] == 0){
		for(int i=0; i<size_qr; ++i){
			for(int j=0; j<size_qr; ++j){
				if(i==j){
					qr[i][j] = 2;
				}
			}
		}
	}
	if(db[1] == size_qr){
		for(int i=0; i<size_qr; ++i){
			for(int j=0; j<size_qr; ++j){
				if(i == size_qr-j-1){
					qr[i][size_qr-j-1] = 3;
				}
			}
		}
	}
	else if(db[1] == 0){
		for(int i=0; i<size_qr; ++i){
			for(int j=0; j<size_qr; ++j){
				if(i == size_qr-j-1){
					qr[i][size_qr-j-1] = 2;
				}
			}
		}
	}
	//Quadrants


	return;
}



//will this ever be finished :c
void generate_qrs(vector<vector<int>>& qr, int line, int collumn){
	int size_qr = qr.size();
	//If line and collumn are the last one, we generated a qr sucessfully(?)
	if(line==size_qr-1 && collumn == size_qr){
		++generated;
	}
	//Alter Line and reset Collumn
	else if(collumn==size_qr && line<size_qr){
		++line;
		collumn = 0;
	}
	//change to black
	qr[line][collumn] = 1;

	//Validations
	if(validateLineBlack(qr[line], collumn, lb[line]) && validateCollumnBlack(qr, line, collumn)){
		//generate_qrs(qr, line, collumn+1);

	}
	
	//After validations we alter qr[line][collumn] to black

	//more validations?
	
	return;
}

//Messy content
bool validateLineBlack(vector<int> line, int collumn, int lineb){
	int counter = 0;
	for(int i = 0; i < collumn; ++i){
		if(line[i] == 1){
			++counter;
		}
	}

	if(counter<=lb[collumn] && collumn <line.size()-1){
		return true;	
	}
	
	return false;
	
}

bool validateCollumnBlack(vector<vector<int>> qr, int line, int collumn){
	
	int size_collumn = qr.size();
	int counter = 0;
	
	for(int i = 0; i < size_collumn; ++i){
		if(qr[i][collumn] == 1){
			++counter;
		}
	}
	if(counter  <= cb[collumn] && line != size_collumn){
		return true;
	}

	return false;
	
}

bool validataCollumnTransactions(vector<vector<int>> qr, int collumnt){
	int counter = 0;
	vector<vector<int>> outtrans(qr[0].size(),vector<int>(qr.size()));

    for (size_t i = 0; i < qr.size(); ++i){
        for (size_t j = 0; j < qr[0].size(); ++j){
            outtrans[j][i] = qr[i][j];
        }
    }

	if (validateLineTransactions(outtrans[collumnt], collumnt, 1)){
        return true;
    } else {
        return false;
    }
}

bool validateLineTransactions(vector<int> line, int linet, int x){
    int counter = 0;

    for(int i=0; i < line.size()-1; ++i){
        if(line[i] != line[i+1]){
            counter++; 
        }
    }

	if (x == 0){
        if (counter==lt[linet]){
            return true;
		}
        return false;
    } else {
        if (counter==ct[linet]){
            return true;
        }
        return false;
    }
}

// Check if the number of black elements in a line is correct
bool checkLine(vector<vector<int>> qr, int line, int col, int nLine){
    int total = 0;
    
    for (int i = 0; i < qr[col].size(); i++){
        total = total + qr[line][i];
    }

    if (total == nLine){
        return true;
    } else {
        return false;
    }
}

// Check if the number of black elements in a collumn is correct
bool checkCol(vector<vector<int>> qr, int line, int col, int nCol){
    int total = 0;
    
    for (int i = 0; i < qr[line].size(); i++){
        total = total + qr[i][col];
    }

	if(total == nCol){
        return true;
    } else {
        return false;
    }
}

// Check the number of black pieces in each quadrant
bool checkQuadrants(vector<vector<int>> qr){
	int q1 = 0;
	int q2 = 0;
	int q3 = 0;
	int q4 = 0;
	int n = qr.size();
	
	for (int i = 0; i < n; i++){
		for (int j = 0; j < qr[i].size(); j++){
			// Quadrant 1
			if (i+1 <= floor(n/2) && j+1 > floor(n/2) ){
				q1 = q1 + qr[i][j];
			}
			// Quadrant 2
			if (i+1 <= floor(n/2) && j+1 <= floor(n/2) ){
                q2 = q2 + qr[i][j];
			}
			// Quadrant 3
			if (i+1 > floor(n/2) && j+1 <= floor(n/2) ){
                q3 = q3 + qr[i][j];
			}
			// Quadrant 4
			if (i+1 > floor(n/2) && j+1 > floor(n/2) ){
                q4 = q4 + qr[i][j];
			}
		}
	}
	return true;
}