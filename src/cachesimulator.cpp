#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

void display(vector<string> );
int bin2dec(string );
string hex2bin(string );

struct Address{
    string index;
    string offset;
    string tag;

    void update(string address, int indexbits, int offsetbits){         // automatically assigns class members
        
        if(offsetbits == 0){
            this->offset = "";
        }else{
            this->offset = address.substr(address.size()-offsetbits, offsetbits);
        }

        this->index = address.substr(address.size()-offsetbits-indexbits, indexbits);

        this->tag = address.substr(0, address.size()-offsetbits-indexbits);
    }

    int position(){
        int pos = bin2dec(this->index);
        return pos;
    }

};

int main(int argc, char *argv[]){

    if(argc < 2 || argc > 4){           // basic command line error checking
        cerr << "Usage: " << argv[0] << " <filename> [block size] [number of blocks]\n";
        return -1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open()) {               // file error checking
        cout << "Could not open file " << argv[1] << endl;
    }

    int blocksize, blocknum;            // block size and number of blocks
    if(argc == 3){                      // user set block size but not number of blocks
        istringstream sin(argv[2]);
        if (!(sin >> blocksize)) {
            cerr << "Invalid number: " << argv[2] << '\n';
            return -1;
        }
        
        blocknum = 4;                   // default value of number of blocks
    }
    else if(argc == 4){                 // user set both block size and number of blocks
        istringstream sin(argv[2]);
        if (!(sin >> blocksize)) {
            cerr << "Invalid number: " << argv[2] << '\n';
            return -1;
        }
        sin.clear();
        
        sin.str(argv[3]);
        if (!(sin >> blocknum)) {
            cerr << "Invalid number: " << argv[3] << '\n';
            return -1;
        }
        sin.clear();
    }
    else if(argc == 2){                 // user did not set neither block size nor number of blocks
        blocksize = 32;
        blocknum = 4;
    }

    if(blocknum < 1){                   // makes sure number of block is not undefined
        cerr << "Invalid number: " << argv[2] << '\n';
        return -1;
    }
    if(blocksize < 0){                  // make sures block size is not undefined
        cerr << "Invalid number: " << argv[3] << '\n';
        return -1;
    }

    string line;
    int maxlength = 0;
    while(getline(fin, line)){          // gets largest word address size
        if(line.size() > maxlength){
            maxlength = line.size();
        }
    }
    fin.close();

    int num_indexbits = log10(blocknum)/log10(2);
    int num_offsetbits;
    if(blocksize == 0){                 // if block size is not considered, then there are no offset bits
        num_offsetbits = 0;
    }else{
        num_offsetbits = log10(blocksize)/log10(2);
    }

    vector<string> cache;     // cache
    for(int i = 0; i < pow(2, num_indexbits); i++){     // allocates space for cache
        cache.push_back("");
    }

    float hits = 0;
    float misses = 0;
    string hex;
    string curr_address;
    Address address;

    string process;
    bool work = false;
    
    cout << "Show process? (y/n) ";
    cin >> process;
    if(process == "y" || process == "yes" || process == "Y" || process == "YES"){
        work = true;
    }

    fin.open(argv[1]);
    while(getline(fin, hex)){                           // analyzes each hex value
        if(hex.size() < maxlength){                     // inserts 0's so all tags are same number of bits
            hex.insert(2, maxlength-hex.size(), '0');
        }

        curr_address = hex2bin(hex);            // converts hex number to binary

        address.update(curr_address, num_indexbits, num_offsetbits);        // assigns address values (tag, index, offset, etc.)

        if(cache[address.position()] == ""){        // makes sure that if cache block is empty, its labeled as a miss
            misses++;
            if(work){
                cout << address.tag << ' ' << address.index << ' ' << address.offset << ": miss" << endl;
            }
        }else if(bin2dec(address.tag) == bin2dec(cache[address.position()])){ 
            hits++;
            if(work){
                cout << address.tag << ' ' << address.index << ' ' << address.offset << ": hit" << endl;
            }
        }else{
            misses++;
            if(work){
                cout << address.tag << ' ' << address.index << ' ' << address.offset << ": miss" << endl;
            }
        }       

        cache[address.position()] = address.tag;            // assigns the tag to the current cache index
    }
    fin.close();

    display(cache);             // displays cache to console as a table

    float hitrate = hits / (hits+misses);           // calculates hit rate
    float missrate = misses / (hits+misses);        // calculates miss rate
    float AMAT = hitrate*1+missrate*190;            // calculates AMAT using i7 core processor values

    cout << "Hit rate: " << setprecision(5) << hitrate*100 << "%\n";
    cout << "Miss rate: " << missrate * 100 << "%\n";
    cout << "AMAT: " << AMAT << " cycles or " << AMAT * 0.5263157895 << " ns";

    return 0;
}

void display(vector<string> cache){
    cout << endl << "Cache: " << endl;
    int maxsize = 0;
    for(int i = 0; i < cache.size(); i++){
        if(cache[i].size() > maxsize){
            maxsize = cache[i].size();
        }
    }

    cout << '|';
    for(int i = 0; i < maxsize; i++){
        cout << "-";
    }
    cout << '|' << endl;

    for(int i = 0; i < cache.size(); i++){
        cout << '|' << setw(maxsize) << cache[i] << '|' << endl;
        cout << '|';
        for(int i = 0; i < maxsize; i++){
            cout << "-";
        }
        cout << '|' << endl;
    }
    
}

int bin2dec(string bin){
    string num = bin;
    int dec_value = 0;
 
    int base = 1;
 
    int len = num.length();
    for (int i = len - 1; i >= 0; i--) {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
 
    return dec_value;
}

string hex2bin(string hex){
    hex.erase(0, 2);

    string bin;

    int i = 0;

    while (hex[i]){
        switch (hex[i]){
        case '0':
            bin += "0000";
            break;
        case '1':
            bin += "0001";
            break;
        case '2':
            bin += "0010";
            break;
        case '3':
            bin += "0011";
            break;
        case '4':
            bin += "0100";
            break;
        case '5':
            bin += "0101";
            break;
        case '6':
            bin += "0110";
            break;
        case '7':
            bin += "0111";
            break;
        case '8':
            bin += "1000";
            break;
        case '9':
            bin += "1001";
            break;
        case 'A':
        case 'a':
            bin += "1010";
            break;
        case 'B':
        case 'b':
            bin += "1011";
            break;
        case 'C':
        case 'c':
            bin += "1100";
            break;
        case 'D':
        case 'd':
            bin += "1101";
            break;
        case 'E':
        case 'e':
            bin += "1110";
            break;
        case 'F':
        case 'f':
            bin += "1111";
            break;
        default:
            break;
        }
        i++;
    }

    return bin;
}
