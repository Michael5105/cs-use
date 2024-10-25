#include "CompilerParser.h"


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this->tokens = tokens;
    tokenIndex = 0;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    return compileClass();
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* trees = new ParseTree("class","");
    trees->addChild(mustBe("keyword","class"));
    tokenIndex++;

    if(current() -> getType() == "identifier"){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    trees->addChild(mustBe("symbol","{"));
    tokenIndex++;

    while(have("keyword","static")||("keyword","field")){
        trees -> addChild(compileClassVarDec());
    }
    while(have("keyword","constructor")||("keyword","function")||("keyword","method")){
        trees -> addChild(compileSubroutine());
    }
    trees->addChild(mustBe("symbol","}"));
    tokenIndex++;

    return trees;
    
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* trees = new ParseTree("ClassVarDec","");
    trees ->addChild(current());
    tokenIndex++;
    if((current()->getType() =="identifier")
    ||(current()->getType() =="keyword" && (current()->getValue() =="int"))
    ||(current()->getType() =="keyword" && (current()->getValue() =="char"))
    ||(current()->getType() =="keyword" && (current()->getValue() =="bool"))){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    if(current()->getType() == "identifier"){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    while(have("symbol",",")){
        trees -> addChild(mustBe("symbol",","));
        tokenIndex++;
        if(current()->getType() == "identifier"){
            trees -> addChild(current());
            tokenIndex++;
        }else{
            throw ParseException();
        }

    }
    trees -> addChild(mustBe("symbol",";"));
    tokenIndex++;

    return trees;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* trees = new ParseTree("Subroutine","");

    trees -> addChild(current());
    tokenIndex++;

    if((current()->getType() == "identifier")
    ||(current()->getType() =="keyword" && (current()->getValue() =="int"))
    ||(current()->getType() =="keyword" && (current()->getValue() =="char"))
    ||(current()->getType() =="keyword" && (current()->getValue() =="bool"))
    ||(current()->getType() =="keyword" && (current()->getValue() =="void"))){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    if(current()->getType() == "identifier"){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    trees -> addChild(mustBe("symbol","("));
    tokenIndex++;

    trees -> addChild(compileParameterList());

    trees -> addChild(mustBe("symbol",")"));
    tokenIndex++;

    trees -> addChild(compileSubroutineBody());

    return trees;

}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree* trees = new ParseTree("ParameterList","");
    if((current()->getType() == "identifier")
        ||(current()->getType() =="keyword" && (current()->getValue() =="int"))
        ||(current()->getType() =="keyword" && (current()->getValue() =="char"))
        ||(current()->getType() =="keyword" && (current()->getValue() =="bool"))){
            trees -> addChild(current());
            tokenIndex++;
        if(current()->getType() == "identifier"){
            trees -> addChild(current());
            tokenIndex++;
        }else{
            throw ParseException();
        }
        while(tokenIndex < this->tokens.size()&&have("symbol",",")){
            trees -> addChild(mustBe("symbol",","));
            tokenIndex++; 

            if((current()->getType() == "identifier")
                ||(current()->getType() =="keyword" && (current()->getValue() =="int"))
                ||(current()->getType() =="keyword" && (current()->getValue() =="char"))
                ||(current()->getType() =="keyword" && (current()->getValue() =="bool"))){
            trees -> addChild(current());
            tokenIndex++;
            }else{
                throw ParseException();
            }

            if(current()->getType() == "identifier"){
                trees -> addChild(current());
                tokenIndex++;
            }else{
                throw ParseException();
            }

        }   
    }
    return trees;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* trees = new ParseTree("SubroutineBody","");

    trees -> addChild(mustBe("symbol","{"));
    tokenIndex++;

    while(have("keyword","var")){
        trees -> addChild(compileClassVarDec());
    }
    trees -> addChild(compileStatements());

    trees -> addChild(mustBe("symbol","}"));
    tokenIndex++;
    
    return trees;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree* trees = new ParseTree("VarDec","");

    trees -> addChild(mustBe("keyword","var"));
    tokenIndex++;

    if((current()->getType() == "identifier")
        ||(current()->getType() =="keyword" && (current()->getValue() =="int"))
        ||(current()->getType() =="keyword" && (current()->getValue() =="char"))
        ||(current()->getType() =="keyword" && (current()->getValue() =="bool"))){
        trees -> addChild(current());
        tokenIndex++;
    }else{
            throw ParseException();
        }

    if(current()->getType() == "identifier"){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    while(tokenIndex < this->tokens.size()&&have("symbol",",")){
        trees -> addChild(mustBe("symbol",","));
        tokenIndex++; 

        if(current()->getType() == "identifier"){
            trees -> addChild(current());
            tokenIndex++;
        }else{
            throw ParseException();
        }

    }    
    trees -> addChild(mustBe("symbol",";"));
    tokenIndex++;
    
    return trees;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* trees = new ParseTree("Statements","");

    while(tokenIndex < this -> tokens.size()&&
        (have("keyword","let")||have("keyword","if")
        ||have("keyword","while")||have("keyword","do")
        ||have("keyword","return"))){
        if(have("keyword","let")){
            trees -> addChild(compileLet());
        }
        if(have("keyword","if")){
            trees -> addChild(compileIf());
        }
        if(have("keyword","while")){
            trees -> addChild(compileWhile());
        }
        if(have("keyword","do")){
            trees -> addChild(compileDo());
        }
        if(have("keyword","return")){
            trees -> addChild(compileReturn());
        }

    }
    return trees;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* trees = new ParseTree("letStatement","");

    trees ->addChild(mustBe("keyword","let"));
    tokenIndex++;

    if(current()->getType() == "identifier"){
        trees -> addChild(current());
        tokenIndex++;
    }else{
        throw ParseException();
    }

    if(have("symbol","[")){
        trees ->addChild(mustBe("keyword","["));
        tokenIndex++;

        trees ->addChild(compileExpression());

        trees ->addChild(mustBe("keyword","]"));
        tokenIndex++;
    }
    trees ->addChild(mustBe("keyword","="));
    tokenIndex++;

    trees ->addChild(compileExpression());

    trees ->addChild(mustBe("keyword",";"));
    tokenIndex++;

    return trees;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree* trees = new ParseTree("ifStatement","");

    trees ->addChild(mustBe("keyword","if"));
    tokenIndex++;

    trees ->addChild(mustBe("symbol","("));
    tokenIndex++;

    trees ->addChild(compileExpression());

    trees ->addChild(mustBe("symbol",")"));
    tokenIndex++;

    trees ->addChild(mustBe("symbol","{"));
    tokenIndex++;

    trees ->addChild(compileStatements());

    trees ->addChild(mustBe("symbol","}"));
    tokenIndex++;

    if(tokenIndex < this->tokens.size() && have("keyword","else")){
        trees ->addChild(mustBe("keyword","else"));
        tokenIndex++;

        trees ->addChild(mustBe("symbol","{"));
        tokenIndex++;

        trees ->addChild(compileStatements());

        trees ->addChild(mustBe("symbol","}"));
        tokenIndex++;

    }
    return trees;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree* trees = new ParseTree("while","");

    trees ->addChild(mustBe("keyword","while"));
    tokenIndex++;

    trees ->addChild(mustBe("symbol","("));
    tokenIndex++;

    trees ->addChild(compileExpression());

    trees ->addChild(mustBe("symbol",")"));
    tokenIndex++;

    trees ->addChild(mustBe("symbol","{"));
    tokenIndex++;

    trees ->addChild(compileStatements());

    trees ->addChild(mustBe("symbol","}"));
    tokenIndex++;

    return trees;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* trees = new ParseTree("do","");

    trees ->addChild(mustBe("keyword","do"));
    tokenIndex++;

    trees ->addChild(compileExpression());

    trees ->addChild(mustBe("symbol",";"));
    tokenIndex++;

    return trees;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* trees = new ParseTree("return","");

    trees ->addChild(mustBe("keyword","return"));
    tokenIndex++;

    if(have("keyword","skip")){
        trees ->addChild(compileExpression());
    }

    trees ->addChild(mustBe("symbol",";"));
    tokenIndex++;

    return trees;

}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* trees = new ParseTree("expression","");

    if(have("keyword","skip")){
        trees -> addChild(mustBe("keyword","skip"));
        tokenIndex++;
    }

    return trees;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    std::list<Token*>::iterator trees =this -> tokens.begin();
    advance(trees,tokenIndex);

    return *trees;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    if(current()->getValue() == expectedValue && current() ->getType() == expectedType){
        return true;
    }

    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    if(current()->getValue() == expectedValue && current()->getType()== expectedType){
        return current();
    }else{
        throw ParseException();
    }

    return current();
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
