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
  char valido;
  Alumno(char _cod[5], int _ciclo, float _mensualidad){
    strcpy(codigo, _cod);
    ciclo = _ciclo;
    mensualidad = _mensualidad;
    valido = '1';
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
  //stream << endl;
  return stream;
}

class FixedRecord{
  private:
    string filename;

  public:
    FixedRecord(string _filename):filename(_filename){};

    vector<Alumno> load(){
      ifstream file(filename, ios::binary);
      Alumno temp;
      vector<Alumno> vec;
      if (file.is_open()){
        cout<<"File opened in load" << endl;
        while(file >> temp){
          cout << "Codigo leido: " << temp.codigo <<endl;
          if(temp.valido == '1')
            vec.push_back(temp);
        }
        file.close();
      }
      cout << "PRINT FROM LOAD: " << endl;

      for(int i = 0; i < vec.size(); ++i){
        cout<< i << ": ";
        vec[i].print();
      }
      return vec;
    }
    void add(Alumno record){
      if(freeList.empty()){
        fstream file(filename, ios::out | ios::app | ios::binary);
        if(file.is_open()){
          cout<<"File opened in add" << endl;
          cout << "freeList empty before add: " << endl;
          /*cout<<"before seekp " << file.tellg() << endl;
          file.seekg(0, ios::end);
          cout<<"after seekp " << file.tellg() << endl;*/
          record.print();
          cout << "\n\n";
          file << record;
          file.close();
        }
      }
      else{
        fstream file(filename, ios::out | ios::ate | ios::binary);
        if(file.is_open()){
          int temp = freeList.top();
          freeList.pop();
          file.seekp(53*(temp-1));
          file << record;
          file.close();
        };
      }
    }
    bool Delete(int pos){
      ofstream file(filename, ios::binary);
      file.seekp(53*pos-2);
      file << '0';
      freeList.push(pos);
      file.close();
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
  //fr.Delete(1);
  //fr.Delete(2);
  //fr.Delete(3);
  //fr.add(alumno3);
  return 0;
}
