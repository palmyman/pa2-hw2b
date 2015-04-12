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

class CRecord {
public:
    CRecord(string, string, string);
    CRecord(string, string);
    CRecord(string);
    bool operator<(const CRecord&) const;
    bool operator>(const CRecord&) const;
    int CompareByRz(const CRecord&) const;
    string GetName(void) const;
    string GetSurName(void) const;
    string GetRz(void) const;
    unsigned GetByNameIndex() const;
    void SetByNameIndex(unsigned byNameIndex);
#ifndef __PROGTEST__
    friend ostream& operator<<(ostream&, const CRecord&);
#endif /* __PROGTEST__ */
private:
    string name, surname, rz;
    unsigned vin, byNameIndex;
};

CRecord::CRecord(string name, string surname, string rz) {
    this->name = name;
    this->surname = surname;
    this->rz = rz;
    this->vin = 0;
    this->byNameIndex = 0;
}

CRecord::CRecord(string name, string surname) {
    this->name = name;
    this->surname = surname;
    this->rz = "";
    this->vin = 0;
    this->byNameIndex = 0;
}

CRecord::CRecord(string rz) {
    this->name = "";
    this->surname = "";
    this->rz = rz;
    this->vin = 0;
    this->byNameIndex = 0;
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

string CRecord::GetName() const {
    return this->name;
}

string CRecord::GetSurName() const {
    return this->surname;
}

string CRecord::GetRz() const {
    return this->rz;
}

unsigned CRecord::GetByNameIndex(void) const {
    return this->byNameIndex;
}

void CRecord::SetByNameIndex(unsigned byNameIndex) {
    this->byNameIndex = byNameIndex;
}

class CCarList {
public:
    CCarList(CRecord ** byName, unsigned firstIndex, unsigned lastIndex);
    CCarList(void);
    string RZ(void) const;
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

bool CCarList::AtEnd() const {
    if(empty) return true;
    return (firstIndex == lastIndex + 1);
}

void CCarList::Next() {
    firstIndex++;
}

class CRegister {
public:
    CRegister(void);
    ~CRegister(void);
    bool AddCar(const string & rz,
            const string & name,
            const string & surname);
    bool DelCar(const string & rz);
    bool Transfer(const string & rz,
            const string & nName,
            const string & nSurname);
    CCarList ListCars(const string & name,
            const string & surname) const;
    int CountCars(const string & name,
            const string & surname) const;
    bool FindByName(const CRecord &, unsigned &) const;
    bool FindByRz(const CRecord &, unsigned &) const;
    bool FindFirstByName(const CRecord &, unsigned &, unsigned &) const;
    void Realloc(void);
#ifndef __PROGTEST__
    friend ostream& operator<<(ostream&, const CRegister&);
#endif /* __PROGTEST__ */    
protected:
    static const unsigned defaultSize = 10;
    unsigned size;
    CRecord ** byName, ** byRz;
    unsigned records;
};

CRegister::CRegister() {
    size = defaultSize;
    records = 0;
    byName = new CRecord* [size];
    byRz = new CRecord* [size];
}

CRegister::~CRegister() {
    //free allocated memory
    for (unsigned i = 0; i < records; i++) {
        delete byName[i];
    }
    delete[] byName;
    delete[] byRz;
    byName = NULL;
    byRz = NULL;
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
        if (byName[firstIndex]->GetSurName() != byName[firstIndex - 1]->GetSurName() ||
                byName[firstIndex]->GetName() != byName[firstIndex - 1]->GetName()) break;
        firstIndex--;
    }
    lastIndex = anyIndex;
    while (lastIndex < records - 1) {
        if (byName[lastIndex]->GetSurName() != byName[lastIndex + 1]->GetSurName() ||
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

void CRegister::Realloc(void) {
    if (records > size * 0.9) {
        size *= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byRz;
        byName = new CRecord * [size];
        byRz = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byRz[i] = tmpByAcc[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
    } else if (size > defaultSize && records < size * 0.4) {
        size /= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byRz;
        byName = new CRecord * [size];
        byRz = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byRz[i] = tmpByAcc[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
    }
}

bool CRegister::AddCar(const string& rz, const string& name, const string& surname) {
    Realloc();
    CRecord tmpRecord(name, surname, rz);
    unsigned newByNameIndex, newByRzIndex;
    FindByName(tmpRecord, newByNameIndex);
    if (FindByRz(tmpRecord, newByRzIndex)) {
        cout << "Unable to add, RECORD ALREADY EXISTS (#" << newByRzIndex << ")" << endl;
        return 0;
    }
    if (name == "" || surname == "") {
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
    }
    CRecord * newRecord = new CRecord(tmpRecord);
    newRecord->SetByNameIndex(newByNameIndex);
    byName[newByNameIndex] = newRecord;
    byRz[newByRzIndex] = newRecord;
    records++;
    return 1;
}

bool CRegister::DelCar(const string& rz) {
    unsigned delByNameIndex, delByRzIndex;
    CRecord delRecord(rz);
    if (!(FindByRz(delRecord, delByRzIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }

    delByNameIndex = byRz[delByRzIndex]->GetByNameIndex();
    delete byName[delByNameIndex];
    //shift indexes back to empty field
    for (unsigned i = delByNameIndex; i < records - 1; i++) {
        byName[i] = byName[i + 1];
        byName[i]->SetByNameIndex(i);
    }
    byName[records] = NULL;
    for (unsigned i = delByRzIndex; i < records - 1; i++) {
        byRz[i] = byRz[i + 1];
    }
    byRz[records] = NULL;

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
    CRecord * toDeleteRecord = byRz[delByRzIndex];
    if (toDeleteRecord->GetName() == nName && toDeleteRecord->GetSurName() == nSurname)
        return 0;
    return DelCar(rz) && AddCar(rz, nName, nSurname);
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
    assert(b1 . AddCar("ABC-12-34", "John", "Smith") == true);
    assert(b1 . AddCar("ABC-32-22", "John", "Hacker") == true);
    assert(b1 . AddCar("XYZ-11-22", "Peter", "Smith") == true);
    assert(b1 . CountCars("John", "Hacker") == 1);
    for (CCarList l = b1 . ListCars("John", "Hacker"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // the following plate

    // ABC-32-22
    
    cout << b1 << endl;

    assert(b1 . Transfer("XYZ-11-22", "John", "Hacker") == true);
    assert(b1 . AddCar("XYZ-99-88", "John", "Smith") == true);
    assert(b1 . CountCars("John", "Smith") == 2);
    for (CCarList l = b1 . ListCars("John", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // the following 2 licence plates, in any order:

    // ABC-12-34
    // XYZ-99-88
    
    cout << b1 << endl;

    assert(b1 . CountCars("John", "Hacker") == 2);
    for (CCarList l = b1 . ListCars("John", "Hacker"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // the following 2 licence plates, in any order:

    // ABC-32-22
    // XYZ-11-22
    
    cout << b1 << endl;

    assert(b1 . CountCars("Peter", "Smith") == 0);
    for (CCarList l = b1 . ListCars("Peter", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // empty output
    
    cout << b1 << endl;

    assert(b1 . Transfer("XYZ-11-22", "Jane", "Black") == true);
    assert(b1 . CountCars("Jane", "Black") == 1);
    for (CCarList l = b1 . ListCars("Jane", "Black"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // the following plate

    // XYZ-11-22
    
    cout << b1 << endl;

    assert(b1 . CountCars("John", "Smith") == 2);
    for (CCarList l = b1 . ListCars("John", "Smith"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // the following 2 licence plates, in any order:

    // ABC-12-34
    // XYZ-99-88
    
    cout << b1 << endl;

    assert(b1 . DelCar("XYZ-11-22") == true);
    assert(b1 . CountCars("Jane", "Black") == 0);
    for (CCarList l = b1 . ListCars("Jane", "Black"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // empty output
    
    cout << b1 << endl;

    assert(b1 . AddCar("XYZ-11-22", "George", "White") == true);

    CRegister b2;
    assert(b2 . AddCar("ABC-12-34", "John", "Smith") == true);
    assert(b2 . AddCar("ABC-32-22", "John", "Hacker") == true);
    assert(b2 . AddCar("XYZ-11-22", "Peter", "Smith") == true);
    assert(b2 . AddCar("XYZ-11-22", "Jane", "Black") == false);
    assert(b2 . DelCar("AAA-11-11") == false);
    assert(b2 . Transfer("BBB-99-99", "John", "Smith") == false);
    assert(b2 . Transfer("ABC-12-34", "John", "Smith") == false);
    assert(b2 . CountCars("George", "White") == 0);
    for (CCarList l = b2 . ListCars("George", "White"); !l . AtEnd(); l . Next())
        cout << l . RZ() << endl;
    // empty output
    return 0;
}
#endif /*__PROGTEST__*/