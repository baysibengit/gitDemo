//
// Created by Ben Baysinger  on 1/16/24.
//


#include "Expr.hpp"


//**********************EXP CLASS IMPLEMENTATIONS **************************************

std::string Expr:: to_string(){
    std::stringstream st("");
    this->print(st);
    return st.str();
}

std::string Expr:: to_stringPP(){
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}
//**********************NUM CLASS IMPLEMENTATIONS **************************************

Num::Num( int val ) {
    this->val = val;
}
bool Num::equals(Expr *comp) {

    Num* numPtr = dynamic_cast<Num*>(comp);
    if ( numPtr == nullptr ) {
        return false;
    }
    else {
        return numPtr->val == this->val;
    }
}

int Num::interp() {
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string valToSub, Expr *expr) {
    return this;
}

void Num::print( std::ostream &ostream){
    ostream<<std::to_string(val);
}

void Num::pretty_print( std::ostream  &ostream){
    ostream<<std::to_string(val);
}

void Num::pretty_print_at( std::ostream  &ostream, precedence_t precedence) {
    ostream<<std::to_string(val);
}


//**********************ADD CLASS IMPLEMENTATIONS **************************************

Add::Add(Expr *lhs, Expr *rhs) {
this->lhs = lhs;
this->rhs = rhs;
}

bool Add::equals ( Expr *comp ) {

    Add* addPtr = dynamic_cast<Add*>(comp);
    if ( addPtr == nullptr ) {
        return false;
    }
    else {
        return addPtr->lhs->equals(this->lhs) && addPtr->rhs->equals(this->rhs);
    }
}

int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Add::subst(std::string valToSub, Expr *expr) {

    return new Add(this->lhs->subst(valToSub,expr), this->rhs->subst(valToSub,expr)) ;
}

void Add::print( std::ostream &ostream){

    ostream << "(";
    this->lhs->print(ostream);
    ostream<< "+";
    this->rhs->print(ostream);
    ostream<< ")";
}

void Add::pretty_print( std::ostream  &ostream){
    pretty_print_at(ostream, prec_add);
}

void Add::pretty_print_at(std::ostream &ostream, precedence_t precedence) {

    if(precedence > prec_add){
        ostream << "(";
    }
    this->lhs->pretty_print_at(ostream, prec_add);
    ostream<< " + ";
    this->rhs->pretty_print_at(ostream, prec_add);

    if(precedence > prec_add){
        ostream << ")";
    }

}

//**********************MULT CLASS IMPLEMENTATIONS *************************************

Mult::Mult( Expr *lhs, Expr *rhs ) {
this->lhs = lhs;
this->rhs = rhs;
}

bool Mult::equals(Expr *comp) {
    Mult* multPtr = dynamic_cast<Mult*>(comp);
    if ( multPtr == nullptr ) {
        return false;
    }
    else {
        return multPtr->lhs->equals(this->lhs) && multPtr->rhs->equals(this->rhs);
    }
}

int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

bool Mult::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Mult::subst(std::string valToSub, Expr *expr) {

    return new Mult(this->lhs->subst(valToSub,expr), this->rhs->subst(valToSub,expr)) ;
}

void Mult::print( std::ostream &ostream){
    ostream << "(";
    this->lhs->print(ostream);
    ostream<< "*";
    this->rhs->print(ostream);
    ostream<< ")";
}

void Mult::pretty_print( std::ostream  &ostream){
    pretty_print_at(ostream, prec_mult);
}

void Mult::pretty_print_at(std::ostream &ostream, precedence_t precedence) {

    if(precedence > prec_mult){
        ostream << "(";
    }
    this->lhs->pretty_print_at(ostream, static_cast<precedence_t>(prec_mult + 1));
    ostream<< " * ";
    this->rhs->pretty_print_at(ostream, prec_mult);

    if(precedence > prec_mult){
        ostream << ")";
    }
}


//**********************VARIABLE CLASS IMPLEMENTATIONS ***********************************
Variable::Variable(std::string value) {
    this->value = value;
}

bool Variable::equals(Expr *comp) {
    Variable* varPtr = dynamic_cast<Variable*>(comp);
    if ( varPtr == nullptr ) {
        return false;
    }
    else {
        return varPtr->value == this->value;
    }
}

int Variable::interp() {
    throw std::runtime_error("Variable has no value");
}

bool Variable::has_variable() {
    return true;
}

Expr* Variable::subst(std::string valToSub, Expr *expr) {

    if ( this->value == valToSub ){
        return expr;
    }
    return this;
}

void Variable::print( std::ostream &ostream){

    ostream<< this->value;

}

void Variable::pretty_print( std::ostream  &ostream){
    ostream<< this->value;
}

void Variable::pretty_print_at(std::ostream &ostream, precedence_t precedence) {
    ostream<< this->value;
}

//**********************CATCH TESTING **************************************

TEST_CASE("Num Class")
{
    SECTION("Equals") {

        CHECK((new Num(1))->equals(new Variable("x")) == false);
        CHECK((new Num(1))->equals(new Num(2)) == false);
        //Zero check
        CHECK((new Num(0))->equals(new Num(0)) == true);
        //Negative check
        CHECK((new Num(-2))->equals(new Num(-2)) == true);
        //Max and min check
        CHECK((new Num(INT16_MAX))->equals(new Num(INT16_MAX)) == true);
        CHECK((new Num(INT16_MIN))->equals(new Num(INT16_MIN)) == true);
        //Repeats
        CHECK((new Num(777))->equals(new Num(777)) == true);
    }
    SECTION("Interp"){

        CHECK((new Num(7))->interp() == 7);
        CHECK((new Num(8))->interp() != 6);
        //Checking interp after a sub change
        CHECK((new Variable("y"))->subst("y", new Num(8))->interp() == 8);
        CHECK_THROWS_WITH((new Variable("x"))->subst("yx", new Num(3))->interp(),  "Variable has no value");
        CHECK_THROWS_WITH((new Add(new Num(2), new Variable("4")))->interp(), "Variable has no value");

    }
    SECTION("HasVariable") {

        CHECK((new Num(7))->has_variable() == false);
        CHECK((new Num(5))->has_variable() != true );

    }
    SECTION("Substitution") {

        CHECK((new Num(7))->subst("x", new Variable("y"))->equals(new Num(7)));
        //Sub change variable to num
        CHECK((new Variable("x"))->subst("x", new Num(8))->equals(new Num(8)));
    }
    SECTION("Print ") {
        CHECK( (new Num(10))->to_string() == "10" );
        CHECK( (new Num (-11))->to_string() == "-11");
        CHECK( (new Num (777))->to_string() == "777");
        CHECK( (new Num (0))->to_string() == "0");
        CHECK( (new Num (83))->to_string() != "72");
    }
    SECTION("Pretty Print") {
        CHECK( (new Num(10))->to_stringPP() == "10" );
        CHECK( (new Num (-11))->to_stringPP() == "-11");
        CHECK( (new Num (777))->to_stringPP() == "777");
        CHECK( (new Num (0))->to_stringPP() == "0");
        CHECK( (new Num (83))->to_stringPP() != "72");
    }
}

TEST_CASE("Add Class")
{
    SECTION("Equals") {

        CHECK((new Add(new Num(2), new Num(3)))->equals(new Add(new Num(2), new Num(3))) == true);
        CHECK((new Add(new Num(2), new Num(3)))->equals(new Add(new Num(3), new Num(2))) == false);
        //Add vs Multiplication check
        CHECK((new Add(new Num(2), new Num(3)))->equals(new Mult(new Num(2), new Num(3))) == false);
        //Add using Num and Var expression
        CHECK((new Add(new Num(2), new Num(3)))->equals(new Add(new Variable("2"), new Variable("3"))) == false);
        CHECK((new Add(new Variable("2"), new Variable("3")))->equals(new Add(new Variable("2"), new Variable("3"))) == true);
    }
    SECTION("Interp") {

        CHECK((new Add(new Add(new Num(10), new Num(15)), new Add(new Num(20), new Num(20))))->interp() == 65);
        //Exception thrown if variable contained
        CHECK_THROWS_WITH((new Add(new Num(2), new Variable("4")))->interp(), "Variable has no value");
        //Add with multiplication
        CHECK((new Add(new Mult(new Num(2), new Num(4)), new Add(new Num(5), new Num(10))))->interp() == 23);
        //Add negative numbers
        CHECK((new Add(new Add(new Num(-5), new Num(15)), new Add(new Num(-15), new Num(23))))->interp() == 18);
        //Add with substitute
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Num(8)), new Add(new Num(13), new Num(30))))->interp() == 66);
    }
    SECTION("HasVariable") {

        CHECK((new Add(new Num(2), new Num(4)))->has_variable() == false);
        //Has variable then substitutes
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Num(8)), new Add(new Num(13), new Num(30))))->has_variable() == false);
        //Has variable then substitutes another variable
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Variable("y")), new Add(new Num(13), new Num(30))))->has_variable() == true);
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(7), new Variable("y"))), new Add(new Num(13), new Num(30))))->has_variable() == true);
        //Has variable then substitutes it out
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(7), new Num(8))), new Add(new Num(13), new Num(30))))->has_variable() == false);
    }
    SECTION("Substitution") {

        //Var with Num
        CHECK((new Add(new Variable("x"), new Num(7)))->subst("x", new Num(9))->equals(new Add(new Num(9), new Num(7))) );
        //Var with another var
        CHECK((new Add(new Variable("x"), new Num(7)))->subst("x", new Variable("y"))->equals(new Add(new Variable("y"), new Num(7))) );
        //Var with add expression
        CHECK((new Add((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(2), new Num(3))),new Add(new Num(13), new Num(30))))
                      ->equals(new Add(new Add(new Add(new Num(2), new Num(3)),new Num(15)),new Add(new Num(13), new Num(30)))));
        //Var with multiplication expression within add
        CHECK((new Add((new Mult(new Variable("x"), new Num(26)))->subst("x", new Add(new Num(7), new Num(8))),new Add(new Num(14), new Num(25))))
                      ->equals(new Add(new Mult(new Add(new Num(7), new Num(8)),new Num(26)),new Add(new Num(14), new Num(25)))));
    }
    SECTION("Print") {
        CHECK( (new Add(new Num(1), new Add(new Num(2), new Num(3))))->to_string() == "(1+(2+3))" );
        //Add with multiplication on right
        CHECK( (new Add(new Num(2), new Mult(new Num(5), new Num(9))))->to_string() == "(2+(5*9))" );
        //Add with multiplication on left
        CHECK( (new Add(new Mult(new Num(3), new Num(9)), new Add(new Num(7), new Num(8))))->to_string() == "((3*9)+(7+8))" );
        //Add with two adds
        CHECK( (new Add(new Add(new Num(6), new Num(3)), new Add(new Num(1), new Num(4))))->to_string() == "((6+3)+(1+4))" );
        //Add with two multiplications
        CHECK( (new Add(new Mult(new Num(9), new Num(2)), new Mult(new Num(3), new Num(5))))->to_string() == "((9*2)+(3*5))" );
        //Add with var
        CHECK( (new Add(new Variable("x"), new Add(new Num(2), new Num(3))))->to_string() == "(x+(2+3))" );
        //Add with num and var
        CHECK( (new Add(new Num(1), new Variable("y")))->to_string() == "(1+y)" );
        //Add with var and multiplication
        CHECK( (new Add(new Variable("z"), new Mult(new Num(4), new Num(9))))->to_string() == "(z+(4*9))" );
    }
    SECTION("Pretty Print") {
        //Add with num on left and add expression on right
        CHECK( (new Add(new Num(1), new Add(new Num(2), new Num(3))))->to_stringPP() == "1 + 2 + 3" );
        //Add with num on right and add expression on left
        CHECK( (new Add( new Add(new Num(7), new Num(7)), new Num(6)))->to_stringPP() == "7 + 7 + 6");
        //Add with add expression on left and multiplication on right. Association to the right example
        CHECK( (new Add( new Add(new Num(7), new Num(7)), new Mult(new Num(9), new Num(2))))->to_stringPP() == "7 + 7 + 9 * 2");
        //Add with multiplication expression on left and add on right. Grouped by precedence example
        CHECK( (new Add( new Mult(new Num(3), new Num(5)), new Add(new Num(6), new Num(1))))->to_stringPP() == "3 * 5 + 6 + 1");
        //Add with var in left and add expression on right.
        CHECK( (new Add( new Variable("x"), new Add(new Num(4), new Num(4))))->to_stringPP() == "x + 4 + 4");
        //Add with add expression on left and var on right
        CHECK( (new Add( new Add(new Num(8), new Num(1)), new Variable("y")))->to_stringPP() == "8 + 1 + y");
        //Association to right example
        CHECK( (new Add( new Num(1), new Mult(new Num(2), new Num(3))))->to_stringPP() == "1 + 2 * 3");
    }

}

TEST_CASE("Multiplication Class")
{
    SECTION("Equals") {

        CHECK((new Mult(new Num(2), new Num(3)))->equals(new Mult(new Num(2), new Num(3))) == true);
        CHECK((new Mult(new Num(2), new Num(3)))->equals(new Mult(new Num(3), new Num(2))) == false);
        //Mult vs Add check
        CHECK((new Mult(new Num(2), new Num(3)))->equals(new Add(new Num(2), new Num(3))) == false);
        //Mult using Num and Var expression
        CHECK((new Mult(new Num(2), new Num(3)))->equals(new Mult(new Variable("2"), new Variable("3"))) == false);
        CHECK((new Mult(new Variable("2"), new Variable("3")))->equals(new Mult(new Variable("2"), new Variable("3"))) == true);
    }
    SECTION("Interp") {

        CHECK((new Mult(new Num(3), new Num(2)))->interp() == 6);
        //Exception thrown if variable contained
        CHECK_THROWS_WITH((new Mult(new Num(2), new Variable("4")))->interp(), "Variable has no value");
        //Multiplication with add
        CHECK((new Mult(new Add(new Num(2), new Num(4)), new Mult(new Num(5), new Num(10))))->interp() == 300);
        //Multiplication negative numbers
        CHECK((new Mult(new Mult(new Num(-5), new Num(15)), new Add(new Num(-15), new Num(23))))->interp() == -600);
        //Multiplication with substitute
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Num(8)), new Add(new Num(13), new Num(30))))->interp() == 989);
    }
    SECTION("HasVariable") {

        CHECK((new Mult(new Num(2), new Num(4)))->has_variable() == false);
        //Has variable then substitutes
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Num(8)), new Add(new Num(13), new Num(30))))->has_variable() == false);
        //Has variable then substitutes another variable
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Variable("y")), new Add(new Num(13), new Num(30))))->has_variable() == true);
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(7), new Variable("y"))), new Add(new Num(13), new Num(30))))->has_variable() == true);
        //Has variable then substitutes it out
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(7), new Num(8))), new Add(new Num(13), new Num(30))))->has_variable() == false);
    }
    SECTION("Substitution") {

        //Var with Num
        CHECK((new Mult(new Variable("x"), new Num(7)))->subst("x", new Num(9))->equals(new Mult(new Num(9), new Num(7))) );
        //Var with another var
        CHECK((new Mult(new Variable("x"), new Num(7)))->subst("x", new Variable("y"))->equals(new Mult(new Variable("y"), new Num(7))) );
        //Var with add expression
        CHECK((new Mult((new Add(new Variable("x"), new Num(15)))->subst("x", new Add(new Num(2), new Num(3))),new Add(new Num(13), new Num(30))))
                      ->equals(new Mult(new Add(new Add(new Num(2), new Num(3)),new Num(15)),new Add(new Num(13), new Num(30)))));
        //Var with add expression within multiplication
        CHECK((new Mult((new Add(new Variable("x"), new Num(26)))->subst("x", new Add(new Num(7), new Num(8))),new Add(new Num(14), new Num(25))))
                      ->equals(new Mult(new Add(new Add(new Num(7), new Num(8)),new Num(26)),new Add(new Num(14), new Num(25)))));
    }
    SECTION("Print") {
        CHECK( (new Mult(new Num(1), new Mult(new Num(2), new Num(3))))->to_string() == "(1*(2*3))" );
        //Multiplication with add expression on right
        CHECK( (new Mult(new Num(2), new Add(new Num(5), new Num(9))))->to_string() == "(2*(5+9))" );
        //Multiplication with add expression on left
        CHECK( (new Mult(new Add(new Num(3), new Num(9)), new Mult(new Num(7), new Num(8))))->to_string() == "((3+9)*(7*8))" );
        //Multiplication with two multiplication expressions
        CHECK( (new Mult(new Mult(new Num(6), new Num(3)), new Mult(new Num(1), new Num(4))))->to_string() == "((6*3)*(1*4))" );
        //Multiplication with two add expressions
        CHECK( (new Mult(new Add(new Num(9), new Num(2)), new Add(new Num(3), new Num(5))))->to_string() == "((9+2)*(3+5))" );
        //Multiplication with var
        CHECK( (new Mult(new Variable("x"), new Mult(new Num(2), new Num(3))))->to_string() == "(x*(2*3))" );
        //Multiplication with num and var
        CHECK( (new Mult(new Num(1), new Variable("y")))->to_string() == "(1*y)" );
        //Multiplication with var and add expression
        CHECK( (new Mult(new Variable("z"), new Add(new Num(4), new Num(9))))->to_string() == "(z*(4+9))" );
    }
    SECTION("Pretty Print") {
        //Associating to right examples
        CHECK((new Mult (new Num(2), new Mult (new Num(3), new Num(4))))->to_stringPP() == "2 * 3 * 4");
        CHECK((new Mult (new Mult (new Num(2), new Num(3)), new Num(4)))->to_stringPP() == "(2 * 3) * 4");
        //Multiplication with multiplication expression on left and add expression on right.
        CHECK( (new Mult( new Mult(new Num(7), new Num(7)), new Add(new Num(9), new Num(2))))->to_stringPP() == "(7 * 7) * (9 + 2)");
        //Multiplication with add expression on left and multiplication on right. Grouped by precedence example
        CHECK( (new Mult( new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_stringPP() == "(3 + 5) * 6 * 1");
        //Multiplication with var in left and multiplication expression on right.
        CHECK( (new Mult( new Variable("x"), new Mult(new Num(4), new Num(4))))->to_stringPP() == "x * 4 * 4");
        //Multiplication with multiplication expression on left and var on right. Association to the right example
        CHECK( (new Mult( new Mult(new Num(8), new Num(1)), new Variable("y")))->to_stringPP() == "(8 * 1) * y");
        //Multiplication with a num on left and an add expression on the right
        CHECK( (new Mult( new Num(1), new Add(new Num(2), new Num(3))))->to_stringPP() == "1 * (2 + 3)");
    }
}
TEST_CASE("Variable Class")
{
    SECTION("Equals") {

        CHECK((new Variable("x"))->equals(new Variable("x")) == true);
        CHECK((new Variable("x"))->equals(new Variable("y")) == false);
        //Empty string check
        CHECK((new Variable(""))->equals(new Variable("")) == true);
        //Palindrome
        CHECK((new Variable("racecar"))->equals(new Variable("racecar")) == true);
        CHECK((new Variable("Racecar"))->equals(new Variable("racecar")) == false);
        //Uppercase
        CHECK((new Variable("CLANG"))->equals(new Variable("CLANG")) == true);
        //Mixed case
        CHECK((new Variable("HeLlO"))->equals(new Variable("HeLlO")) == true);
        CHECK((new Variable("HeLlO"))->equals(new Variable("HeLLO")) == false);
    }
    SECTION("Interp") {

        CHECK_THROWS_WITH((new Variable("x"))->interp(), "Variable has no value");
        //Checking interp after a sub change
        CHECK((new Variable("z"))->subst("z", new Num(56))->interp() == 56);
    }
    SECTION("HasVariable") {

        CHECK((new Variable("x"))->has_variable() == true);
        //Doesn't have within num
        CHECK((new Num(4))->has_variable() == false);
        //Has within add
        CHECK((new Add(new Num(5), new Variable("6")))->has_variable() == true);
        //Doesn't have withing add
        CHECK((new Add(new Num(5), new Num(6)))->has_variable() == false);
        //Has within multiplication
        CHECK((new Mult(new Num(25), new Variable("6")))->has_variable() == true);
        //Doesn't have withing multiplication
        CHECK((new Mult(new Num(36), new Num(8)))->has_variable() == false);
    }
    SECTION("Substitution") {

        CHECK( (new Variable("x"))->subst("x", new Add(new Variable("y"),new Num(7)))->equals(new Add(new Variable("y"),new Num(7))) );
        //Sub within num
        CHECK((new Num(4))->subst("x", new Num(8))->equals(new Num(4)));
        //Sub within add
        CHECK((new Add(new Num(5), new Variable("6")))->subst("6", new Add(new Num(6), new Num(0)))
                      ->equals(new Add(new Num(5), new Add(new Num(6), new Num(0)))));
        //Sub within multiplication
        CHECK((new Mult(new Num(4), new Variable("3")))->subst("3", new Mult(new Num(7), new Num(2)))
                      ->equals(new Mult(new Num(4), new Mult(new Num(7), new Num(2)))));
    }
    SECTION("Print") {
        CHECK( (new Variable("x"))->to_string() == "x" );
        CHECK( (new Variable("-y"))->to_string() == "-y" );
        CHECK( (new Variable("777"))->to_string() == "777" );
        CHECK( (new Variable("HeLlO"))->to_string() == "HeLlO" );
        CHECK( (new Variable("RACECAR"))->to_string() == "RACECAR");


    }
    SECTION("Pretty Print") {
        CHECK( (new Variable("x"))->to_stringPP() == "x" );
        CHECK( (new Variable("-y"))->to_stringPP() == "-y" );
        CHECK( (new Variable("777"))->to_stringPP() == "777" );
        CHECK( (new Variable("HeLlO"))->to_stringPP() == "HeLlO" );
        CHECK( (new Variable("RACECAR"))->to_stringPP() == "RACECAR");

    }
}

