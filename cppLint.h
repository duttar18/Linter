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
        bool runThroughString(unsigned int& c, string& line, ostream &os);

        // EFFECT:  If current char is start of a <>, print chars to os until it ends
        //          leave c at index after string. this is for #include
        bool runThroughAngleBracket(unsigned int& c, string& line, ostream &os);

        // EFFECT:  If current char is start of a char, print chars to os until char ends
        //          leave c at index after string
        bool runThroughChar(unsigned int& c, string& line, ostream &os);

        // EFFECT:  If current char is start of single line commment, add two spaces before comment
        //          and ad a space after //
        bool runThroughSingleLineComment(unsigned int&c, string& line, ostream &os);

        //EFFECT: Skip space and return false if no space was there
        bool runThroughWhiteSpace(unsigned int&c, string&line);

        // EFFECT: Add indent
        void indent(ostream &os);

        // EFFECT: Run through the letters and leave c at the index after
        bool runThroughWords(unsigned int&c, string& line, ostream&os);

        // REQUIRES: c=0
        // EFFECT:  Check and format #include lines. leaves
        //          c at the end of next index after statement
        bool includeStatement(unsigned int& c, string& line, ostream &os);

        //EFFECT: Indent {}
        bool curlOpen(unsigned int& c, string& line, ostream &os, bool atNewLine);

        bool curlClose(unsigned int&c, string& line, ostream &os, bool atNewLine);

        bool semicolon(unsigned int&c, string& line, ostream&os);

    public:
        void run(istream &is, ostream &os);
};

/*TODO 
 *  multi line comment
 *  multi line ()
 *  space around ,
 *  space around == &
 *  space around = += operators
 *
 */
