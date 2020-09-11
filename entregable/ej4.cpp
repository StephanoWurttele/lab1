#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include<stack>

using namespace std;

struct Matricula{
  string codigo;
  int ciclo;
  float mensualidad;
  string observaciones;
  Matricula(string _codigo, int _ciclo, float _mensualidad, string _observaciones){
    codigo = _codigo;
    ciclo = _ciclo;
    mensualidad = _mensualidad;
    observaciones = _observaciones;
  };
  Matricula(){};

  int size(){
    return codigo.size() + sizeof(int) + observaciones.size() + sizeof(float) + 4;
  }

  void print(){
    cout << "------------------------------"<<endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Ciclo: " << ciclo << endl;
    cout << "Mensualidad: " << mensualidad << endl;
    cout << "Observaciones: " << observaciones << endl;
    cout << "------------------------------"<<endl;
  };
};

istream& operator>> (istream& stream, Matricula & record){
  int size;
  stream.read((char*) &size, sizeof(int));
  getline(stream, record.codigo, '|');
  stream.read((char*) &record.ciclo, sizeof(int));
  stream.seekg((long)(stream.tellg())+1,ios::beg);
  stream.read((char*) &record.mensualidad, sizeof(float));
  stream.seekg((long)(stream.tellg())+1,ios::beg);

  getline(stream, record.observaciones, '\n');
  return stream;
}

ostream& operator<< (ostream& stream, Matricula & record){
  stream.write(record.codigo.c_str(), record.codigo.size());
  stream << "|";
  stream.write((char*) &record.ciclo, sizeof(record.ciclo));
  stream << "|";
  stream.write((char*) &record.mensualidad, sizeof(record.mensualidad));
  stream << "|";
  stream.write(record.observaciones.c_str(), record.observaciones.size());
  stream<<"\n";

  return stream;
}

class FixedRecord{
  private:
    string file;
    string ind_file;

  public:
    FixedRecord(string _file):file(_file),ind_file("ind_"+_file){};
    vector<Matricula> load(){
      fstream fileRe(file);
      Matricula temp;
      vector<Matricula> vec;
      if (fileRe.is_open()){
        while(fileRe >> temp){
          vec.push_back(temp);
        }
        fileRe.close();
        for( int i = 0; i < vec.size() ; ++i){
          vec[i].print();
        }
      }
      return vec;
    }
    void add(Matricula record){
      fstream fileWr(file, ios::in | ios::out | ios::app);
      if(fileWr.is_open()){
        fstream fileInd(ind_file, ios::binary | ios::out | ios::app);
        if (fileInd.is_open()){
          fileWr.seekp(0, ios::end);
          long reg_position = fileWr.tellp();
          int reg_size = record.size() + sizeof(int);
          cout << reg_position << " " << reg_size << endl;
          fileInd.write((char*) &reg_position, sizeof(long));
          fileInd.write((char*) &reg_size, sizeof(int));
          fileWr.write((char*) &reg_size, sizeof(int));
          fileWr << record;
          fileInd.close();
        }
        fileWr.close();
      }
    }

    Matricula readRecord(int pos){
      fstream fileRe(file);
      Matricula response;
      if(fileRe.is_open()){
        fstream fileInd(ind_file, ios::binary | ios::in);
        if (fileInd.is_open()){
          int find_pos = (sizeof(long)+sizeof(int))*(pos-1);
          fileInd.seekg(find_pos);
          long pos;
          fileInd.read((char*)&pos, sizeof(long));
          fileRe.seekg(pos);
          fileRe >> response;
          fileInd.close();
        }
        fileRe.close();
      }
      response.print();
      return response;
    }
};

int main(){
  Matricula matricula1("0001", 5, 10.5, "Observaciones1");
  Matricula matricula2("0002", 6, 11.5, "Observaciones2");
  Matricula matricula3("0003", 7, 12.5, "Observaciones3");
  FixedRecord fr("datos4.txt");
  fr.add(matricula1);
  fr.add(matricula2);
  fr.add(matricula3);
  fr.load();
  fr.readRecord(1);
  return 0;
}
