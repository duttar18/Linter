#include <iostream>
#include <string>
#include <cassert>

using namespace std;

// functor reads is file, lints, and output to os file
class cppLint{
    private:
        int indLev = 0; // Indentation Level
        int paran = -1; // on multiline paramaters will signify start char otherwise -1
        string line;

        // EFFECT:  If current char is start of a string, print chars to os until string ends
        //          leave c at index after string
        bool runThroughString(unsigned int& c, string& line, ostream &os){
            //if hits a string "" go until completion
            if(line.at(c)=='\"'){
                os<<line.at(c);
                while(line.at(c)!='\"'){
                    //if hit a dash then skip next char
                    if(c=='\\'){
                        c++;
                        os<<line.at(c);
                    }
                    c++;
                    os<<line.at(c);
                }
                c++;
                return true;
            }
            return false;
        }

        // EFFECT:  If current char is start of a <>, print chars to os until it ends
        //          leave c at index after string. this is for #include
        bool runThroughAngleBracket(unsigned int& c, string& line, ostream &os){
            //if hits a string "" go until completion
            if(line.at(c)=='<'){
                os<<line.at(c);
                while(line.at(c)!='>'){
                    //if hit a dash then skip next char
                    if(line.at(c)=='\\'){
                        c++;
                        os<<line.at(c);
                    }
                    c++;
                    os<<line.at(c);
                }
                c++;
                return true;
            }
            return false;
        }

        // EFFECT:  If current char is start of a char, print chars to os until char ends
        //          leave c at index after string
        bool runThroughChar(unsigned int& c, string& line, ostream &os){
            //if hits a string '' go until completion
            if(line.at(c)=='\''){
                os<<line.at(c);
                while(line.at(c)!='\''){
                    //if hit a dash then skip next char
                    if(c=='\\'){
                        c++;
                        os<<line.at(c);
                    }
                    c++;
                    os<<line.at(c);
                }
                c++;
                return true;
            }
            return false;
        }

        // EFFECT:  If current char is start of single line commment, add two spaces before comment
        //          and ad a space after //
        bool runThroughSingleLineComment(unsigned int&c, string& line, ostream &os){
            //if hits a // then go to end of line
            if(line.at(c)=='/' && line.at(c+1)=='/' ){
                os<<"  // ";
                c+=2;
                while(c<line.length()){
                    os<<line.at(c);
                    c++;
                }
                return true;
            }
            return false;
        }

        //EFFECT: Skip space and return false if no space was there
        bool runThroughWhiteSpace(unsigned int&c, string&line){
            bool space = false;
            while(c<line.length() && (line.at(c)==' ' || line.at(c)=='\t')){
                c++;
                space = true;
            }
            return space;
        }

        // EFFECT: Add indent
        void indent(ostream &os){
            for(int i=0; i<indLev; i++){
                os<<"  ";
            }
        }

        // EFFECT: Run through the letters and leave c at the index after
        bool runThroughWords(unsigned int&c, string& line, ostream&os){
            string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP_";
            if(chars.find(line.at(c))==string::npos) return false;
            while(c<line.length() && chars.find(line.at(c))!=string::npos){
                os<<line.at(c);
                c++;
            }
            return true;
        }

        // REQUIRES: c=0
        // EFFECT:  Check and format #include lines. leaves
        //          c at the end of next index after statement
        bool includeStatement(unsigned int& c, string& line, ostream &os){
            assert(c==0);
            if(line.at(0)!='#') return false;

            //'#include '
            os<<"#";
            c++;
            runThroughWhiteSpace(c,line);
            runThroughWords(c,line,os);
            os<<" ";

            //include
            runThroughWhiteSpace(c,line);
            runThroughString(c,line,os);
            runThroughAngleBracket(c,line,os);

            //check if thers a comment after
            runThroughWhiteSpace(c,line);
            runThroughSingleLineComment(c,line,os);
            os<<"\n";
            return true;

        }

        //EFFECT: Indent {}
        bool curlOpen(unsigned int& c, string& line, ostream &os, bool atNewLine){
            if(line.at(c)!='{') return false;
            c++;
            runThroughWhiteSpace(c,line);
            if(line.at(c+1)=='}'){
                os<<"{}";
                c++;
                runThroughWhiteSpace(c,line);
                if(line.at(c)==';'){
                    os<<";\n";
                    c++;
                }
            }
            else{
                if(!atNewLine) os<<"\n";
                indent(os);
                os<<"{";
                runThroughWhiteSpace(c,line);
                runThroughSingleLineComment(c,line,os);
                os<<"\n";
                indLev+=1;
            }
            return true;
        }

        bool curlClose(unsigned int&c, string& line, ostream &os, bool atNewLine){
            if(line.at(c)!='}') return false;
            indLev-=1;
            if(!atNewLine) os<<"\n";
            indent(os);
            os<<"}";
            c++;
            if(line.at(c)==';'){
                os<<";";
                c++;
            }
            runThroughWhiteSpace(c,line);
            runThroughSingleLineComment(c,line,os);
            os<<"\n";
        }

        bool semicolon(unsigned int&c, string& line, ostream&os){
            if(line.at(c)!=';') return false;
            c++;
            os<<";\n";
            return true;
        }

    public:
        void operator() (istream &is, ostream &os) {
            while(getline(is,line)){
                unsigned int c = 0; // character count

                //check for #include line
                includeStatement(c,line,os);

                //skip given indent and replace with new indentation
                runThroughWhiteSpace(c,line);
                if(!curlOpen(c,line,os,true)){
                    curlClose(c,line,os,true);
                }

                indent(os);


                while(c<line.length()){

                    if(runThroughString(c, line, os)) continue;
                    if(runThroughChar(c, line, os)) continue;
                    if(runThroughWhiteSpace(c, line)){
                        os<<" ";
                        continue;
                    }
                    if(runThroughWords(c,line,os)) continue;
                    if(curlOpen(c,line,os,false)) continue;
                    if(curlClose(c,line,os,false)) continue;
                    if(runThroughSingleLineComment(c, line, os)) continue;
                    if(semicolon(c,line,os)) continue;
                    os<<line.at(c);
                    c++;
                }
            }
        }
};
/*TODO 
 *  multi line comment
 *  multi line ()
 *  space around ,
 *  space around == &
 *  space around = += operators
 *
 */
