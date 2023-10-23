
// Generated from MQL_Lexer.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  MQL_Lexer : public antlr4::Lexer {
public:
  enum {
    K_ACYCLIC = 1, K_AND = 2, K_ANY = 3, K_AVG = 4, K_ALL = 5, K_ASC = 6, 
    K_BY = 7, K_BOOL = 8, K_COUNT = 9, K_DESCRIBE = 10, K_DESC = 11, K_DISTINCT = 12, 
    K_EDGE = 13, K_INCOMING = 14, K_INSERT = 15, K_INTEGER = 16, K_IS = 17, 
    K_FALSE = 18, K_FLOAT = 19, K_GROUP = 20, K_LABELS = 21, K_LABEL = 22, 
    K_LIMIT = 23, K_MAX = 24, K_MATCH = 25, K_MIN = 26, K_OPTIONAL = 27, 
    K_ORDER = 28, K_OR = 29, K_OUTGOING = 30, K_PROPERTIES = 31, K_PROPERTY = 32, 
    K_NOT = 33, K_NULL = 34, K_SHORTEST = 35, K_SIMPLE = 36, K_RETURN = 37, 
    K_SET = 38, K_SUM = 39, K_STRING = 40, K_TRUE = 41, K_TRAILS = 42, K_WALKS = 43, 
    K_WHERE = 44, TRUE_PROP = 45, FALSE_PROP = 46, ANON_ID = 47, EDGE_ID = 48, 
    KEY = 49, TYPE = 50, TYPE_VAR = 51, VARIABLE = 52, STRING = 53, UNSIGNED_INTEGER = 54, 
    UNSIGNED_FLOAT = 55, NAME = 56, LEQ = 57, GEQ = 58, EQ = 59, NEQ = 60, 
    LT = 61, GT = 62, SINGLE_EQ = 63, PATH_SEQUENCE = 64, PATH_ALTERNATIVE = 65, 
    PATH_NEGATION = 66, STAR = 67, PERCENT = 68, QUESTION_MARK = 69, PLUS = 70, 
    MINUS = 71, L_PAR = 72, R_PAR = 73, LCURLY_BRACKET = 74, RCURLY_BRACKET = 75, 
    LSQUARE_BRACKET = 76, RSQUARE_BRACKET = 77, COMMA = 78, COLON = 79, 
    WHITE_SPACE = 80, SINGLE_LINE_COMMENT = 81, UNRECOGNIZED = 82
  };

  enum {
    WS_CHANNEL = 2
  };

  explicit MQL_Lexer(antlr4::CharStream *input);
  ~MQL_Lexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

