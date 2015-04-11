/* 
 * File:   main.cpp
 * Author: cuhellub
 *
 * Created on April 11, 2015, 9:54 AM
 */

#ifndef __PROGTEST__
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
    bool operator<(const CRecord&) const;
    bool operator>(const CRecord&) const;
    int CompareByAcc(const CRecord&) const;
    bool HasAcc(void) const;
    void Income(int amount);
    void Expense(int amount);
    int GetIncome(void) const;
    int GetExpense(void) const;
    string GetAcc(void) const;
    #ifndef __PROGTEST__
    friend ostream& operator <<(ostream&, const CRecord&);
    #endif /* __PROGTEST__ */
private:
    string name, address, account;
    int sumIncome, sumExpense;
};

CRecord::CRecord(string name, string address, string account) {
    this->name = name;
    this->address = address;
    this->account = account;
    this->sumIncome = 0;
    this->sumExpense = 0;
}

CRecord::CRecord(string name, string address) {
    this->name = name;
    this->address = address;
    this->account = "";
    this->sumIncome = 0;
    this->sumExpense = 0;
}

bool CRecord::operator<(const CRecord& x) const {
    return (this->name < x.name
            || (this->name == x.name
            && this->address < x.address)
            );
}

bool CRecord::operator>(const CRecord& x) const {
    return (this->name > x.name
            || (this->name == x.name
            && this->address > x.address)
            );
}

int CRecord::CompareByAcc(const CRecord& x) const {
    if (this->account < x.account) return -1;
    if (this->account > x.account) return 1;
    return 0;
}

bool CRecord::HasAcc() const {
    if (this->account == "")
        return 0;
    return 1;
}

void CRecord::Income(int amount) {
    sumIncome += amount;
}

void CRecord::Expense(int amount) {
    sumExpense += amount;
}

int CRecord::GetIncome() const {
    return sumIncome;
}

int CRecord::GetExpense() const {
    return sumExpense;
}

string CRecord::GetAcc() const {
    return account;
}

class CTaxRegister {
public:
    CTaxRegister(void);
    ~CTaxRegister(void);
    bool Birth(const string & name,
            const string & addr,
            const string & account);
    bool Birth(const string & name,
            const string & addr);
    bool Death(const string & name,
            const string & addr);
    bool Income(const string & account,
            int amount);
    bool Income(const string & name,
            const string & addr,
            int amount);
    bool Expense(const string & account,
            int amount);
    bool Expense(const string & name,
            const string & addr,
            int amount);
    bool Audit(const string & name,
            const string & addr,
            string & account,
            int & sumIncome,
            int & sumExpense) const;
    bool Audit(const string & name,
            const string & addr,
            int & sumIncome,
            int & sumExpense) const;
    bool FindByName(const CRecord &, unsigned &) const;
    bool FindByAcc(const CRecord &, unsigned &) const;
    void Realloc(void);
    #ifndef __PROGTEST__
    friend ostream& operator <<(ostream&, const CTaxRegister&);
    #endif /* __PROGTEST__ */    
protected:
    static const unsigned defaultSize = 10;
    unsigned size;
    CRecord ** byName, ** byAcc;
    unsigned records;
};

CTaxRegister::CTaxRegister() {
    size = defaultSize;
    records = 0;
    byName = new CRecord* [size];
    byAcc = new CRecord* [size];
}

CTaxRegister::~CTaxRegister() {
    //free allocated memory
    for (unsigned i = 0; i < records; i++) {
        delete byName[i];
    }
    delete[] byName;
    delete[] byAcc;
    byName = NULL;
    byAcc = NULL;
    records = 0;
}

bool CTaxRegister::FindByName(const CRecord & pattern, unsigned & targetIndex) const {
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

bool CTaxRegister::FindByAcc(const CRecord & pattern, unsigned & targetIndex) const {
    int from = 0, to = records - 1, middle = 0;
    targetIndex = 0;
    while (from <= to && records) {
        middle = (from + to) / 2;
        if (pattern.CompareByAcc(*(byAcc[middle])) < 0)
            to = middle - 1;
        else if (pattern.CompareByAcc(*(byAcc[middle])) > 0)
            from = middle + 1;
        else {
            targetIndex = middle;
            return 1;
        }
    }
    targetIndex = from;
    return 0;
}

void CTaxRegister::Realloc(void) {
    if (records > size * 0.9) {
        size *= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byAcc;
        byName = new CRecord * [size];
        byAcc = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byAcc[i] = tmpByAcc[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
    } else if (size > defaultSize && records < size * 0.4) {
        size /= 2;
        CRecord ** tmpByName = byName, ** tmpByAcc = byAcc;
        byName = new CRecord * [size];
        byAcc = new CRecord * [size];
        for (unsigned i = 0; i < records; i++) {
            byName[i] = tmpByName[i];
            byAcc[i] = tmpByAcc[i];
        }
        delete[] tmpByName;
        delete[] tmpByAcc;
    }
}

bool CTaxRegister::Birth(const string& name, const string& address, const string& account) {
    Realloc();
    CRecord tmpRecord(name, address, account);
    unsigned newByNameIndex, newByAccIndex;
    if (FindByName(tmpRecord, newByNameIndex) || FindByAcc(tmpRecord, newByAccIndex)) {
        cout << "Unable to add, RECORD ALREADY EXISTS (#" << newByNameIndex << ")" << endl;
        return 0;
    }
    if (name == "" || address == "") {
        cout << "Unable to add, system doesn't support empty records.";
        return 0;
    }
    //shift indexes
    for (unsigned i = records; i > newByNameIndex; i--) {
        byName[i] = byName[i - 1];
    }
    for (unsigned i = records; i > newByAccIndex; i--) {
        byAcc[i] = byAcc[i - 1];
    }
    CRecord * newRecord = new CRecord(tmpRecord);
    byName[newByNameIndex] = newRecord;
    byAcc[newByAccIndex] = newRecord;
    records++;
    return 1;
}

bool CTaxRegister::Birth(const string& name, const string& address) {
    Realloc();
    CRecord tmpRecord(name, address);
    unsigned newByNameIndex;
    if (FindByName(tmpRecord, newByNameIndex)) {
        cout << "Unable to add, RECORD ALREADY EXISTS (#" << newByNameIndex << ")" << endl;
        return 0;
    }
    if (name == "" || address == "") {
        cout << "Unable to add, system doesn't support empty records.";
        return 0;
    }
    //shift indexes
    for (unsigned i = records; i > newByNameIndex; i--) {
        byName[i] = byName[i - 1];
    }
    CRecord * newRecord = new CRecord(tmpRecord);
    byName[newByNameIndex] = newRecord;
    records++;
    return 1;
}

bool CTaxRegister::Death(const string& name, const string& address) {
    unsigned delByNameIndex, delByAccIndex;
    CRecord delRecord(name, address);
    if (!(FindByName(delRecord, delByNameIndex))) {
        cout << "Unable to delete, RECORD DOESN'T EXIST" << endl;
        return 0;
    }
    bool hasAcc = byName[delByNameIndex]->HasAcc();
    if (hasAcc)
        FindByAcc(*(byName[delByNameIndex]), delByAccIndex);
    delete byName[delByNameIndex];
    //shift indexes back to empty field
    for (unsigned i = delByNameIndex; i < records; i++) {
        byName[i] = byName[i + 1];
    }
    byName[records] = NULL;
    if (hasAcc) {
        for (unsigned i = delByAccIndex; i < records; i++) {
            byAcc[i] = byAcc[i + 1];
        }
        byAcc[records] = NULL;
    }
    records--;
    //if less then 40% of memory is used reallocate
    Realloc();
    return 1;
}

bool CTaxRegister::Income(const string & account, int amount) {
    CRecord tmp("", "", account);
    unsigned targetIndex;
    if (FindByAcc(tmp, targetIndex)) {
        byAcc[targetIndex]->Income(amount);
        return 1;
    }
    return 0;
}

bool CTaxRegister::Income(const string & name, const string & addr, int amount) {
    CRecord tmp(name, addr, "");
    unsigned targetIndex;
    if (FindByName(tmp, targetIndex)) {
        byName[targetIndex]->Income(amount);
        return 1;
    }
    return 0;
}

bool CTaxRegister::Expense(const string & account, int amount) {
    CRecord tmp("", "", account);
    unsigned targetIndex;
    if (FindByAcc(tmp, targetIndex)) {
        byAcc[targetIndex]->Expense(amount);
        return 1;
    }
    return 0;
}

bool CTaxRegister::Expense(const string & name, const string & addr, int amount) {
    CRecord tmp(name, addr, "");
    unsigned targetIndex;
    if (FindByName(tmp, targetIndex)) {
        byName[targetIndex]->Expense(amount);
        return 1;
    }
    return 0;
}

bool CTaxRegister::Audit(const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense) const {
    CRecord tmp(name, addr, "");
    unsigned targetIndex;
    if (FindByName(tmp, targetIndex)) {
        account = byName[targetIndex]->GetAcc();
        sumIncome = byName[targetIndex]->GetIncome();
        sumExpense = byName[targetIndex]->GetExpense();
        return 1;
    }
    return 0;
}

bool CTaxRegister::Audit(const string & name, const string & addr, int & sumIncome, int & sumExpense) const {
    CRecord tmp(name, addr, "");
    unsigned targetIndex;
    if (FindByName(tmp, targetIndex)) {
        sumIncome = byName[targetIndex]->GetIncome();
        sumExpense = byName[targetIndex]->GetExpense();
        return 1;
    }
    return 0;
}

/*
 * 
 */
#ifndef __PROGTEST__

ostream& operator <<(ostream& out, const CRecord& record) {
    out << "|" << setw(9) << record.name
            << "|" << setw(19) << record.address
            << "|" << setw(23) << record.account
            << "|" << setw(19) << record.sumExpense
            << "|" << flush;
    return out;
}

ostream& operator <<(ostream& out, const CTaxRegister& database) {
    out << "===============================================================================" << endl;
    out << "|" << setw(3) << "#" << setw(10) << left
            << "|Name:" << setw(20)
            << "|Addr:" << setw(24)
            << "|Account:" << setw(20)
            << "|Expense:"
            << "|" << endl;
    out << "===============================================================================" << endl;
    for (unsigned i = 0; i < database.records; i++) {
        out << "|" << right << setw(3) << i << left << *(database.byName[i]) << endl;
        if (i < database.records - 1)
            out << "+---+---------+-------------------+-----------------------+-------------------+" << endl;
    }
    out << "===============================================================================" << endl;
    for (unsigned i = 0; i < database.records; i++) {
        out << "|" << right << setw(3) << i << left << *(database.byAcc[i]) << endl;
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

    CTaxRegister b1, b2, b3, b4, b5;
    
//    status = b1 . Birth("John Smith", "Oak Road 23");
//    // status = true
//    status = b1 . Birth("Jane Hacker", "Main Street 17");
//    // status = true
//    status = b1 . Birth("Peter Hacker", "Main Street 17");
//    // status = true
//    status = b1 . Birth("John Smith", "Main Street 17");
//    // status = true
//    status = b1 . Income("Peter Hacker", "Main Street 17", 2000);
//    // status = true
//    status = b1 . Income("Jane Hacker", "Main Street 17", 3000);
//    // status = true
//    status = b1 . Income("Peter Hacker", "Main Street 17", 4000);
//    // status = true
//    status = b1 . Expense("Jane Hacker", "Main Street 17", 2000);
//    // status = true
//    status = b1 . Expense("John Smith", "Main Street 17", 500);
//    // status = true
//    status = b1 . Expense("Jane Hacker", "Main Street 17", 100);
//    // status = true
//    status = b1 . Audit("John Smith", "Oak Road 23", sumIncome, sumExpense);
//    // status = true, sumIncome=0, sumExpense=0
//    status = b1 . Audit("Jane Hacker", "Main Street 17", sumIncome, sumExpense);
//    // status = true, sumIncome=3000, sumExpense=2100
//    status = b1 . Audit("Peter Hacker", "Main Street 17", sumIncome, sumExpense);
//    // status = true, sumIncome=6000, sumExpense=0
//    status = b1 . Audit("John Smith", "Main Street 17", sumIncome, sumExpense);
//    // status = true, sumIncome=0, sumExpense=500
//    status = b1 . Death("John Smith", "Main Street 17");
//    // status = true
//
//
//    status = b2 . Birth("John Smith", "Oak Road 23");
//    // status = true
//    status = b2 . Birth("Jane Hacker", "Main Street 17");
//    // status = true
//    status = b2 . Income("Jane Hacker", "Oak Road 10", 4000);
//    // status = false
//    status = b2 . Expense("John Smith", "Main Street 18", 500);
//    // status = false
//    status = b2 . Audit("John Nowak", "Second Street 23", sumIncome, sumExpense);
//    // status = false
//    status = b2 . Death("Peter Nowak", "5-th Avenue");
//    // status = false
//    status = b2 . Birth("Jane Hacker", "Main Street 17");
//    // status = false
//    status = b2 . Birth("Joe Hacker", "Elm Street 23");
//    // status = true
//    status = b2 . Birth("Joe Hacker", "Elm Street 23");
//    // status = false
//    status = b2 . Death("Jane Hacker", "Main Street 17");
//    // status = true
//    status = b2 . Audit("Jane Hacker", "Main Street 17", sumIncome, sumExpense);
//    // status = false
//    status = b2 . Birth("Jane Hacker", "Main Street 17");
//    // status = true
//
//
//    status = b3 . Birth("John Smith", "Oak Road 23", "123/456/789");
//    // status = true
//    status = b3 . Birth("Jane Hacker", "Main Street 17", "Xuj5#94");
//    // status = true
//    status = b3 . Birth("Peter Hacker", "Main Street 17", "634oddT");
//    // status = true
//    status = b3 . Birth("John Smith", "Main Street 17", "Z343rwZ");
//    // status = true
//    status = b3 . Income("Xuj5#94", 1000);
//    // status = true
//    status = b3 . Income("634oddT", 2000);
//    // status = true
//    status = b3 . Income("123/456/789", 3000);
//    // status = true
//    status = b3 . Income("634oddT", 4000);
//    // status = true
//    status = b3 . Income("Peter Hacker", "Main Street 17", 2000);
//    // status = true
//    status = b3 . Expense("Jane Hacker", "Main Street 17", 2000);
//    // status = true
//    status = b3 . Expense("John Smith", "Main Street 17", 500);
//    // status = true
//    status = b3 . Expense("Jane Hacker", "Main Street 17", 1000);
//    // status = true
//    status = b3 . Expense("Xuj5#94", 1300);
//    // status = true
//    status = b3 . Audit("John Smith", "Oak Road 23", acct, sumIncome, sumExpense);
//    // status = true, acct = "123/456/789", sumIncome=3000, sumExpense=0
//    status = b3 . Audit("Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense);
//    // status = true, acct = "Xuj5#94", sumIncome=1000, sumExpense=4300
//    status = b3 . Audit("Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense);
//    // status = true, acct = "634oddT", sumIncome=8000, sumExpense=0
//    status = b3 . Audit("John Smith", "Main Street 17", acct, sumIncome, sumExpense);
//    // status = true, acct = "Z343rwZ", sumIncome=0, sumExpense=500
//    status = b3 . Death("John Smith", "Main Street 17");
//    // status = true
//
//
//    status = b4 . Birth("John Smith", "Oak Road 23", "123/456/789");
//    // status = true
//    status = b4 . Birth("Jane Hacker", "Main Street 17", "Xuj5#94");
//    // status = true
//    status = b4 . Income("634oddT", 4000);
//    // status = false
//    status = b4 . Expense("John Smith", "Main Street 18", 500);
//    // status = false
//    status = b4 . Audit("John Nowak", "Second Street 23", acct, sumIncome, sumExpense);
//    // status = false
//    status = b4 . Death("Peter Nowak", "5-th Avenue");
//    // status = false
//    status = b4 . Birth("Jane Hacker", "Main Street 17", "4et689A");
//    // status = false
//    status = b4 . Birth("Joe Hacker", "Elm Street 23", "Xuj5#94");
//    // status = false
//    status = b4 . Death("Jane Hacker", "Main Street 17");
//    // status = true
//    status = b4 . Birth("Joe Hacker", "Elm Street 23", "Xuj5#94");
//    // status = true
//    status = b4 . Audit("Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense);
//    // status = true, acct = "Xuj5#94", sumIncome=0, sumExpense=0
//    status = b4 . Birth("Joe Hacker", "Elm Street 23", "AAj5#94");
//    // status = false

    string name, addr, acc;
    random_string(name);
    random_string(addr);
    random_string(acc);
    for (unsigned i = 0; i < 1000; i++) {
        if (!(i % 3)) {
            random_string(name);
            random_string(addr);
        }
        if (!(i % 4)) {
            random_string(acc);
        }
        cout << "adding (" << name << ", " << addr << ", " << acc << ")" << endl;
        b5.Birth(name, addr, acc);
        ofstream output("database.out", ofstream::out);
        if (output.fail() || !output) {
            return 0;
        }
        output << b5 << endl;
        output.close();

        if (!(i % 30)) {
            cout << "deleting (" << name << ", " << addr << ")" << endl;
            b5.Death(name, addr);
            ofstream output("database.out", ofstream::out);
            if (output.fail() || !output) {
                return 0;
            }
            output << b5 << endl;
            output.close();
        }
    }
    return 0;
}
#endif /*__PROGTEST__*/