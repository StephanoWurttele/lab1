#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include<stack>

using namespace std;

struct Alumno{
  char codigo[5];
  int ciclo;
  float mensualidad;
  int valido;
  Alumno(char _cod[5], int _ciclo, float _mensualidad){
    strcpy(codigo, _cod);
    ciclo = _ciclo;
    mensualidad = _mensualidad;
    valido = 0;
  };
  Alumno(){};
  void print(){
    cout << "------------------------------"<<endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Ciclo: " << ciclo << endl;
    cout << "Mensualidad: " << mensualidad << endl;
    cout << "Valido: " << valido << endl;
    cout << "------------------------------"<<endl;
  };
};

istream& operator>> (istream& stream, Alumno & record){
  stream.read((char*) &record, sizeof(record));
  return stream;
}

ostream& operator<< (ostream& stream, Alumno & record){
  stream.write((char*) &record, sizeof(record));
  return stream;
}

class FixedRecord{
  private:
    string filename;

  public:
    FixedRecord(string _filename):filename(_filename){
      fstream file(filename, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if (file.is_open()){
        if (file.tellg() == 0){
          int init = -1;
          file.write((char*) &init, sizeof(init));
          file << endl;
          file.close();
        }
      }
    };
    vector<Alumno> load(){
      fstream file(filename, ios::in | ios::out | ios::binary);
      Alumno temp;
      vector<Alumno> vec;
      if (file.is_open()){
        file.seekg(5);
        while(file >> temp){
          if(0 == 0)
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
          int register_pos = 5 + sizeof(record)*(curr_top-1);
          file.seekp(0, ios::beg);
          file.write((char*)&new_top, sizeof(new_top));
          file.seekp(register_pos, ios::beg);
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
        int valido;
        Alumno record;
        int register_pos = 5 + sizeof(Alumno)*(pos-1);
        file.seekg(0, ios::beg);
        file.read((char*)&curr_top, sizeof(curr_top));
        if(curr_top == -1){

          file.seekp(0, ios::beg);
          file.write((char*)&pos, sizeof(pos));

          file.seekg(register_pos, ios::beg);
          file.read((char*) &record, sizeof(record));
          record.valido = top;
          record.print();
          file.seekp(register_pos, ios::beg);
          file.write((char*)&record, sizeof(record));
        }
        else{
          file.seekg(register_pos, ios::beg);
          file.read((char*) &record, sizeof(record));
          valido = record.valido;
          record.print();

          if (valido == 0){
            record.valido = curr_top;
            record.print();
            file.seekp(register_pos, ios::beg);
            file.write((char*)&record, sizeof(record));
            
            file.seekp(0, ios::beg);
            file.write((char*)&pos, sizeof(pos));
          }
          else{ cout << "Already deleted" << endl; }
        }
        file.close();
      }
    }
};

int main(){
  string line;
  Alumno alumno1((char*)"0001", 6, 1000.5);
  Alumno alumno2((char*)"0002", 7, 1001.5);
  Alumno alumno3((char*)"0003", 8, 1002.5);
  FixedRecord fr("datos2.txt");
  fr.add(alumno1);
  fr.add(alumno2);
  fr.add(alumno3);
  fr.load();
  cout << endl << endl << endl;
  fr.Delete(2);
  fr.Delete(1);
  fr.Delete(1);
  cout << endl <<endl << endl;
  fr.load();
  fr.add(alumno2);
  cout << endl <<endl << endl;
  fr.load();
  return 0;
}
