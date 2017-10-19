#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

template <typename Type>
class List{
	std::vector<Type> v;
	int size;
public:
	List(int size_):size(size_){
		this->v = std::vector<Type>(size_);
	};

	~List(){
		cout << "Detroy List" << endl;
		/*for(int i=0;i<this->v.size();i++){
			this->v.erase(this->v.begin() +1);
		}*/
	}
	int Get_size(){
		return this->size;
	}
};

class Curve{
	std::vector< List<double> > v;
	string id;
public:
	Curve(string id_,int dimension,int m):id(id_),v(m,List <double> (dimension) ){
		cout <<"Curve created" << endl;
		cout << this->v[0].Get_size() << endl;
	};
	~Curve(){
		cout << "Destroy Curve" << endl;
		/*for(int i=0;i<this->v.size();i++){
			this->v.erase(this->v.begin() +1);
		}*/
	}
};

int read_curves(string path)
{
	string STRING;
	ifstream infile;
	infile.open (path.c_str());
	int k = 0;
	bool dim = false;
	int dimension = 2;
	while(!infile.eof()) // To get you all the lines.
	{
		int m = 0;
		string id;
		int size;
		getline(infile,STRING); // Saves the line in STRING.
		string delimiter = "\t";
		size_t pos = 0;
		string token;
		while ((pos = STRING.find(delimiter)) != string::npos) {
			token = STRING.substr(0, pos);
			if((k == 0)){
				if(!dim && !token.compare("@dimension")){
					dim = true;
					cout << "Dimension: " ;
				}
			}
			else{
				switch(m){
					case 0:
						id = token;
						break;
					case 1:
						std::istringstream(token) >> size;
						break;
					default:
						break;
				}
				cout << token << "\t";
				m++;
			}
			STRING.erase(0, pos + delimiter.length());
		}
		if(k == 0){
			std::istringstream(STRING) >> dimension;	
		}
		Curve c("hello",3,2);
		cout << STRING << endl;
		k++;
	}
	infile.close();
	return 0;
}

int main(int argc, char *argv[]){
	if(argc != 2){
		cerr << "Give path for reading curves" << endl;
		return 1;
	}
	return read_curves(argv[1]);

}