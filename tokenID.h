#ifndef TOKENID_H
#define TOKENID_H

enum error {
    WS_E     = -1, // whitespace is not a token
    ID_ERR   = -2, // ID cannot start with a number or a capital letter
    ASGN_ERR = -3, // unfished assignment (unrecongnized character after = )
    LTE_ERR  = -4, // unfinished less than or equal to 
    GRTE_ERR = -5, // unfinished greater than or equal to
    BANG_ERR = -6, // hanging ! 
    IC_ERR   = -7, // illegal character
};
enum state {
    // Transition States
    s1 = 0, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, last_state,
};

enum tokenID {
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
    COLNEQ_TK, // :=
    
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
    THEN_TK,
    PK_TK,
    DEC_TK,
    ASGN_KW_TK,
    PRC_TK,

    EOF_TK,
    LAST_TK, //for knowing how long the token id list is
     
};

#endif