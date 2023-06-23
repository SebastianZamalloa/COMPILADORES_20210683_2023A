#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

enum class TokenType {
    ID,
    INT,
    NUM,
    MAS,
    ASIGN,
    MENOS,
    POR,
    ENTRE,
    PARENTESIS_IZQ,
    PARENTESIS_DER,
    PUNTO_Y_COMA,
    FIN,
    PASS,
    ERROR
};

struct Token {
    TokenType type;
    string value;
};

char lexema[80];
string transformed;
bool comment = false;
bool isTxt = false;

void error()
{
    throw runtime_error("Error de sintaxis");
}

class sintacticAn {
private:
    vector<vector<string>> POSTS;
    vector<string> post;
    vector<string> op_;
    Token currentToken;
    stringstream inputStream;

    void evaluate(char c)
    {
        string d; d += c;
        if(op_.size() == 0)
            op_.push_back(d);
        else
        {
            if(c == EOF || c == ';')
            {
                for(int i = op_.size()-1; i >= 0; i--)
                {
                    post.push_back(op_[i]);
                    op_.pop_back();
                }
                cout<<"\nPost Actual: ";printPost();POSTS.push_back(post);
                post.clear(); op_ .clear();
            }
            else if(c == '+' || c == '-')
            {
                if(*(op_.end()-1) == "+"|| *(op_.end()-1) == "-")
                {
                    post.push_back(*(op_.end()-1));
                    op_.pop_back();
                    op_.push_back(d);
                }
                else if(*(op_.end()-1) == "*"|| *(op_.end()-1) == "/")
                {
                    for(int i = op_.size()-1; i >= 0; i--)
                    {
                        if(op_[i] == "(")
                            break;
                        post.push_back(op_[i]);
                        op_.pop_back();
                    }
                }
                else if(*(op_.end()-1) == "(")
                    op_.push_back(d);
            }
            else if(c == '*' || c == '/')
            {
                if(*(op_.end()-1) == "+"|| *(op_.end()-1) == "-")
                    op_.push_back(d);
                else if(*(op_.end()-1) == "*"|| *(op_.end()-1) == "/")
                {
                    post.push_back(*(op_.end()-1));
                    op_.pop_back();
                    op_.push_back(d);
                }
                else if(*(op_.end()-1) == "(")
                    op_.push_back(d);
            }
            else if(c == '(')
                op_.push_back(d);
            else if(c == ')')
            {
                for(int i = op_.size()-1; i >= 0; i--)
                {
                    if(op_[i] == "("){
                        op_.pop_back();
                        break;
                    }
                    post.push_back(op_[i]);
                    op_.pop_back();
                }
            }
        }
    }
    Token getNextToken() {
        Token token;
        char c = inputStream.get();
        while (isspace(c)) {
            c = inputStream.get();
        }

        if (isdigit(c)) {
            token.type = TokenType::NUM;
            token.value = "";

            while (isdigit(c) || c == '\n') {
                token.value += c;
                c = inputStream.get();
            }
            post.push_back(token.value);
            inputStream.putback(c);
        } else if (c == '+') {
            token.type = TokenType::MAS;
            token.value = "+";
            evaluate(c);
        } else if (c == '-') {
            token.type = TokenType::MENOS;
            token.value = "-";
            evaluate(c);
        } else if (c == '*') {
            token.type = TokenType::POR;
            token.value = "*";
            evaluate(c);
        } else if (c == '/') {
            token.type = TokenType::ENTRE;
            token.value = "/";
            evaluate(c);
        } else if (c == '(') {
            token.type = TokenType::PARENTESIS_IZQ;
            token.value = "(";
            evaluate(c);
        } else if (c == ')') {
            token.type = TokenType::PARENTESIS_DER;
            token.value = ")";
            evaluate(c);
        } else if (c == ';') {
            token.type = TokenType::PUNTO_Y_COMA;
            token.value = ";";
            evaluate(c);
        } else if (c == EOF || c == '\n') {
            token.type = TokenType::FIN;
            token.value = "";
            evaluate(c);
        } else {
            cerr << "Error: Caracter no reconocido: " << c << endl;
            error();
        }

        return token;
    }

    void match(TokenType expectedType) {
        if (currentToken.type == expectedType) {
            currentToken = getNextToken();
        } else {
            cerr << "Error de sintaxis. Se esperaba: " << tokenTypeToString(expectedType) << endl;
            error();
        }
    }

    string tokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::NUM:
                return "NUM";
            case TokenType::MAS:
                return "MAS";
            case TokenType::MENOS:
                return "MENOS";
            case TokenType::POR:
                return "POR";
            case TokenType::ENTRE:
                return "ENTRE";
            case TokenType::PARENTESIS_IZQ:
                return "PARENTESIS_IZQ";
            case TokenType::PARENTESIS_DER:
                return "PARENTESIS_DER";
            case TokenType::PUNTO_Y_COMA:
                return "PUNTO_Y_COMA";
            case TokenType::FIN:
                return "FIN";
            default:
                return "";
        }
    }

    void S() {
        Exp();
        if (currentToken.type == TokenType::PUNTO_Y_COMA) {
            match(TokenType::PUNTO_Y_COMA);
            if (currentToken.type == TokenType::NUM || currentToken.type == TokenType::PARENTESIS_IZQ) {
                S();
            }
        } else {
            cerr << "Error de sintaxis en S." << endl;
            error();
        }
    }

    void Exp() {
        if (currentToken.type == TokenType::NUM) {
            Term();
            Resto();
        } else if (currentToken.type == TokenType::PARENTESIS_IZQ) {
            match(TokenType::PARENTESIS_IZQ);
            Exp();
            match(TokenType::PARENTESIS_DER);
            Resto();
        } else {
            cerr << "Error de sintaxis en Exp." << endl;
            error();
        }
    }

    void Resto() {
        if (currentToken.type == TokenType::MAS || currentToken.type == TokenType::MENOS ||
            currentToken.type == TokenType::POR || currentToken.type == TokenType::ENTRE) {
            match(currentToken.type);
            Exp();
        } else if (currentToken.type == TokenType::PUNTO_Y_COMA || currentToken.type == TokenType::PARENTESIS_DER) {
            // No hacer nada, producción Resto -> ε
        } else {
            cerr << "\nError de sintaxis en Resto." << endl;
            error();
        }
    }

    void Term() {
        if (currentToken.type == TokenType::NUM) {
            match(TokenType::NUM);
        } else {
            cerr << "\nError de sintaxis en Term." << endl;
            error();
        }
    }

public:
    sintacticAn(string input) {
        inputStream.str(input);
        currentToken = getNextToken();
    }

    void parse() {
        S();
        if (currentToken.type == TokenType::FIN) {
            cout << "Analisis sintactico completado sin errores\n" << endl;
        } else {
            cout<<currentToken.value<<endl;
            cerr << "\nError: Caracteres adicionales después del final del análisis sintáctico." << endl;
            error();
        }
    }
    void printPost()
    {
        for(auto i:post)
        {
            cout<<i<<" ";
        }cout<<endl;
    }
};

TokenType espalres()
{
    if(strcmp(lexema,"int")==0) return TokenType::INT;
	return TokenType::PASS;
}

void mostrar(TokenType token)
{
    if(token == TokenType::ERROR){cout<<"\nERROR LEXICO\n";error();}
    switch(token)
    {
        case TokenType::ID: cout<<"token = ID ["<<lexema<<"] \n";    break;
        case TokenType::NUM:  cout<<"token = NUM ["<<lexema<<"] \n";    break;
        case TokenType::MAS:  cout<<"token = SIGNOSUMA ["<<lexema<<"] \n";    break;
        case TokenType::MENOS:  cout<<"token = SIGNORESTA ["<<lexema<<"] \n";    break;
        case TokenType::POR:  cout<<"token = SIGNOMULTIPLICACION ["<<lexema<<"] \n";    break;
        case TokenType::ENTRE:  cout<<"token = SIGNODIVISION ["<<lexema<<"] \n";    break;
        case TokenType::ASIGN: cout<<"token = ASIGNACION ["<<lexema<<"] \n";    break; 
        case TokenType::INT:  cout<<"token = INT_DATATYPE ["<<lexema<<"] \n";; break;	
        case TokenType::PARENTESIS_IZQ:  cout<<"token = PARI ["<<lexema<<"] \n";;    break;
        case TokenType::PARENTESIS_DER:  cout<<"token = PARD ["<<lexema<<"] \n";;    break;
        case TokenType::PUNTO_Y_COMA:  cout<<"token = PUNTOYCOMA ["<<lexema<<"] \n";;    break;
        default: break;
    }
}

void resetLexema(int n)
{
    while(n--&&n!=0)
        lexema[n-1] = 0;
}

TokenType scaner(string &input)
{
	int c, j = 0, i = 0;
    if(comment)
    {
        c=input[j++];
        if(c == '*')
        {
            c=input[j++];
            if(c == '/')
            {
                input.erase(0,j);
                comment = 0;
                TokenType::PASS;
            }
            j--;
        }
        c=input[j++];
        do
        {     
            while(c != '*')
            {
                if(input[j] == 0)
                {
                    input.erase(0,j-1);
                    TokenType::PASS;
                }
                c=input[j++];
            }
            c=input[j++];
        }while(c != '/'); 
        comment = 0; 
        input.erase(0,j);
        return TokenType::PASS;
    }
	do   c=input[j++]; while(isspace(c));  //ignora blancos

	if(c==EOF) return TokenType::FIN;

	if(isalpha(c))   //regla del ID
	{
		while(isalnum(c)|| c=='_')
        {
            lexema[i++]=c;
			c=input[j++];
        }
		TokenType i_ = espalres();
        input.erase(0,j-1);
		if(i_ != TokenType::PASS)
		   return i_;
		return TokenType::ID; 
	}
    if(isdigit(c))   //regla del NUM
	{
        while(isdigit(c))
        {
            lexema[i++] = c;
		    c = input[j++];
		}
        input.erase(0,j-1);
		return TokenType::NUM;
	}
    if((c==';')||(c=='(')||(c==')')||(c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='='))
    {  
        lexema[i++] = c;
        c = input[j++];
        if(lexema[i-1] == '/' && c=='/')
        {
            while(c != 0)
                c = input[j++];
            input.erase(0,j-1);
            resetLexema(80);
            return TokenType::PASS;
        }
        else if(lexema[i-1] == '/' && c=='*')
        {
            comment = true;
            input.erase(0,j);
            resetLexema(80);
            return TokenType::PASS;
        }
        input.erase(0,j-1);
        if (lexema[i-1] == '+') {
            return TokenType::MAS;
        } else if (lexema[i-1] == '-') {
            return TokenType::MENOS;
        } else if (lexema[i-1] == '*') {
            return TokenType::POR;
        } else if (lexema[i-1] == '/') {
            return TokenType::ENTRE;
        } else if (lexema[i-1] == '(') {
            return TokenType::PARENTESIS_IZQ;
        } else if (lexema[i-1] == ')') {
            return TokenType::PARENTESIS_DER;
        } else if (lexema[i-1] == ';') {
            return TokenType::PUNTO_Y_COMA;
        }
    }

	return TokenType::ERROR;

}

int main()
{
    TokenType token;
    string input = " ";
    cout<<"\n\nIngrese texto (Si desea abrir un archivo ponga el nombre + .txt): ";
    //while(!input.empty())
    {
        getline(cin,input);
        cout<<endl;
        if(input.empty())
            error();
        if(input.length() >= 5)
        {
            string templ = ".txt";
            isTxt = true;
            for(int i = 0; i<4; i++)
            {
                if(templ[i]!=*(input.end()-4+i))
                    isTxt = false;
            }
            if(isTxt)
            {
                ifstream reader(input);
                string read;
                while(getline(reader,input))
                {
                    while(1)
                    {
                        if(input.empty())
                            break;
                        token = scaner(input);
                        mostrar(token);
                        transformed += lexema;
                        transformed += " ";
                        resetLexema(80);
                    }
                }
                input = " ";
            }
        }
        if(!isTxt)
        {
            while(1)
            {
                if(input.empty())
                    break;
                token=scaner(input);
                mostrar(token);
                transformed += lexema;
                transformed += " ";
                resetLexema(80);
            }
            input = " ";
        }
    }
    cout<<"\nANALIZADOR LEXICO SIN ERRORES \n";
    cout<<transformed<<endl;
    sintacticAn parser(transformed);
    parser.parse();
}
