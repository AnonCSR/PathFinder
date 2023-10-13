
// Generated from MQL_Parser.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  MQL_Parser : public antlr4::Parser {
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
    RuleRoot = 0, RuleMatchQuery = 1, RuleDescribeQuery = 2, RuleDescribeFlag = 3, 
    RuleInsertQuery = 4, RuleInsertLabelList = 5, RuleInsertPropertyList = 6, 
    RuleInsertEdgeList = 7, RuleInsertLabelElement = 8, RuleInsertPropertyElement = 9, 
    RuleInsertEdgeElement = 10, RuleSetStatement = 11, RuleMatchStatement = 12, 
    RuleWhereStatement = 13, RuleGroupByStatement = 14, RuleOrderByStatement = 15, 
    RuleReturnStatement = 16, RuleSetItem = 17, RuleReturnItem = 18, RuleAggregateFunc = 19, 
    RuleOrderByItem = 20, RuleGroupByItem = 21, RuleGraphPattern = 22, RuleOptionalPattern = 23, 
    RuleBasicPattern = 24, RuleLinearPattern = 25, RulePath = 26, RulePathAlternatives = 27, 
    RulePathSequence = 28, RulePathAtom = 29, RulePathSuffix = 30, RulePathType = 31, 
    RuleNode = 32, RuleFixedNode = 33, RuleFixedNodeInside = 34, RuleEdge = 35, 
    RuleEdgeInside = 36, RuleVarNode = 37, RuleProperties = 38, RuleProperty = 39, 
    RuleIdentifier = 40, RuleBoolValue = 41, RuleNumericValue = 42, RuleValue = 43, 
    RuleConditionalOrExpr = 44, RuleConditionalAndExpr = 45, RuleComparisonExpr = 46, 
    RuleAdditiveExpr = 47, RuleMultiplicativeExpr = 48, RuleUnaryExpr = 49, 
    RuleAtomicExpr = 50, RuleValueExpr = 51, RuleExprTypename = 52, RuleKeyword = 53
  };

  explicit MQL_Parser(antlr4::TokenStream *input);
  ~MQL_Parser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class RootContext;
  class MatchQueryContext;
  class DescribeQueryContext;
  class DescribeFlagContext;
  class InsertQueryContext;
  class InsertLabelListContext;
  class InsertPropertyListContext;
  class InsertEdgeListContext;
  class InsertLabelElementContext;
  class InsertPropertyElementContext;
  class InsertEdgeElementContext;
  class SetStatementContext;
  class MatchStatementContext;
  class WhereStatementContext;
  class GroupByStatementContext;
  class OrderByStatementContext;
  class ReturnStatementContext;
  class SetItemContext;
  class ReturnItemContext;
  class AggregateFuncContext;
  class OrderByItemContext;
  class GroupByItemContext;
  class GraphPatternContext;
  class OptionalPatternContext;
  class BasicPatternContext;
  class LinearPatternContext;
  class PathContext;
  class PathAlternativesContext;
  class PathSequenceContext;
  class PathAtomContext;
  class PathSuffixContext;
  class PathTypeContext;
  class NodeContext;
  class FixedNodeContext;
  class FixedNodeInsideContext;
  class EdgeContext;
  class EdgeInsideContext;
  class VarNodeContext;
  class PropertiesContext;
  class PropertyContext;
  class IdentifierContext;
  class BoolValueContext;
  class NumericValueContext;
  class ValueContext;
  class ConditionalOrExprContext;
  class ConditionalAndExprContext;
  class ComparisonExprContext;
  class AdditiveExprContext;
  class MultiplicativeExprContext;
  class UnaryExprContext;
  class AtomicExprContext;
  class ValueExprContext;
  class ExprTypenameContext;
  class KeywordContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    MatchQueryContext *matchQuery();
    DescribeQueryContext *describeQuery();
    InsertQueryContext *insertQuery();
    SetStatementContext *setStatement();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  MatchQueryContext : public antlr4::ParserRuleContext {
  public:
    MatchQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MatchStatementContext *matchStatement();
    ReturnStatementContext *returnStatement();
    WhereStatementContext *whereStatement();
    GroupByStatementContext *groupByStatement();
    OrderByStatementContext *orderByStatement();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchQueryContext* matchQuery();

  class  DescribeQueryContext : public antlr4::ParserRuleContext {
  public:
    DescribeQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_DESCRIBE();
    FixedNodeInsideContext *fixedNodeInside();
    std::vector<DescribeFlagContext *> describeFlag();
    DescribeFlagContext* describeFlag(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DescribeQueryContext* describeQuery();

  class  DescribeFlagContext : public antlr4::ParserRuleContext {
  public:
    DescribeFlagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_LABELS();
    antlr4::tree::TerminalNode *K_PROPERTIES();
    antlr4::tree::TerminalNode *K_OUTGOING();
    antlr4::tree::TerminalNode *K_INCOMING();
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DescribeFlagContext* describeFlag();

  class  InsertQueryContext : public antlr4::ParserRuleContext {
  public:
    InsertQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_INSERT();
    InsertLabelListContext *insertLabelList();
    InsertPropertyListContext *insertPropertyList();
    InsertEdgeListContext *insertEdgeList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertQueryContext* insertQuery();

  class  InsertLabelListContext : public antlr4::ParserRuleContext {
  public:
    InsertLabelListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_LABEL();
    std::vector<InsertLabelElementContext *> insertLabelElement();
    InsertLabelElementContext* insertLabelElement(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertLabelListContext* insertLabelList();

  class  InsertPropertyListContext : public antlr4::ParserRuleContext {
  public:
    InsertPropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_PROPERTY();
    std::vector<InsertPropertyElementContext *> insertPropertyElement();
    InsertPropertyElementContext* insertPropertyElement(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPropertyListContext* insertPropertyList();

  class  InsertEdgeListContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_EDGE();
    std::vector<InsertEdgeElementContext *> insertEdgeElement();
    InsertEdgeElementContext* insertEdgeElement(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgeListContext* insertEdgeList();

  class  InsertLabelElementContext : public antlr4::ParserRuleContext {
  public:
    InsertLabelElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *R_PAR();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ANON_ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertLabelElementContext* insertLabelElement();

  class  InsertPropertyElementContext : public antlr4::ParserRuleContext {
  public:
    InsertPropertyElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    FixedNodeInsideContext *fixedNodeInside();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *STRING();
    ValueContext *value();
    antlr4::tree::TerminalNode *R_PAR();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPropertyElementContext* insertPropertyElement();

  class  InsertEdgeElementContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgeElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<FixedNodeInsideContext *> fixedNodeInside();
    FixedNodeInsideContext* fixedNodeInside(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *R_PAR();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgeElementContext* insertEdgeElement();

  class  SetStatementContext : public antlr4::ParserRuleContext {
  public:
    SetStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SET();
    std::vector<SetItemContext *> setItem();
    SetItemContext* setItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetStatementContext* setStatement();

  class  MatchStatementContext : public antlr4::ParserRuleContext {
  public:
    MatchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_MATCH();
    GraphPatternContext *graphPattern();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchStatementContext* matchStatement();

  class  WhereStatementContext : public antlr4::ParserRuleContext {
  public:
    WhereStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_WHERE();
    ConditionalOrExprContext *conditionalOrExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhereStatementContext* whereStatement();

  class  GroupByStatementContext : public antlr4::ParserRuleContext {
  public:
    GroupByStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_GROUP();
    antlr4::tree::TerminalNode *K_BY();
    std::vector<GroupByItemContext *> groupByItem();
    GroupByItemContext* groupByItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByStatementContext* groupByStatement();

  class  OrderByStatementContext : public antlr4::ParserRuleContext {
  public:
    OrderByStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ORDER();
    antlr4::tree::TerminalNode *K_BY();
    std::vector<OrderByItemContext *> orderByItem();
    OrderByItemContext* orderByItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByStatementContext* orderByStatement();

  class  ReturnStatementContext : public antlr4::ParserRuleContext {
  public:
    ReturnStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ReturnStatementContext() = default;
    void copyFrom(ReturnStatementContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnAllContext : public ReturnStatementContext {
  public:
    ReturnAllContext(ReturnStatementContext *ctx);

    antlr4::tree::TerminalNode *K_RETURN();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnListContext : public ReturnStatementContext {
  public:
    ReturnListContext(ReturnStatementContext *ctx);

    antlr4::tree::TerminalNode *K_RETURN();
    std::vector<ReturnItemContext *> returnItem();
    ReturnItemContext* returnItem(size_t i);
    antlr4::tree::TerminalNode *K_DISTINCT();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnStatementContext* returnStatement();

  class  SetItemContext : public antlr4::ParserRuleContext {
  public:
    SetItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *SINGLE_EQ();
    FixedNodeInsideContext *fixedNodeInside();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetItemContext* setItem();

  class  ReturnItemContext : public antlr4::ParserRuleContext {
  public:
    ReturnItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ReturnItemContext() = default;
    void copyFrom(ReturnItemContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnItemVarContext : public ReturnItemContext {
  public:
    ReturnItemVarContext(ReturnItemContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnItemAggContext : public ReturnItemContext {
  public:
    ReturnItemAggContext(ReturnItemContext *ctx);

    AggregateFuncContext *aggregateFunc();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *KEY();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnItemCountContext : public ReturnItemContext {
  public:
    ReturnItemCountContext(ReturnItemContext *ctx);

    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *KEY();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnItemContext* returnItem();

  class  AggregateFuncContext : public antlr4::ParserRuleContext {
  public:
    AggregateFuncContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SUM();
    antlr4::tree::TerminalNode *K_MAX();
    antlr4::tree::TerminalNode *K_MIN();
    antlr4::tree::TerminalNode *K_AVG();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AggregateFuncContext* aggregateFunc();

  class  OrderByItemContext : public antlr4::ParserRuleContext {
  public:
    OrderByItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    OrderByItemContext() = default;
    void copyFrom(OrderByItemContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  OrderByItemCountContext : public OrderByItemContext {
  public:
    OrderByItemCountContext(OrderByItemContext *ctx);

    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrderByItemVarContext : public OrderByItemContext {
  public:
    OrderByItemVarContext(OrderByItemContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrderByItemAggContext : public OrderByItemContext {
  public:
    OrderByItemAggContext(OrderByItemContext *ctx);

    AggregateFuncContext *aggregateFunc();
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_DESC();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  OrderByItemContext* orderByItem();

  class  GroupByItemContext : public antlr4::ParserRuleContext {
  public:
    GroupByItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByItemContext* groupByItem();

  class  GraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BasicPatternContext *basicPattern();
    std::vector<OptionalPatternContext *> optionalPattern();
    OptionalPatternContext* optionalPattern(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternContext* graphPattern();

  class  OptionalPatternContext : public antlr4::ParserRuleContext {
  public:
    OptionalPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_OPTIONAL();
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    GraphPatternContext *graphPattern();
    antlr4::tree::TerminalNode *RCURLY_BRACKET();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalPatternContext* optionalPattern();

  class  BasicPatternContext : public antlr4::ParserRuleContext {
  public:
    BasicPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LinearPatternContext *> linearPattern();
    LinearPatternContext* linearPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BasicPatternContext* basicPattern();

  class  LinearPatternContext : public antlr4::ParserRuleContext {
  public:
    LinearPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NodeContext *> node();
    NodeContext* node(size_t i);
    std::vector<EdgeContext *> edge();
    EdgeContext* edge(size_t i);
    std::vector<PathContext *> path();
    PathContext* path(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearPatternContext* linearPattern();

  class  PathContext : public antlr4::ParserRuleContext {
  public:
    PathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    PathAlternativesContext *pathAlternatives();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> SINGLE_EQ();
    antlr4::tree::TerminalNode* SINGLE_EQ(size_t i);
    PathTypeContext *pathType();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *GT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathContext* path();

  class  PathAlternativesContext : public antlr4::ParserRuleContext {
  public:
    PathAlternativesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathSequenceContext *> pathSequence();
    PathSequenceContext* pathSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_ALTERNATIVE();
    antlr4::tree::TerminalNode* PATH_ALTERNATIVE(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathAlternativesContext* pathAlternatives();

  class  PathSequenceContext : public antlr4::ParserRuleContext {
  public:
    PathSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathAtomContext *> pathAtom();
    PathAtomContext* pathAtom(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_SEQUENCE();
    antlr4::tree::TerminalNode* PATH_SEQUENCE(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSequenceContext* pathSequence();

  class  PathAtomContext : public antlr4::ParserRuleContext {
  public:
    PathAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PathAtomContext() = default;
    void copyFrom(PathAtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  PathAtomAlternativesContext : public PathAtomContext {
  public:
    PathAtomAlternativesContext(PathAtomContext *ctx);

    antlr4::tree::TerminalNode *L_PAR();
    PathAlternativesContext *pathAlternatives();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *PATH_NEGATION();
    PathSuffixContext *pathSuffix();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PathAtomSimpleContext : public PathAtomContext {
  public:
    PathAtomSimpleContext(PathAtomContext *ctx);

    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *PATH_NEGATION();
    PathSuffixContext *pathSuffix();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PathAtomContext* pathAtom();

  class  PathSuffixContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    antlr4::Token *min = nullptr;
    antlr4::Token *max = nullptr;
    PathSuffixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *QUESTION_MARK();
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode* UNSIGNED_INTEGER(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSuffixContext* pathSuffix();

  class  PathTypeContext : public antlr4::ParserRuleContext {
  public:
    PathTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ANY();
    antlr4::tree::TerminalNode *K_ALL();
    antlr4::tree::TerminalNode *K_SHORTEST();
    antlr4::tree::TerminalNode *K_WALKS();
    antlr4::tree::TerminalNode *K_SIMPLE();
    antlr4::tree::TerminalNode *K_ACYCLIC();
    antlr4::tree::TerminalNode *K_TRAILS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathTypeContext* pathType();

  class  NodeContext : public antlr4::ParserRuleContext {
  public:
    NodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FixedNodeContext *fixedNode();
    VarNodeContext *varNode();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeContext* node();

  class  FixedNodeContext : public antlr4::ParserRuleContext {
  public:
    FixedNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    FixedNodeInsideContext *fixedNodeInside();
    antlr4::tree::TerminalNode *R_PAR();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedNodeContext* fixedNode();

  class  FixedNodeInsideContext : public antlr4::ParserRuleContext {
  public:
    FixedNodeInsideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ANON_ID();
    antlr4::tree::TerminalNode *EDGE_ID();
    ValueContext *value();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedNodeInsideContext* fixedNodeInside();

  class  EdgeContext : public antlr4::ParserRuleContext {
  public:
    EdgeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LT();
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);
    EdgeInsideContext *edgeInside();
    antlr4::tree::TerminalNode *GT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeContext* edge();

  class  EdgeInsideContext : public antlr4::ParserRuleContext {
  public:
    EdgeInsideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE_BRACKET();
    antlr4::tree::TerminalNode *RSQUARE_BRACKET();
    PropertiesContext *properties();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *EDGE_ID();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPE_VAR();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeInsideContext* edgeInside();

  class  VarNodeContext : public antlr4::ParserRuleContext {
  public:
    VarNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAR();
    antlr4::tree::TerminalNode *R_PAR();
    antlr4::tree::TerminalNode *VARIABLE();
    std::vector<antlr4::tree::TerminalNode *> TYPE();
    antlr4::tree::TerminalNode* TYPE(size_t i);
    PropertiesContext *properties();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarNodeContext* varNode();

  class  PropertiesContext : public antlr4::ParserRuleContext {
  public:
    PropertiesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LCURLY_BRACKET();
    std::vector<PropertyContext *> property();
    PropertyContext* property(size_t i);
    antlr4::tree::TerminalNode *RCURLY_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertiesContext* properties();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    antlr4::tree::TerminalNode *TRUE_PROP();
    antlr4::tree::TerminalNode *FALSE_PROP();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyContext* property();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NAME();
    KeywordContext *keyword();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  BoolValueContext : public antlr4::ParserRuleContext {
  public:
    BoolValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BoolValueContext* boolValue();

  class  NumericValueContext : public antlr4::ParserRuleContext {
  public:
    NumericValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_FLOAT();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericValueContext* numericValue();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumericValueContext *numericValue();
    antlr4::tree::TerminalNode *STRING();
    BoolValueContext *boolValue();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  ConditionalOrExprContext : public antlr4::ParserRuleContext {
  public:
    ConditionalOrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConditionalAndExprContext *> conditionalAndExpr();
    ConditionalAndExprContext* conditionalAndExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> K_OR();
    antlr4::tree::TerminalNode* K_OR(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalOrExprContext* conditionalOrExpr();

  class  ConditionalAndExprContext : public antlr4::ParserRuleContext {
  public:
    ConditionalAndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComparisonExprContext *> comparisonExpr();
    ComparisonExprContext* comparisonExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> K_AND();
    antlr4::tree::TerminalNode* K_AND(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalAndExprContext* conditionalAndExpr();

  class  ComparisonExprContext : public antlr4::ParserRuleContext {
  public:
    ComparisonExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ComparisonExprContext() = default;
    void copyFrom(ComparisonExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ComparisonExprIsContext : public ComparisonExprContext {
  public:
    ComparisonExprIsContext(ComparisonExprContext *ctx);

    AdditiveExprContext *additiveExpr();
    antlr4::tree::TerminalNode *K_IS();
    ExprTypenameContext *exprTypename();
    antlr4::tree::TerminalNode *K_NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ComparisonExprOpContext : public ComparisonExprContext {
  public:
    ComparisonExprOpContext(ComparisonExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<AdditiveExprContext *> additiveExpr();
    AdditiveExprContext* additiveExpr(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *GEQ();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ComparisonExprContext* comparisonExpr();

  class  AdditiveExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s70 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s71 = nullptr;
    antlr4::Token *_tset1006 = nullptr;
    AdditiveExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MultiplicativeExprContext *> multiplicativeExpr();
    MultiplicativeExprContext* multiplicativeExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveExprContext* additiveExpr();

  class  MultiplicativeExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *s67 = nullptr;
    std::vector<antlr4::Token *> op;
    antlr4::Token *s64 = nullptr;
    antlr4::Token *s68 = nullptr;
    antlr4::Token *_tset1025 = nullptr;
    MultiplicativeExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnaryExprContext *> unaryExpr();
    UnaryExprContext* unaryExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STAR();
    antlr4::tree::TerminalNode* STAR(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATH_SEQUENCE();
    antlr4::tree::TerminalNode* PATH_SEQUENCE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PERCENT();
    antlr4::tree::TerminalNode* PERCENT(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeExprContext* multiplicativeExpr();

  class  UnaryExprContext : public antlr4::ParserRuleContext {
  public:
    UnaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NOT();
    UnaryExprContext *unaryExpr();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    AtomicExprContext *atomicExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExprContext* unaryExpr();

  class  AtomicExprContext : public antlr4::ParserRuleContext {
  public:
    AtomicExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtomicExprContext() = default;
    void copyFrom(AtomicExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExprValueExprContext : public AtomicExprContext {
  public:
    ExprValueExprContext(AtomicExprContext *ctx);

    ValueExprContext *valueExpr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprVarContext : public AtomicExprContext {
  public:
    ExprVarContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *KEY();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprParenthesisContext : public AtomicExprContext {
  public:
    ExprParenthesisContext(AtomicExprContext *ctx);

    antlr4::tree::TerminalNode *L_PAR();
    ConditionalOrExprContext *conditionalOrExpr();
    antlr4::tree::TerminalNode *R_PAR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtomicExprContext* atomicExpr();

  class  ValueExprContext : public antlr4::ParserRuleContext {
  public:
    ValueExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_FLOAT();
    antlr4::tree::TerminalNode *STRING();
    BoolValueContext *boolValue();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueExprContext* valueExpr();

  class  ExprTypenameContext : public antlr4::ParserRuleContext {
  public:
    ExprTypenameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NULL();
    antlr4::tree::TerminalNode *K_STRING();
    antlr4::tree::TerminalNode *K_BOOL();
    antlr4::tree::TerminalNode *K_INTEGER();
    antlr4::tree::TerminalNode *K_FLOAT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprTypenameContext* exprTypename();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ACYCLIC();
    antlr4::tree::TerminalNode *K_AND();
    antlr4::tree::TerminalNode *K_ANY();
    antlr4::tree::TerminalNode *K_ALL();
    antlr4::tree::TerminalNode *K_AVG();
    antlr4::tree::TerminalNode *K_ASC();
    antlr4::tree::TerminalNode *K_BY();
    antlr4::tree::TerminalNode *K_BOOL();
    antlr4::tree::TerminalNode *K_COUNT();
    antlr4::tree::TerminalNode *K_DESCRIBE();
    antlr4::tree::TerminalNode *K_DESC();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *K_EDGE();
    antlr4::tree::TerminalNode *K_INCOMING();
    antlr4::tree::TerminalNode *K_INSERT();
    antlr4::tree::TerminalNode *K_INTEGER();
    antlr4::tree::TerminalNode *K_IS();
    antlr4::tree::TerminalNode *K_FLOAT();
    antlr4::tree::TerminalNode *K_GROUP();
    antlr4::tree::TerminalNode *K_LABELS();
    antlr4::tree::TerminalNode *K_LABEL();
    antlr4::tree::TerminalNode *K_LIMIT();
    antlr4::tree::TerminalNode *K_MAX();
    antlr4::tree::TerminalNode *K_MATCH();
    antlr4::tree::TerminalNode *K_MIN();
    antlr4::tree::TerminalNode *K_OPTIONAL();
    antlr4::tree::TerminalNode *K_ORDER();
    antlr4::tree::TerminalNode *K_OR();
    antlr4::tree::TerminalNode *K_OUTGOING();
    antlr4::tree::TerminalNode *K_PROPERTIES();
    antlr4::tree::TerminalNode *K_PROPERTY();
    antlr4::tree::TerminalNode *K_NOT();
    antlr4::tree::TerminalNode *K_NULL();
    antlr4::tree::TerminalNode *K_SHORTEST();
    antlr4::tree::TerminalNode *K_SIMPLE();
    antlr4::tree::TerminalNode *K_RETURN();
    antlr4::tree::TerminalNode *K_SET();
    antlr4::tree::TerminalNode *K_SUM();
    antlr4::tree::TerminalNode *K_STRING();
    antlr4::tree::TerminalNode *K_TRAILS();
    antlr4::tree::TerminalNode *K_WALKS();
    antlr4::tree::TerminalNode *K_WHERE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeywordContext* keyword();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

