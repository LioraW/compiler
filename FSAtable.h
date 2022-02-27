#ifndef FSATABLE_H
#define FSATABLE_H

#include "tokenID.h"
#include "Characters.h"

const int table[last_state][lastCharacter] = {
    // a        1         A         _         =          <         >         !         :         (         )         [         ]         {         }         ,         .         +         -         *           /        %       ;           WS
    { s2,       s3,       ID_ERR,   s5,       s6,        s7,       s8,       s9,       s10,      LPRN_TK,  RPRN_TK,  LBRC_TK,  RBRC_TK,  LCBRC_TK, RCBRC_TK, COMMA_TK, DOT_TK,   PLUS_TK,  MINUS_TK, MULT_TK,   DIV_TK,   MOD_TK,   SCOLN_TK, WS_E,     }, // s1 (from start)
    { s2,       s11,      s4,       s5,       ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    }, // s2 (from lower case letter) 
    { ID_ERR,   s3,       ID_ERR,   ID_ERR,   NUM_TK,    NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   s3,       NUM_TK,   NUM_TK,   NUM_TK,    NUM_TK,   NUM_TK,   NUM_TK,   NUM_TK,   }, // s3 (from a string of numbers)
    { s2,       s11,      s4,       s5,       ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    }, // s4 (from capital letter) 
    { s2,       s11,      s4,       s5,       ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    }, // s5 (from underscore)
    { ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  EQ_TK,     ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,   ASGN_TK,  ASGN_TK,  ASGN_TK,  ASGN_TK,  }, // s6 (from =) 
    { LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_TK,    LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,   LTE_ERR,  LTE_ERR,  LTE_ERR,  LTE_ERR,  }, // s7 (from <)  
    { GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_TK,   GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR,  GRTE_ERR, GRTE_ERR, GRTE_ERR, GRTE_ERR, }, // s8 (from >)
    { BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, NTEQ_TK,   BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR,  BANG_ERR, BANG_ERR, BANG_ERR, BANG_ERR, }, // s9 (from !)
    { COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLNEQ_TK, COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,   COLN_TK,  COLN_TK,  COLN_TK,  COLN_TK,  }, // s10 (from :)
    { s2,       s11,      s4,       s5,       ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,    ID_TK,     ID_TK,    ID_TK,    ID_TK,    ID_TK,    }, // s11 (from number after a letter or underscore) 
    
};

#endif
