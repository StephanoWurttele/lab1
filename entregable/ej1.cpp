#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include<stack>

using namespace std;

struct Alumno{
  char codigo[5];
  char nombre[12];
  char apellidos[20];
  char carrera[15];
  int valido;
  Alumno(char _cod[5], char _nombre[12], char _apellidos[20], char _carrera[15]){
    strcpy(codigo, _cod);
    strcpy(nombre, _nombre);
    strcpy(apellidos, _apellidos);
    strcpy(carrera, _carrera);
    valido = 0;
  };
  Alumno(){};
  void print(){
    cout << "------------------------------"<<endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Apellidos: " << apellidos << endl;
    cout << "Carrera: " << carrera << endl;
    cout << "Valido: " << valido << endl;
    cout << "------------------------------"<<endl;
  };
};

istream& operator>> (istream& stream, Alumno & record){
  stream.read(record.codigo, 5);
  stream.read(record.nombre, 12);
  stream.read(record.apellidos, 20);
  stream.read(record.carrera, 15);
  stream.read((char*) &record.valido, sizeof(record.valido));
  //stream.read(record.valido, 4);
  stream.get();
  return stream;
}

ostream& operator<< (ostream& stream, Alumno & record){
  stream.write(record.codigo, 5);
  stream.write(record.nombre, 12);
  stream.write(record.apellidos, 20);
  stream.write(record.carrera, 15);
  stream.write((char*) &record.valido, sizeof(record.valido));
  //stream.write(record.valido, 1);
  stream << endl;
  return stream;
}

class FixedRecord{
  private:
    string filename;
    stack<int> freeList;

  public:
    FixedRecord(string _filename):filename(_filename){
      fstream file(filename, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if (file.is_open()){
        int init = -1;
        cout<<"tellg is "<<file.tellg()<<endl;
        if (file.tellg() == 0){
          file.write((char*) &init, sizeof(init));
          file << endl;
          file.close();
        }
        else {
          cout << "all good" << endl;
        }
      }
    };
    vector<Alumno> load(){
      fstream file(filename);
      Alumno temp;
      vector<Alumno> vec;
      if (file.is_open()){
        file.seekg(5);
        while(file >> temp){
          if(temp.valido == 0)
            vec.push_back(temp);
        }
        file.close();
        for( int i = 0; i < vec.size() ; ++i){
          vec[i].print();
        }
      }
      return vec;
    }
    void add(Alumno record){
      fstream file(filename, ios::in | ios::out | ios::binary);
      if(file.is_open()){
        int curr_top;
        file.seekg(0, ios::beg);
        file.read((char*)&curr_top, sizeof(curr_top));
        if(curr_top == -1){
          file.seekp(0, ios::end);
          file << record;
        }
        else{
          int new_top;
          file.seekg(5 + 57*(curr_top-1) + 52, ios::beg);
          file.read((char*)&new_top, sizeof(new_top));

          file.seekp(0, ios::beg);
          file.write((char*)&new_top, sizeof(new_top));

          file.seekp(5 + 57*(curr_top-1), ios::beg);
          file << record;

        };
        file.close();
      }
    }
    bool Delete(int pos){
      fstream file(filename, ios::in | ios::out | ios::binary);
      if(file.is_open()){
        int top = -1;
        int curr_top;
        int temp;
        int temp2;
        file.seekg(0, ios::beg);
        file.read((char*)&curr_top, sizeof(curr_top));
        if(curr_top == -1){

          file.seekp(0, ios::beg);
          file.write((char*)&pos, sizeof(pos));

          file.seekg(0, ios::beg);
          file.read((char*)&temp, sizeof(temp));

          file.seekp(5 + 57*(pos-1) + 52, ios::beg);
          file.write((char*)&top, sizeof(top));
        }
        else{
          file.seekg(5 + 57*(pos-1) + 52, ios::beg);
          file.read((char*)&temp2, sizeof(temp2));

          if (temp2 == 0){

            file.seekp(5 + 57*(pos-1) + 52, ios::beg);
            file.write((char*)&curr_top, sizeof(curr_top));
            
            file.seekp(0, ios::beg);
            file.write((char*)&pos, sizeof(pos));


            file.seekg(0, ios::beg);
            file.read((char*)&curr_top, sizeof(curr_top));
          }
          else{ cout << "Already deleted" << endl; }
        }
        file.seekg(0, ios::beg);
        file.read((char*)&curr_top, sizeof(curr_top));
        cout<<"top is now: " << curr_top << endl;
        file.close();
      }
    }
};

int main(){
  string line;
  Alumno alumno1((char*)"0001", (char*)"Howard", (char*)"Paredes Zegarra", (char*)"Computacion");
  Alumno alumno2((char*)"0002", (char*)"Penny", (char*)"Vargas Cordero", (char*)"Industrial");
  Alumno alumno3((char*)"0003", (char*)"Sheldon", (char*)"Cooper Quizpe", (char*)"Mecatronica");
  FixedRecord fr("datos1.txt");
  fr.add(alumno1);
  fr.add(alumno2);
  fr.add(alumno3);
  fr.load();
  fr.Delete(1);
  cout << endl <<endl << endl;
  fr.load();
  cout << endl <<endl << endl;
  fr.Delete(2);
  fr.Delete(1);
  cout << endl <<endl << endl;
  fr.load();
  fr.add(alumno1);
  cout << endl <<endl << endl;
  fr.load();
  return 0;
}
