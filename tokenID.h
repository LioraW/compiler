#ifndef TOEKNID_H
#define TOKENID_H

#include <string>
using namespace std;


enum tokenID {
    //error
    WS_E     = -6,   // whitespace is not a token
    ID_ERR   = -5, // ID cannot start with a number or a capital letter
    ASGN_ERR = -4, // unfished assignment (unrecongnized character after = )
    LTE_ERR  = -3, // unfinished less than or equal to 
    GRTE_ERR = -2, // unfinished greater than or equal to
    BANG_ERR = -1, // hanging ! 

    // Transition States
    s1 = 0, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, last_state,

    //TOKENS
    ID_TK = 1001,
    NUM_TK,
    
    //OPERATORS
    EQ_TK,   // == 
    NTEQ_TK, // !=
    GRTE_TK, // >=
    LTE_TK,  // <= 
    
    //ASSIGNMENT
    ASGN_TK, // =  
    COLN_TK,
    COLNEQ_TK,
    
    //MATH
    PLUS_TK,
    MINUS_TK,
    MULT_TK,
    DIV_TK,
    MOD_TK,
    DOT_TK,
    COMMA_TK,

    //GROUPS
    LPRN_TK,
    RPRN_TK,
    LCBRC_TK,
    RCBRC_TK,
    SCOLN_TK,
    LBRC_TK,
    RBRC_TK,
    
    //KEYWORDS
    STR_TK,
    STP_TK,
    RPT_TK,
    UNT_TK,
    WHL_TK,
    QT_TK,
    LBL_TK,
    LST_TK,
    YELL_TK,
    MN_TK,
    PTL_TK,
    IF_TK,
    PK_TK,
    DEC_TK,
    ASGN_KW_TK,
    PRC_TK,

    EOF_TK,
    LAST_TK, //for knowing how long the token id list is
     
};

string tokenNames[] = { 
    "ID",
    "Number",
    "Equality",
    "Not Equal",
    "Greater than Equal to",
    "Less than Equal to",
    "Assignment",
    "Colon",
    "Colon Equal",
    "Plus",
    "Minus",
    "Multiply",
    "Division",
    "Modulous",
    "Dot",
    "Comma",
    "Left Parentheses",
    "Right Parentheses",
    "Left Curly Bracket",
    "Right Curly Bracket",
    "Semicolon",
    "Left Bracket",
    "Right Bracket",
    "start",
    "stop",
    "repeat", 
    "until", 
    "whole", 
    "label", 
    "quit", 
    "listen", 
    "yell", 
    "main", 
    "portal", 
    "if", 
    "then", 
    "pick", 
    "declare", 
    "assign",
    "proc",
    "End of line",
    "Whitespace is not a token",
    "ID cannot start with a number or a capital letter",
    "unfished assignment (unrecongnized character after = )",
    "unfinished less than or equal to ",
    "unfinished greater than or equal to",
    "hanging !"

};

#endif