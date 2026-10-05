#include <iostream>
using namespace std;

class Mahasiswa {
public:
    string nama;
    int umur;

    void perkenalan() {
        cout << "Halo, nama saya " << nama << endl;
        cout << "Umur saya " << umur << " tahun" << endl;
    }
};

int main() {

    Mahasiswa mhs1;

    mhs1.nama = "Rayyan Ahmad";
    mhs1.umur = 18;

    mhs1.perkenalan();


    return 0;
}