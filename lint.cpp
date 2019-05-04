#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;


//Error Types
struct fileFailure {
    string file;
    fileFailure(const char* f){
        while(*f!='\0'){
            file+=*f;
            f++;
        }
    }
};
struct fileTypeDNE {
    string file;
    string extention;
    fileTypeDNE(const char*f,const char*e){
        while(*f!='\0'){
            file+=*f;
            f++;
        }
        while(*e!='\0'){
            extention+=*e;
            e++;
        }
    }
    
};

//String Compare
bool stringComp(const char*a, const char*b){
    while(*a!='\0' && *b!='\0'){
        if(*a!=*b) return false;
        a++;
        b++;
    }
    return *a==*b;
}

const char* getExtention(const char* fileName){
    bool hasExtention = false;
    while(*fileName!='\0'){
        fileName++;
        if(*fileName=='.'){
            hasExtention = true;
        }
    }
    if(!hasExtention) return nullptr;
    while(*fileName!='.'){
        fileName--;
    }
    return fileName;
}

void lint(char fileName[]){

    string input = fileName;
    ifstream inFile;
    inFile.open(input);
    if(!inFile.is_open()){
        throw fileFailure{fileName};
    }

    //ofstream outFile;
    const char* extention = getExtention(fileName);
    if(stringComp(extention,".cpp")){
        cout<<"hi";
    }
    else{
        throw fileTypeDNE{fileName,extention};
    }
    inFile.close();
}
int main(int argc, char* argv[]){
    for(int i =1; i<argc; i++){
        try{
            lint(argv[i]);
        }
        catch(fileTypeDNE fileType){
            cout<<"ERROR: unsupported file type "<<fileType.extention<<" of "<<fileType.file<<endl;
        }
        catch(fileFailure file){
            cout<<"ERROR: could not open "<<file.file<<endl;
        }
    }
}


