#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include<stack>

using namespace std;

struct Alumno{
  string nombre;
  string apellidos;
  string carrera;
  float mensualidad;
  int valido;
  Alumno(string _nombre, string _apellidos, string _carrera, float _mensualidad){
    nombre = _nombre;
    apellidos = _apellidos;
    carrera = _carrera;
    mensualidad = _mensualidad;
  };
  Alumno(){};

  int size(){
    return nombre.size() + apellidos.size() + carrera.size() + sizeof(float) + 3;
  }

  void print(){
    cout << "------------------------------"<<endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Apellidos: " << apellidos << endl;
    cout << "Carrera: " << carrera << endl;
    cout << "Mensualidad: " << mensualidad << endl;
    cout << "------------------------------"<<endl;
  };
};

istream& operator>> (istream& stream, Alumno & record){
  getline(stream, record.nombre, '|');
  getline(stream, record.apellidos, '|');
  getline(stream, record.carrera, '|');
  stream.read((char*) &record.mensualidad, sizeof(float));
  return stream;
}

ostream& operator<< (ostream& stream, Alumno & record){
  stream.write(record.nombre.c_str(), record.nombre.size());
  stream << "|";
  stream.write(record.apellidos.c_str(), record.apellidos.size());
  stream << "|";
  stream.write(record.carrera.c_str(), record.carrera.size());
  stream << "|";
  stream.write((char*) &record.mensualidad, sizeof(record.mensualidad));
  return stream;
}

class FixedRecord{
  private:
    string file;
    string ind_file;

  public:
    FixedRecord(string _file):file(_file),ind_file("ind_"+_file){};
    vector<Alumno> load(){
      fstream fileRe(file);
      Alumno temp;
      vector<Alumno> vec;
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
    void add(Alumno record){
      fstream fileWr(file, ios::in | ios::out | ios::app);
      if(fileWr.is_open()){
        fstream fileInd(ind_file, ios::binary | ios::out | ios::app);
        if (fileInd.is_open()){
          fileWr.seekp(0, ios::end);
          long reg_position = fileWr.tellp();
          int reg_size = record.size();
          fileInd.write((char*) &reg_position, sizeof(long));
          fileInd.write((char*) &reg_size, sizeof(int));
          fileWr << record;
          cout << reg_position << " " << reg_size << endl;
          fileInd.close();
        }
        fileWr.close();
      }
    }

    Alumno readRecord(int pos){
      fstream fileRe(file);
      Alumno response;
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
  string line;
  Alumno alumno1("Howardaaa", "Paredes Zegarra", "Computacion", 255);
  Alumno alumno2("Penny", "Hofstadter Zegarra", "Mecanica", 254);
  Alumno alumno3("Sheldon", "Cooper Zegarra", "Computacion", 253);
  FixedRecord fr("datos3.txt");
  fr.add(alumno1);
  fr.add(alumno2);
  fr.add(alumno3);
  fr.load();
  fr.readRecord(3);
  return 0;
}
