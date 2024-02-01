//
// Created by Ben Baysinger  on 1/16/24.
//

#ifndef HOMEWORK1SMSDSCRIPT_EXPR_H
#define HOMEWORK1SMSDSCRIPT_EXPR_H

#include <iostream>
#include "catch.hpp"
#include <stdexcept>
#include <sstream>



typedef enum {
    prec_none = 0,
    prec_add = 1,
    prec_mult = 2
} precedence_t;


class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst( std::string valToSub, Expr* expr ) = 0;
    virtual void print( std::ostream &ostream) = 0;
    virtual void pretty_print( std::ostream  &ostream) = 0;
    std::string to_string();
    std::string to_stringPP();
    virtual void pretty_print_at( std::ostream  &ostream, precedence_t precedence) = 0;
};

class Num : public Expr {
public:
    int val;

   Num(int val);
   bool equals( Expr *comp );
   int interp();
   bool has_variable();
   Expr* subst( std::string valToSub, Expr* expr );
   void print( std::ostream &ostream);
   void pretty_print( std::ostream  &ostream);
   void pretty_print_at( std::ostream  &ostream, precedence_t precedence);

};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals( Expr *comp );
    int interp();
    bool has_variable();
    Expr* subst( std::string valToSub, Expr* expr );
    void print( std::ostream &ostream);
    void pretty_print( std::ostream  &ostream);
    void pretty_print_at( std::ostream  &ostream, precedence_t precedence);

};


class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Mult( Expr *lhs, Expr *rhs );
    bool equals( Expr *comp );
    int interp();
    bool has_variable();
    Expr* subst( std::string valToSub, Expr* expr );
    void print( std::ostream &ostream);
    void pretty_print( std::ostream  &ostream);
    void pretty_print_at( std::ostream  &ostream, precedence_t precedence);

};

class Variable : public Expr {
public:
    std::string value;

    Variable( std::string value);
    bool equals(Expr *comp);
    int interp();
    bool has_variable();
    Expr* subst( std::string valToSub, Expr* expr );
    void print( std::ostream &ostream);
    void pretty_print( std::ostream  &ostream);
    void pretty_print_at( std::ostream  &ostream, precedence_t precedence);
};


#endif //HOMEWORK1SMSDSCRIPT_EXPR_H

