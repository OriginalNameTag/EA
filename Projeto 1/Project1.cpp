#include <iostream>
#include <vector>
#include <set>
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

set<vector<vector<int>>> impossible;
vector<vector<int>> final_qr;

bool validate(int size);

void preprocess(vector<vector<int>>& qr);
void generate_qrs(vector<vector<int>>& qr, int line, int column);

bool possible(vector<vector<int>> qr);
void add_impossible(vector<vector<int>> qr);

bool validateLine(vector<int> line, int column, int lb);
bool validateLineTransactions(vector<int> line, int linet, int x);

bool validateCollumn(vector<vector<int>> qr, int line, int column);
bool validataCollumnTransactions(vector<vector<int>> qr, int column);

bool validateDiagonals(vector<vector<int>> qr, int line, int column);
bool validateQuadrants(vector<vector<int>> qr, int line, int column);

void print_qr(vector<vector<int>> qr);

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
				
				//preprocesser not return properly, maybe it gets stuck somewhere(?)
				preprocess(qr);
				generate_qrs(qr, 0, 0);
				print_qr(qr);

				//generate_qrs(qr, 0, 0);
			}
			

			if(generated == 0){
				cout << "DEFECT: No QR Code generated!\n";
			}
			else if(generated == 1){
				cout << "Valid case: 1 QR code decoded\n";
				//print_qr(qr);
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

//maybe using diagonals like i==j to process both collumns and lines is better
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
		if(lt[i] == 1 && lb[i] == size_qr-1){
			//fill(qr[i][1], qr[i][size_qr-1], 3);
			for(int j=1; j<size_qr-1; ++j){
				qr[i][j] = 3;
			}
		}
		else if(lt[i] == 1 && lb[i] == 1){
			//fill(qr[i][1], qr[i][size_qr-1], 2);
			for(int j=1; j<size_qr-1; ++j){
				qr[i][j] = 2;
			}
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
					qr[j][i] = 2;
				}
				else{
					qr[j][i] = 3;
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
	//Quadrants === something wrong here ===
	/*
	int size;
	bool q1 = false;
	bool q2 = false;
	bool q3 = false;
	bool q4 = false;

	if(size_qr%2 == 0){
		size = size_qr/2;
		if(qb[0] == size*size){
			q1 = true;
		}
		if(qb[1] == size*size){
			q2 = true;
		}
		if(qb[2] == size*size){
			q3 = true;
		}
		if(qb[3] == size*size){
			q3 = true;
		}
	}
	else{
		size = size_qr/2 + 1;
		if(qb[0] == size*(size-1)){
			q1 = true;
		}
		if(qb[1] == (size-1)*(size-1)){
			q2 = true;
		}
		if(qb[2] == (size)*(size-1)){
			q3 = true;
		}
		if(qb[3] == (size)*(size)){
			q3 = true;
		}
	}

	if(q1 || q2 || q3 || q4 || qb[0] == 0 || qb[1] == 0 || qb[2] == 0 || qb[3] == 0){
		for (int i = 1; i < size_qr+1; i++) {
			for (int j = 1; j < size_qr+1; j++) {

				//First Quadrant
				if (i <= size_qr / 2 && j > size_qr / 2 && (q1 || qb[0] == 0)){
					qr[i][j] = q1 ? 2 : 3;
				}
				//Second Quadrant
				else if (i <= size_qr / 2 && j <= size_qr / 2 && (q2 || qb[1] == 0)){
					qr[i][j] = q2 ? 2 : 3;
				}
				//Third Quadrant
				else if (i > size_qr / 2 && j <= size_qr / 2 && (q3 || qb[2] == 0)){
					qr[i][j] = q3 ? 2 : 3;
				}
				//Fourth Quadrant
				else if (i > size_qr / 2 && j > size_qr / 2 && (q4 || qb[3] == 0)){
					qr[i][j] = 3;
				}
			}
		}
	}
	*/
	return;
}



//will this ever be finished :c
//was thinking if this function wouldnt work to generate multiple qrs, but since we dont
//have to store more than one qr we can work with the same just copy it to not alter the ones in recursion
void generate_qrs(vector<vector<int>>& qr, int line, int column){
	int size_qr = (int)qr.size();
	if(!possible(qr)){
		return;
	}

	//Switch Line and reset Collumn
	if(column == size_qr && line<size_qr){
		++line;
		column = 0;
	}
	//If line equals size of qr then it's over, dont need to see collumns
	if(line==size_qr){
		++generated;
		final_qr = qr;
		return;
	}
	vector<vector<int>> copy = qr;
	//deal with the pre processing
	//If white (2)
	if(copy[line][column] == 2){
		if(validateLine(copy[line], column, lb[line])){
			generate_qrs(copy, line, column+1);
		}
		impossible.insert(copy);
		return;
	}

	//change to black
	qr[line][column] = 1;

	//Validations
	//if(validateLineBlack(qr[line], column, lb[line]) && validateCollumnBlack(qr, line, column)){
		//generate_qrs(qr, line, collumn+1);

	//}
	
	//After validations we alter qr[line][collumn] to black

	//more validations?
	
	return;
}

bool possible(vector<vector<int>> qr){
    auto inserting = impossible.find(qr);
    return inserting == impossible.end(); 
}
void add_impossible(vector<vector<int>> qr){
	impossible.insert(qr);
	return;
}


bool validateLine(vector<int> line, int column, int lb){
	int counter = 0;
	int size_qr = (int)line.size();

	for(int i=0; i<column; ++i){
		if(line[i] == 1 || line[i] == 3){
			++counter;
		}
	}
	if(counter <= lb && column<size_qr-1){
		return true;
	}
	return true;
}

bool validateCollumn(vector<vector<int>> qr, int line, int column){
	int counter = 0;

	for(int i = 0; i< line; ++i){
		if(qr[column][i] == 1 || qr[column][i] == 3){
			++counter;
		}
	}
	return true;
}
bool validataCollumnTransactions(vector<vector<int>> qr, int collumnt){
	return true;
}

bool validateDiagonals(vector<vector<int>> qr, int line, int column){
	int diagonal1 = 0;
	int diagonal2 = 0;
	int size_qr = (int)qr.size();
	
	for(int i=0; i<line; ++i){
		for(int j=0; j<column; ++column){
			if(i==j &&(qr[column][i] == 1 || qr[column][i] == 3)){
				++diagonal1;
			}
			if(size_qr-i-1 == j && (qr[column][i] == 1 || qr[column][i] == 3)){
				++diagonal2;
			}
		}
	}

	if(diagonal1 > db[0] || diagonal2 > db[1]){
		return false;
	}
	//missing validations
	return true;
}
bool validateQuadrants(vector<vector<int>> qr, int line, int collumn){
	return true;
}

void print_qr(vector<vector<int>> qr){
	int size_qr = (int)qr.size();
	cout << "+";
	for(int i=0; i<size_qr; ++i){
			cout << "-";
	}
	cout << "+\n";
	
	for(int i=0; i<size_qr; ++i){
		cout << "|";
		for(int j=0; j<size_qr; ++j){
				if(qr[i][j] %2 == 0){
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
