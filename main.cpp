/* 
 * File:   main.cpp
 * Author: cuhellub
 *
 * Created on April 11, 2015, 9:54 AM
 */

#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;
#endif /* __PROGTEST__ */

class CPerson {
public:
    CPerson(string, string);
    string GetName(void);
    string GetSurname(void);
private:
    string name, surname;
};

CPerson::CPerson(string name, string surname) {
    this->name = name;
    this->surname = surname;
}

class CRecord {
public:
    CRecord(string, string, string, unsigned);
    CRecord(string, string);
    CRecord(string);
    CRecord(unsigned);
    bool operator<(const CRecord&) const;
    bool operator>(const CRecord&) const;
    int CompareByRz(const CRecord&) const;
    int CompareByVin(const CRecord&) const;
    string GetName(void) const;
    string GetSurname(void) const;
    string GetRz(void) const;
    unsigned GetVin(void) const;
    unsigned GetByNameIndex() const;
    unsigned GetByRzIndex() const;
    unsigned GetByVinIndex() const;
    vector<CPerson> GetOwnerHistory(void) const;
    void SetName(string);
    void SetSurname(string);
    void SetByNameIndex(unsigned byNameIndex);
    void SetByRzIndex(unsigned byVinIndex);
    void SetByVinIndex(unsigned byRzIndex);
#ifndef __PROGTEST__
    friend ostream& operator<<(ostream&, const CRecord&);
#endif /* __PROGTEST__ */
private:
    string name, surname, rz;
    unsigned vin, byNameIndex, byRzIndex, byVinIndex;
    vector<CPerson> ownerHistory;
};

CRecord::CRecord(string name, string surname, string rz, unsigned vin) {
    this->name = name;
    this->surname = surname;
    this->rz = rz;
    this->vin = vin;
    this->byNameIndex = 0;
    this->byRzIndex = 0;
    this->byVinIndex = 0;
    this->ownerHistory.push_back(CPerson(name, surname));
}

CRecord::CRecord(string name, string surname) {
    this->name = name;
    this->surname = surname;
    this->rz = "";
    this->vin = 0;
    this->byNameIndex = 0;
    this->byRzIndex = 0;
    this->byVinIndex = 0;
}

CRecord::CRecord(string rz) {
    this->name = "";
    this->surname = "";
    this->rz = rz;
    this->vin = 0;
    this->byNameIndex = 0;
    this->byRzIndex = 0;
    this->byVinIndex = 0;
}

CRecord::CRecord(unsigned vin) {
    this->name = "";
    this->surname = "";
    this->rz = "";
    this->vin = vin;
    this->byNameIndex = 0;
    this->byRzIndex = 0;
    this->byVinIndex = 0;
}

bool CRecord::operator<(const CRecord& x) const {
    return (this->surname < x.surname
            || (this->surname == x.surname
            && this->name < x.name)
            );
}

bool CRecord::operator>(const CRecord& x) const {
    return (this->surname > x.surname
            || (this->surname == x.surname
            && this->name > x.name)
            );
}

int CRecord::CompareByRz(const CRecord& x) const {
    if (this->rz < x.rz) return -1;
    if (this->rz > x.rz) return 1;
    return 0;
}

int CRecord::CompareByVin(const CRecord& x) const {
    if (this->vin < x.vin) return -1;
    if (this->vin > x.vin) return 1;
    return 0;
}

string CRecord::GetName() const {
    return this->name;
}

string CRecord::GetSurname() const {
    return this->surname;
}

string CRecord::GetRz() const {
    return this->rz;
}

unsigned CRecord::GetVin() const {
    return this->vin;
}

unsigned CRecord::GetByNameIndex(void) const {
    return this->byNameIndex;
}

unsigned CRecord::GetByRzIndex(void) const {
    return this->byRzIndex;
}

unsigned CRecord::GetByVinIndex(void) const {
    return this->byVinIndex;
}

vector<CPerson> CRecord::GetOwnerHistory(void) const {
    return this->ownerHistory;
}

void CRecord::SetByNameIndex(unsigned byNameIndex) {
    this->byNameIndex = byNameIndex;
}

void CRecord::SetByRzIndex(unsigned byRzIndex) {
    this->byRzIndex = byRzIndex;
}

void CRecord::SetByVinIndex(unsigned byVinIndex) {
    this->byVinIndex = byVinIndex;
}

void CRecord::SetName(string name) {
    this->name = name;
}

void CRecord::SetSurname(string surname) {
    this->surname = surname;
}

class CCarList {
public:
    CCarList(CRecord ** byName, unsigned firstIndex, unsigned lastIndex);
    CCarList(void);
    string RZ(void) const;
    unsigned VIN(void) const;
    bool AtEnd(void) const;
    void Next(void);
private:
    CRecord ** byName;
    unsigned firstIndex, lastIndex;
    bool empty;
};

CCarList::CCarList(CRecord** byName, unsigned firstIndex, unsigned lastIndex) {
    this->byName = byName;
    this->firstIndex = firstIndex;
    this->lastIndex = lastIndex;
    this->empty = false;
}

CCarList::CCarList() {
    this->empty = true;
}

string CCarList::RZ() const {
    return byName[firstIndex]->GetRz();
}

unsigned CCarList::VIN() const {
    return byName[firstIndex]->GetVin();
}

bool CCarList::AtEnd() const {
    if (empty) return true;
    return (firstIndex == lastIndex + 1);
}

void CCarList::Next() {
    firstIndex++;
}

class COwnerList {
public:
    COwnerList(CRecord * carRecord);
    string Name(void) const;
    string Surname(void) const;
    bool AtEnd(void) const;
    void Next(void);
private:
    vector<CPerson>::reverse_iterator revIterator;
    CRecord * carRecord;
};

COwnerList::COwnerList(CRecord * carRecord){
    this->carRecord = carRecord;
    this->revIterator = carRecord->GetOwnerHistory().rbegin();
}

string COwnerList::Name() const {
    return (*revIterator).GetName();
}

string COwnerList::Surname() const {
    return (*revIterator).GetSurname();
}

void COwnerList::Next() {
    this->revIterator++;
}

bool COwnerList::AtEnd() const {
    return revIterator == carRecord->GetOwnerHistory().rend();
}

class CRegister {
public:
    CRegister(void);
    ~CRegister(void);
    bool AddCar(const string & rz,
            unsigned int vin,
            const string & name,
            const string & surname);
    bool DelCar(const string & rz);
    bool DelCar(unsigned int vin);
    bool Transfer(const string & rz,
            const string & nName,
            const string & nSurname);
    bool Transfer(unsigned int vin,
            const string & nName,
            const string & nSurname);
    CCarList ListCars(const string & name,
            const string & surname) const;
    int CountCars(const string & name,
            const string & surname) const;
    COwnerList ListOwners(const string & RZ) const;
    int CountOwners(const string & RZ) const;
    COwnerList ListOwners(unsigned int vin) const;
    int CountOwners(unsigned int vin) const;

    bool FindByName(const CRecord &, unsigned &) const;
    bool FindByRz(const CRecord &, unsigned &) const;
    bool FindByVin(const CRecord &, unsigned &) const;
    bool FindFirstByName(const CRecord &, unsigned &, unsigned &) const;
    void Realloc(void);
#ifndef __PROGTEST__
    friend ostream& operator<<(ostream&, const CRegister&);
#endif /* __PROGTEST__ */    
protected:
    static const unsigned defaultSize = 10;
    unsigned size;
    CRecord ** byName, ** byRz, ** byVin;
    unsigned records;
};

CRegister::CRegister() {
    size = defaultSize;
    records = 0;
    byName = new CRecord* [size];
    byRz = new CRecord* [size];
    byVin = new CRecord* [size];
}

CRegister::~CRegister() {
    //free allocated memory
    for (unsigned i = 0; i < records; i++) {
        delete byName[i];
    }
    delete[] byName;
    delete[] byRz;
    delete[] byVin;
    byName = NULL;
    byRz = NULL;
    byVin = NULL;
    records = 0;
}

bool CRegister::FindByName(const CRecord & pattern, unsigned & targetIndex) const {
    int from = 0, to = records - 1, middle = 0;
    targetIndex = 0;
    while (from <= to && records) {
        middle = (from + to) / 2;
        if (pattern < *(byName[middle]))
            to = middle - 1;
        else if (pattern > *(byName[middle]))
            from = middle + 1;
        else {
            targetIndex = middle;
            return 1;
        }
    }
    targetIndex = from;
    return 0;
}

bool CRegister::FindFirstByName(const CRecord& tmpRecord, unsigned& firstIndex, unsigned& lastIndex) const {
    unsigned anyIndex;
    if (!FindByName(tmpRecord, anyIndex)) return 0;
    firstIndex = anyIndex;
    while (firstIndex > 0) {
        if (byName[firstIndex]->GetSurname() != byName[firstIndex - 1]->GetSurname() ||
                byName[firstIndex]->GetName() != byName[firstIndex - 1]->GetName()) break;
        firstIndex--;
    }
    lastIndex = anyIndex;
    while (lastIndex < records - 1) {
        if (byName[lastIndex]->GetSurname() != byName[lastIndex + 1]->GetSurname() ||
                byName[lastIndex]->GetName() != byName[lastIndex + 1]->GetName()) break;
        lastIndex++;
    }
    return 1;
}

bool CRegister::FindByRz(const CRecord & pattern, unsigned & targetIndex) const {
    int from = 0, to = records - 1, middle = 0;
    targetIndex = 0;
    while (from <= to && records) {
        middle = (from + to) / 2;
        if (pattern.CompareByRz(*(byRz[middle])) < 0)
            to = middle - 1;
        else if (pattern.CompareByRz(*(byRz[middle])) > 0)
            from = middle + 1;
        else {
            targetIndex = middle;
            return 1;
        }
    }
    targetIndex = from;
    return 0;
}

bool CRegister::FindByVin(const CRecord & pattern, unsigned & targetIndex) const {
    int from = 0, to = records - 1, middle = 0;
    targetIndex = 0;
    while (from <= to && records) {
        middle = (from + to) / 2;
        if (pattern.CompareByVin(*(byRz[middle])) < 0)
            to = middle - 1;
        else if (pattern.CompareByVin(*(byRz[middle])) > 0)
            from = middle + 1;
        else {
            targetIndex = middle;
            return 1;
        }
    }
    targetIndex = from;
    return 0;
}

void CRegister::Realloc(void) {
    if (records > size * 0.9) {
        size *= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byRz, ** tmpByVin;
        byName = new CRecord * [size];
        byRz = new CRecord * [size];
        byVin = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byRz[i] = tmpByAcc[i];
            byVin[i] = tmpByVin[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
        delete[] tmpByVin;
    } else if (size > defaultSize && records < size * 0.4) {
        size /= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byRz, ** tmpByVin;
        byName = new CRecord * [size];
        byRz = new CRecord * [size];
        byVin = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byRz[i] = tmpByAcc[i];
            byVin[i] = tmpByVin[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
        delete[] tmpByVin;
    }
}

bool CRegister::AddCar(const string& rz, unsigned int vin, const string& name, const string& surname) {
    Realloc();
    CRecord tmpRecord(name, surname, rz, vin);
    unsigned newByNameIndex, newByRzIndex, newByVinIndex;
    FindByName(tmpRecord, newByNameIndex);
    if (FindByRz(tmpRecord, newByRzIndex) || FindByVin(tmpRecord, newByVinIndex)) {
        cout << "Unable to add, RECORD ALREADY EXISTS (#" << newByRzIndex << ")" << endl;
        return 0;
    }
    if (name == "" || surname == "" || rz == "") {
        cout << "Unable to add, system doesn't support empty records.";
        return 0;
    }
    //shift indexes
    for (unsigned i = records; i > newByNameIndex; i--) {
        byName[i] = byName[i - 1];
        byName[i]->SetByNameIndex(i);
    }
    for (unsigned i = records; i > newByRzIndex; i--) {
        byRz[i] = byRz[i - 1];
        byRz[i]->SetByRzIndex(i);
    }
    for (unsigned i = records; i > newByVinIndex; i--) {
        byVin[i] = byVin[i - 1];
        byVin[i]->SetByVinIndex(i);
    }
    CRecord * newRecord = new CRecord(tmpRecord);
    newRecord->SetByNameIndex(newByNameIndex);
    newRecord->SetByRzIndex(newByRzIndex);
    newRecord->SetByVinIndex(newByVinIndex);
    byName[newByNameIndex] = newRecord;
    byRz[newByRzIndex] = newRecord;
    byVin[newByVinIndex] = newRecord;
    records++;
    return 1;
}

bool CRegister::DelCar(const string& rz) {
    unsigned delByNameIndex, delByRzIndex, delByVinIndex;
    CRecord delRecord(rz);
    if (!(FindByRz(delRecord, delByRzIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }

    delByNameIndex = byRz[delByRzIndex]->GetByNameIndex();
    delByVinIndex = byRz[delByRzIndex]->GetByVinIndex();
    delete byName[delByNameIndex];
    //shift indexes back to empty field
    for (unsigned i = delByNameIndex; i < records - 1; i++) {
        byName[i] = byName[i + 1];
        byName[i]->SetByNameIndex(i);
    }
    byName[records] = NULL;
    for (unsigned i = delByRzIndex; i < records - 1; i++) {
        byRz[i] = byRz[i + 1];
        byRz[i]->SetByRzIndex(i);
    }
    byVin[records] = NULL;
    for (unsigned i = delByVinIndex; i < records - 1; i++) {
        byVin[i] = byVin[i + 1];
        byVin[i]->SetByVinIndex(i);
    }
    byRz[records] = NULL;

    records--;
    //if less then 40% of memory is used reallocate
    Realloc();
    return 1;
}

bool CRegister::DelCar(unsigned int vin) {
    unsigned delByNameIndex, delByRzIndex, delByVinIndex;
    CRecord delRecord(vin);
    if (!(FindByVin(delRecord, delByVinIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }

    delByNameIndex = byVin[delByVinIndex]->GetByNameIndex();
    delByRzIndex = byRz[delByVinIndex]->GetByRzIndex();
    delete byName[delByNameIndex];
    //shift indexes back to empty field
    for (unsigned i = delByNameIndex; i < records - 1; i++) {
        byName[i] = byName[i + 1];
        byName[i]->SetByNameIndex(i);
    }
    byName[records] = NULL;
    for (unsigned i = delByRzIndex; i < records - 1; i++) {
        byRz[i] = byRz[i + 1];
        byRz[i]->SetByRzIndex(i);
    }
    byRz[records] = NULL;
    for (unsigned i = delByVinIndex; i < records - 1; i++) {
        byVin[i] = byVin[i + 1];
        byVin[i]->SetByVinIndex(i);
    }
    byVin[records] = NULL;

    records--;
    //if less then 40% of memory is used reallocate
    Realloc();
    return 1;
}

bool CRegister::Transfer(const string & rz,
        const string & nName,
        const string & nSurname) {
    unsigned delByRzIndex;
    CRecord delRecord(rz);
    if (!(FindByRz(delRecord, delByRzIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }
    CRecord * toUpdateRecord = byRz[delByRzIndex];    
    
    if (toUpdateRecord->GetName() == nName && toUpdateRecord->GetSurname() == nSurname)
        return 0;
    
    toUpdateRecord->GetOwnerHistory().push_back(CPerson(nName, nSurname));
    toUpdateRecord->SetName(nName);
    toUpdateRecord->SetSurname(nSurname);
    
    for (unsigned i = toUpdateRecord->GetByNameIndex(); i < records - 1; i++) {
        byName[i] = byName[i + 1];
        byName[i]->SetByNameIndex(i);
    }
    
    unsigned newByNameIndex;
    FindByName(*toUpdateRecord, newByNameIndex);
    for (unsigned i = records; i > newByNameIndex; i--) {
        byName[i] = byName[i - 1];
        byName[i]->SetByNameIndex(i);
    }
    
    byName[newByNameIndex] = toUpdateRecord;
    
    return 1;
}

bool CRegister::Transfer(unsigned int vin,
        const string & nName,
        const string & nSurname) {
    unsigned delByVinIndex;
    CRecord delRecord(vin);
    if (!(FindByRz(delRecord, delByVinIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }
    CRecord * toUpdateRecord = byVin[delByVinIndex];    
    
    if (toUpdateRecord->GetName() == nName && toUpdateRecord->GetSurname() == nSurname)
        return 0;
    
    toUpdateRecord->GetOwnerHistory().push_back(CPerson(nName, nSurname));
    toUpdateRecord->SetName(nName);
    toUpdateRecord->SetSurname(nSurname);
    
    for (unsigned i = toUpdateRecord->GetByNameIndex(); i < records - 1; i++) {
        byName[i] = byName[i + 1];
        byName[i]->SetByNameIndex(i);
    }
    
    unsigned newByNameIndex;
    FindByName(*toUpdateRecord, newByNameIndex);
    for (unsigned i = records; i > newByNameIndex; i--) {
        byName[i] = byName[i - 1];
        byName[i]->SetByNameIndex(i);
    }
    
    byName[newByNameIndex] = toUpdateRecord;
    
    return 1;
}

int CRegister::CountOwners(const string& rz) const {
    unsigned byRzIndex;
    if(!FindByRz(CRecord(rz), byRzIndex)) return 0;
    return int(byRz[byRzIndex]->GetOwnerHistory().size());
}

int CRegister::CountOwners(unsigned int vin) const {
    unsigned byVinIndex;
    if(!FindByVin(CRecord(vin), byVinIndex)) return 0;
    return int(byVin[byVinIndex]->GetOwnerHistory().size());
}

COwnerList CRegister::ListOwners(const string& rz) const {
    unsigned rzIndex;
    if(!FindByRz(CRecord(rz), rzIndex)) return NULL;
    
    return COwnerList(byRz[rzIndex]);
}

COwnerList CRegister::ListOwners(unsigned int vin) const {
    unsigned vinIndex;
    if(!FindByVin(CRecord(vin), vinIndex)) return NULL;
    
    return COwnerList(byVin[vinIndex]);
}

int CRegister::CountCars(const string& name, const string& surname) const {
    unsigned first, last;
    CRecord tmpRecord(name, surname);
    if (!FindFirstByName(tmpRecord, first, last)) return 0;
    cout << first << "-" << last << " = " << flush;
    cout << int(last - first + 1) << endl;
    return int(last - first + 1);
}

CCarList CRegister::ListCars(const string& name, const string& surname) const {
    unsigned first, last;
    CRecord tmpRecord(name, surname);
    if (FindFirstByName(tmpRecord, first, last))
        return CCarList(this->byName, first, last);
    return CCarList();
}

/*
 * 
 */
#ifndef __PROGTEST__

ostream& operator<<(ostream& out, const CRecord& record) {
    out << "|" << setw(9) << record.surname
            << "|" << setw(19) << record.name
            << "|" << setw(23) << record.rz
            << "|" << flush;
    return out;
}

ostream& operator<<(ostream& out, const CRegister& database) {
    out << "===============================================================================" << endl;
    out << "|" << setw(3) << "#" << setw(10) << left
            << "|Surname:" << setw(20)
            << "|Name:" << setw(24)
            << "|Rz:" << setw(20)
            << endl;
    out << "===============================================================================" << endl;
    for (unsigned i = 0; i < database.records; i++) {
        out << "|" << right << setw(3) << i << left << *(database.byName[i]) << endl;
        if (i < database.records - 1)
            out << "+---+---------+-------------------+-----------------------+-------------------+" << endl;
    }
    out << "===============================================================================" << endl;
    for (unsigned i = 0; i < database.records; i++) {
        out << "|" << right << setw(3) << i << left << *(database.byRz[i]) << endl;
        if (i < database.records - 1)
            out << "+---+---------+-------------------+-----------------------+-------------------+" << endl;
        else
            out << "===============================================================================" << endl;
    }
    return out;
}

void random_string(string & s) {
    s = "";
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz ";
    unsigned len = (rand() % 10) + 1;
    for (unsigned i = 0; i < len; ++i) {
        s.push_back(alphanum[rand() % (sizeof (alphanum) - 1)]);
    }
}

int main(int argc, char** argv) {
    bool status;
    string acct;
    int sumIncome, sumExpense;

    string name, addr, acc;
    random_string(name);
    random_string(addr);
    random_string(acc);
    //    for (unsigned i = 0; i < 1000; i++) {
    //        if (!(i % 3)) {
    //            random_string(name);
    //            random_string(addr);
    //        }
    //        if (!(i % 4)) {
    //            random_string(acc);
    //        }
    //        cout << "adding (" << name << ", " << addr << ", " << acc << ")" << endl;
    //        b5.Birth(name, addr, acc);
    //        ofstream output("database.out", ofstream::out);
    //        if (output.fail() || !output) {
    //            return 0;
    //        }
    //        output << b5 << endl;
    //        output.close();
    //
    //        if (!(i % 30)) {
    //            cout << "deleting (" << name << ", " << addr << ")" << endl;
    //            b5.Death(name, addr);
    //            ofstream output("database.out", ofstream::out);
    //            if (output.fail() || !output) {
    //                return 0;
    //            }
    //            output << b5 << endl;
    //            output.close();
    //        }
    //    }

    CRegister b1;
    assert(b1 . AddCar("ABC-12-34", 1000, "John", "Smith") == true);
    assert(b1 . AddCar("ABC-32-22", 2000, "John", "Hacker") == true);
    assert(b1 . AddCar("XYZ-11-22", 10, "Peter", "Smith") == true);
    assert(b1 . CountCars("John", "Hacker") == 1);
    for (CCarList l = b1 . ListCars("John", "Hacker"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // the following license plate + VIN

    // ABC-32-22, 2000

    assert(b1 . CountOwners("ABC-12-34") == 1);
    for (COwnerList l = b1 . ListOwners("ABC-12-34"); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 1 owners in that order:

    // Smith, John

    assert(b1 . CountOwners(10) == 1);
    for (COwnerList l = b1 . ListOwners(10); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 1 owners in that order:

    // Smith, Peter

    assert(b1 . Transfer("XYZ-11-22", "John", "Hacker") == true);
    assert(b1 . Transfer(10, "Will", "Smith") == true);
    assert(b1 . Transfer("XYZ-11-22", "John", "Hacker") == true);
    assert(b1 . AddCar("XYZ-99-88", 20, "John", "Smith") == true);
    assert(b1 . CountCars("John", "Smith") == 2);
    for (CCarList l = b1 . ListCars("John", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // the following 2 license plates + VINs, in any order:

    // ABC-12-34, 1000
    // XYZ-99-88, 20

    assert(b1 . CountCars("John", "Hacker") == 2);
    for (CCarList l = b1 . ListCars("John", "Hacker"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // the following 2 license plates + VINs, in any order:

    // ABC-32-22, 2000
    // XYZ-11-22, 10

    assert(b1 . CountCars("Peter", "Smith") == 0);
    for (CCarList l = b1 . ListCars("Peter", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // empty output

    assert(b1 . CountOwners(10) == 4);
    for (COwnerList l = b1 . ListOwners(10); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 4 owners in that order:

    // Hacker, John
    // Smith, Will
    // Hacker, John
    // Smith, Peter

    assert(b1 . CountOwners("XYZ-11-22") == 4);
    for (COwnerList l = b1 . ListOwners("XYZ-11-22"); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 4 owners in that order:

    // Hacker, John
    // Smith, Will
    // Hacker, John
    // Smith, Peter

    assert(b1 . Transfer("XYZ-11-22", "Jane", "Black") == true);
    assert(b1 . CountCars("Jane", "Black") == 1);
    for (CCarList l = b1 . ListCars("Jane", "Black"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // the following license plate + VIN

    // XYZ-11-22, 10

    assert(b1 . CountCars("John", "Smith") == 2);
    for (CCarList l = b1 . ListCars("John", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // the following 2 license plates + VINs, in any order:

    // ABC-12-34, 1000
    // XYZ-99-88, 20

    assert(b1 . CountOwners(10) == 5);
    for (COwnerList l = b1 . ListOwners(10); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 5 owners in that order:

    // Black, Jane
    // Hacker, John
    // Smith, Will
    // Hacker, John
    // Smith, Peter

    assert(b1 . CountOwners("XYZ-11-22") == 5);
    for (COwnerList l = b1 . ListOwners("XYZ-11-22"); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 5 owners in that order:

    // Black, Jane
    // Hacker, John
    // Smith, Will
    // Hacker, John
    // Smith, Peter

    assert(b1 . DelCar("XYZ-11-22") == true);
    assert(b1 . DelCar(1000) == true);
    assert(b1 . CountCars("Jane", "Black") == 0);
    for (CCarList l = b1 . ListCars("Jane", "Black"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // empty output

    assert(b1 . AddCar("XYZ-11-22", 30, "George", "White") == true);
    assert(b1 . CountOwners(30) == 1);
    for (COwnerList l = b1 . ListOwners(30); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 1 owners in that order:

    // White, George

    assert(b1 . CountOwners("XYZ-11-22") == 1);
    for (COwnerList l = b1 . ListOwners("XYZ-11-22"); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 1 owners in that order:

    // White, George


    CRegister b2;
    assert(b2 . AddCar("ABC-12-34", 10, "John", "Smith") == true);
    assert(b2 . AddCar("ABC-32-22", 20, "John", "Hacker") == true);
    assert(b2 . AddCar("XYZ-11-22", 30, "Peter", "Smith") == true);
    assert(b2 . AddCar("XYZ-11-22", 30, "Jane", "Black") == false);
    assert(b2 . AddCar("XYZ-11-22", 50, "Jane", "Black") == false);
    assert(b2 . AddCar("ZZZ-11-22", 30, "Jane", "Black") == false);
    assert(b2 . DelCar("AAA-11-11") == false);
    assert(b2 . DelCar(9999) == false);
    assert(b2 . DelCar("ABC-32-22") == true);
    assert(b2 . DelCar(20) == false);
    assert(b2 . DelCar(30) == true);
    assert(b2 . DelCar("XYZ-11-22") == false);
    assert(b2 . Transfer("BBB-99-99", "John", "Smith") == false);
    assert(b2 . Transfer("ABC-12-34", "John", "Smith") == false);
    assert(b2 . Transfer(9999, "John", "Smith") == false);
    assert(b2 . Transfer(10, "John", "Smith") == false);
    assert(b2 . CountCars("George", "White") == 0);
    for (CCarList l = b2 . ListCars("George", "White"); !l . AtEnd(); l . Next())
        cout << l . RZ() << ", " << l . VIN() << endl;
    // empty output

    assert(b2 . CountOwners("AAA-AA-AA") == 0);
    for (COwnerList l = b2 . ListOwners("AAA-AA-AA"); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 0 owners in that order:


    assert(b2 . CountOwners(9999) == 0);
    for (COwnerList l = b2 . ListOwners(9999); !l . AtEnd(); l . Next())
        cout << l . Surname() << ", " << l . Name() << endl;
    // the following 0 owners in that order:



    return 0;
}
#endif /*__PROGTEST__*/