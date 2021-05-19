// A Bison parser, made by GNU Bison 3.7.6.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hpp"


// Unqualified %code blocks.
#line 33 "lib/parser.yy"
 
	#include <iostream>
	#include <vector>

	namespace yy { 
		Parser::symbol_type yylex(Scanner &scanner);
	}

	int ctx = 0;
	Node ast;
	std::vector<Error> error_vector = std::vector<Error>();
	std::string cur_clazz = "Object";

	std::vector<std::vector<Node>> fields_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> methods_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> formals_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> block_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> args_vec = {std::vector<Node>()};

#line 66 "lib/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 158 "lib/parser.cpp"

  /// Build a parser object.
   Parser :: Parser  (Scanner &scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg)
  {}

   Parser ::~ Parser  ()
  {}

   Parser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
   Parser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   Parser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   Parser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   Parser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   Parser ::symbol_kind_type
   Parser ::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

   Parser ::stack_symbol_type::stack_symbol_type ()
  {}

   Parser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_clazz: // clazz
      case symbol_kind::S_field: // field
      case symbol_kind::S_method: // method
      case symbol_kind::S_form: // form
      case symbol_kind::S_block: // block
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_literal: // literal
        value.YY_MOVE_OR_COPY< Node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_LIT: // INTEGER_LIT
      case symbol_kind::S_STRING_LIT: // STRING_LIT
      case symbol_kind::S_TYPE_ID: // TYPE_ID
      case symbol_kind::S_OBJ_ID: // OBJ_ID
      case symbol_kind::S_type: // type
      case symbol_kind::S_boolean: // boolean
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

   Parser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_clazz: // clazz
      case symbol_kind::S_field: // field
      case symbol_kind::S_method: // method
      case symbol_kind::S_form: // form
      case symbol_kind::S_block: // block
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_literal: // literal
        value.move< Node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_LIT: // INTEGER_LIT
      case symbol_kind::S_STRING_LIT: // STRING_LIT
      case symbol_kind::S_TYPE_ID: // TYPE_ID
      case symbol_kind::S_OBJ_ID: // OBJ_ID
      case symbol_kind::S_type: // type
      case symbol_kind::S_boolean: // boolean
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_clazz: // clazz
      case symbol_kind::S_field: // field
      case symbol_kind::S_method: // method
      case symbol_kind::S_form: // form
      case symbol_kind::S_block: // block
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_literal: // literal
        value.copy< Node > (that.value);
        break;

      case symbol_kind::S_INTEGER_LIT: // INTEGER_LIT
      case symbol_kind::S_STRING_LIT: // STRING_LIT
      case symbol_kind::S_TYPE_ID: // TYPE_ID
      case symbol_kind::S_OBJ_ID: // OBJ_ID
      case symbol_kind::S_type: // type
      case symbol_kind::S_boolean: // boolean
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_clazz: // clazz
      case symbol_kind::S_field: // field
      case symbol_kind::S_method: // method
      case symbol_kind::S_form: // form
      case symbol_kind::S_block: // block
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_literal: // literal
        value.move< Node > (that.value);
        break;

      case symbol_kind::S_INTEGER_LIT: // INTEGER_LIT
      case symbol_kind::S_STRING_LIT: // STRING_LIT
      case symbol_kind::S_TYPE_ID: // TYPE_ID
      case symbol_kind::S_OBJ_ID: // OBJ_ID
      case symbol_kind::S_type: // type
      case symbol_kind::S_boolean: // boolean
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
   Parser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   Parser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   Parser ::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

   Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::operator() ()
  {
    return parse ();
  }

  int
   Parser ::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (scanner));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_clazz: // clazz
      case symbol_kind::S_field: // field
      case symbol_kind::S_method: // method
      case symbol_kind::S_form: // form
      case symbol_kind::S_block: // block
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_literal: // literal
        yylhs.value.emplace< Node > ();
        break;

      case symbol_kind::S_INTEGER_LIT: // INTEGER_LIT
      case symbol_kind::S_STRING_LIT: // STRING_LIT
      case symbol_kind::S_TYPE_ID: // TYPE_ID
      case symbol_kind::S_OBJ_ID: // OBJ_ID
      case symbol_kind::S_type: // type
      case symbol_kind::S_boolean: // boolean
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: clazz
#line 128 "lib/parser.yy"
                              { ast = Node::create_program(1, 1, {yystack_[0].value.as < Node > ()}); }
#line 647 "lib/parser.cpp"
    break;

  case 3: // program: program clazz
#line 129 "lib/parser.yy"
                                              { ast.add_clazz(yystack_[0].value.as < Node > ()); }
#line 653 "lib/parser.cpp"
    break;

  case 4: // $@1: %empty
#line 131 "lib/parser.yy"
                                        { cur_clazz = yystack_[0].value.as < std::string > (); }
#line 659 "lib/parser.cpp"
    break;

  case 5: // clazz: "class" TYPE_ID $@1 clazzbody
#line 131 "lib/parser.yy"
                                                                      { yylhs.value.as < Node > () = Node::create_clazz(yystack_[3].location.begin.line, yystack_[3].location.begin.column, yystack_[2].value.as < std::string > (), "Object", fields_vec[ctx], methods_vec[ctx]); fields_vec[ctx].clear(); methods_vec[ctx].clear(); }
#line 665 "lib/parser.cpp"
    break;

  case 6: // $@2: %empty
#line 132 "lib/parser.yy"
                                                                  { cur_clazz = yystack_[2].value.as < std::string > (); }
#line 671 "lib/parser.cpp"
    break;

  case 7: // clazz: "class" TYPE_ID "extends" TYPE_ID $@2 clazzbody
#line 132 "lib/parser.yy"
                                                                                                { yylhs.value.as < Node > () = Node::create_clazz(yystack_[5].location.begin.line, yystack_[5].location.begin.column, yystack_[4].value.as < std::string > (), yystack_[2].value.as < std::string > (), fields_vec[ctx], methods_vec[ctx]); fields_vec[ctx].clear(); methods_vec[ctx].clear(); }
#line 677 "lib/parser.cpp"
    break;

  case 10: // body: body field
#line 137 "lib/parser.yy"
                                           { fields_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 683 "lib/parser.cpp"
    break;

  case 11: // body: body method
#line 138 "lib/parser.yy"
                                            { methods_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 689 "lib/parser.cpp"
    break;

  case 12: // field: OBJ_ID ":" type ";"
#line 140 "lib/parser.yy"
                                            { yylhs.value.as < Node > () = Node::create_field(yystack_[3].location.begin.line, yystack_[3].location.begin.column, yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::string > ()); }
#line 695 "lib/parser.cpp"
    break;

  case 13: // field: OBJ_ID ":" type "<-" expr ";"
#line 141 "lib/parser.yy"
                                                              { yylhs.value.as < Node > () = Node::create_field(yystack_[5].location.begin.line, yystack_[5].location.begin.column, yystack_[5].value.as < std::string > (), yystack_[3].value.as < std::string > (), yystack_[1].value.as < Node > ()); }
#line 701 "lib/parser.cpp"
    break;

  case 14: // method: OBJ_ID "(" formals ")" ":" type block
#line 143 "lib/parser.yy"
                                                              { yylhs.value.as < Node > () = Node::create_method(yystack_[6].location.begin.line, yystack_[6].location.begin.column, yystack_[6].value.as < std::string > (), formals_vec[ctx], yystack_[1].value.as < std::string > (), yystack_[0].value.as < Node > ()); formals_vec[ctx].clear(); }
#line 707 "lib/parser.cpp"
    break;

  case 15: // type: TYPE_ID
#line 145 "lib/parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 713 "lib/parser.cpp"
    break;

  case 16: // type: "int32"
#line 146 "lib/parser.yy"
                                        { yylhs.value.as < std::string > () = "int32"; }
#line 719 "lib/parser.cpp"
    break;

  case 17: // type: "bool"
#line 147 "lib/parser.yy"
                                       { yylhs.value.as < std::string > () = "bool"; }
#line 725 "lib/parser.cpp"
    break;

  case 18: // type: "string"
#line 148 "lib/parser.yy"
                                         { yylhs.value.as < std::string > () = "string"; }
#line 731 "lib/parser.cpp"
    break;

  case 19: // type: "unit"
#line 149 "lib/parser.yy"
                                       { yylhs.value.as < std::string > () = "unit"; }
#line 737 "lib/parser.cpp"
    break;

  case 22: // formal: form
#line 154 "lib/parser.yy"
                             { formals_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 743 "lib/parser.cpp"
    break;

  case 23: // formal: formal "," form
#line 155 "lib/parser.yy"
                                                { formals_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 749 "lib/parser.cpp"
    break;

  case 24: // form: OBJ_ID ":" type
#line 157 "lib/parser.yy"
                                        {yylhs.value.as < Node > () = Node::create_formal(yystack_[2].location.begin.line, yystack_[2].location.begin.column, yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 755 "lib/parser.cpp"
    break;

  case 25: // $@3: %empty
#line 159 "lib/parser.yy"
                            { push_ctx(); }
#line 761 "lib/parser.cpp"
    break;

  case 26: // block: "{" $@3 exprs "}"
#line 159 "lib/parser.yy"
                                                      { yylhs.value.as < Node > () = Node::create_block(yystack_[3].location.begin.line, yystack_[3].location.begin.column, block_vec[ctx]); pop_ctx(); }
#line 767 "lib/parser.cpp"
    break;

  case 27: // exprs: expr
#line 161 "lib/parser.yy"
                             { block_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 773 "lib/parser.cpp"
    break;

  case 28: // exprs: exprs ";" expr
#line 162 "lib/parser.yy"
                                               { block_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 779 "lib/parser.cpp"
    break;

  case 29: // expr: "if" expr "then" expr
#line 164 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_if_expr(yystack_[3].location.begin.line, yystack_[3].location.begin.column, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 785 "lib/parser.cpp"
    break;

  case 30: // expr: "if" expr "then" expr "else" expr
#line 165 "lib/parser.yy"
                                                          { yylhs.value.as < Node > () = Node::create_if_expr(yystack_[5].location.begin.line, yystack_[5].location.begin.column, yystack_[4].value.as < Node > (), yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 791 "lib/parser.cpp"
    break;

  case 31: // expr: "while" expr "do" expr
#line 166 "lib/parser.yy"
                                                       { yylhs.value.as < Node > () = Node::create_while_expr(yystack_[3].location.begin.line, yystack_[3].location.begin.column, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 797 "lib/parser.cpp"
    break;

  case 32: // expr: "let" OBJ_ID ":" type "in" expr
#line 167 "lib/parser.yy"
                                                                { yylhs.value.as < Node > () = Node::create_let_expr(yystack_[5].location.begin.line, yystack_[5].location.begin.column, yystack_[4].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[0].value.as < Node > ()); }
#line 803 "lib/parser.cpp"
    break;

  case 33: // expr: "let" OBJ_ID ":" type "<-" expr "in" expr
#line 168 "lib/parser.yy"
                                                                          { yylhs.value.as < Node > () = Node::create_let_expr(yystack_[7].location.begin.line, yystack_[7].location.begin.column, yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[0].value.as < Node > (), yystack_[2].value.as < Node > ()); }
#line 809 "lib/parser.cpp"
    break;

  case 34: // expr: OBJ_ID "<-" expr
#line 169 "lib/parser.yy"
                                                 { yylhs.value.as < Node > () = Node::create_assign_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, yystack_[2].value.as < std::string > (), yystack_[0].value.as < Node > ()); }
#line 815 "lib/parser.cpp"
    break;

  case 35: // expr: "not" expr
#line 170 "lib/parser.yy"
                                           { yylhs.value.as < Node > () = Node::create_unop_expr(yystack_[1].location.begin.line, yystack_[1].location.begin.column, "not", yystack_[0].value.as < Node > ()); }
#line 821 "lib/parser.cpp"
    break;

  case 36: // expr: "-" expr
#line 171 "lib/parser.yy"
                         { yylhs.value.as < Node > () = Node::create_unop_expr(yystack_[1].location.begin.line, yystack_[1].location.begin.column, "-", yystack_[0].value.as < Node > ()); }
#line 827 "lib/parser.cpp"
    break;

  case 37: // expr: "isnull" expr
#line 172 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_unop_expr(yystack_[1].location.begin.line, yystack_[1].location.begin.column, "isnull", yystack_[0].value.as < Node > ()); }
#line 833 "lib/parser.cpp"
    break;

  case 38: // expr: expr "and" expr
#line 173 "lib/parser.yy"
                                                { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "and", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 839 "lib/parser.cpp"
    break;

  case 39: // expr: expr "=" expr
#line 174 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "=", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 845 "lib/parser.cpp"
    break;

  case 40: // expr: expr "<" expr
#line 175 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "<", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 851 "lib/parser.cpp"
    break;

  case 41: // expr: expr "<=" expr
#line 176 "lib/parser.yy"
                                               { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "<=", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 857 "lib/parser.cpp"
    break;

  case 42: // expr: expr "+" expr
#line 177 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "+", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 863 "lib/parser.cpp"
    break;

  case 43: // expr: expr "-" expr
#line 178 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "-", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 869 "lib/parser.cpp"
    break;

  case 44: // expr: expr "*" expr
#line 179 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "*", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 875 "lib/parser.cpp"
    break;

  case 45: // expr: expr "/" expr
#line 180 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "/", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 881 "lib/parser.cpp"
    break;

  case 46: // expr: expr "^" expr
#line 181 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "^", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 887 "lib/parser.cpp"
    break;

  case 47: // expr: expr error expr
#line 182 "lib/parser.yy"
                                                { yylhs.value.as < Node > () = Node::create_binop_expr(yystack_[2].location.begin.line, yystack_[2].location.begin.column, "ERROR", yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 893 "lib/parser.cpp"
    break;

  case 48: // $@4: %empty
#line 183 "lib/parser.yy"
                                           { push_ctx(); }
#line 899 "lib/parser.cpp"
    break;

  case 49: // expr: OBJ_ID "(" $@4 args ")"
#line 183 "lib/parser.yy"
                                                                    { yylhs.value.as < Node > () = Node::create_call_expr(yystack_[4].location.begin.line, yystack_[4].location.begin.column, Node::create_object_identifier(yystack_[4].location.begin.line, yystack_[4].location.begin.column, "self", cur_clazz), yystack_[4].value.as < std::string > (), args_vec[ctx]); pop_ctx(); }
#line 905 "lib/parser.cpp"
    break;

  case 50: // $@5: %empty
#line 184 "lib/parser.yy"
                                                    { push_ctx(); }
#line 911 "lib/parser.cpp"
    break;

  case 51: // expr: expr "." OBJ_ID "(" $@5 args ")"
#line 184 "lib/parser.yy"
                                                                             { yylhs.value.as < Node > () = Node::create_call_expr(yystack_[6].location.begin.line, yystack_[6].location.begin.column, yystack_[6].value.as < Node > (), yystack_[4].value.as < std::string > (), args_vec[ctx]); pop_ctx(); }
#line 917 "lib/parser.cpp"
    break;

  case 52: // expr: "new" TYPE_ID
#line 185 "lib/parser.yy"
                                              { yylhs.value.as < Node > () = Node::create_new_expr(yystack_[1].location.begin.line, yystack_[1].location.begin.column, yystack_[0].value.as < std::string > ()); }
#line 923 "lib/parser.cpp"
    break;

  case 53: // expr: OBJ_ID
#line 186 "lib/parser.yy"
                                       { yylhs.value.as < Node > () = Node::create_object_identifier(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].value.as < std::string > ()); }
#line 929 "lib/parser.cpp"
    break;

  case 54: // expr: "self"
#line 187 "lib/parser.yy"
                                       { yylhs.value.as < Node > () = Node::create_object_identifier(yystack_[0].location.begin.line, yystack_[0].location.begin.column, "self", cur_clazz); }
#line 935 "lib/parser.cpp"
    break;

  case 55: // expr: literal
#line 188 "lib/parser.yy"
                                        { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 941 "lib/parser.cpp"
    break;

  case 56: // expr: "(" ")"
#line 189 "lib/parser.yy"
                                        { yylhs.value.as < Node > () = Node::create_unit(yystack_[1].location.begin.line, yystack_[1].location.begin.column); }
#line 947 "lib/parser.cpp"
    break;

  case 57: // expr: "(" expr ")"
#line 190 "lib/parser.yy"
                                             { yylhs.value.as < Node > () = yystack_[1].value.as < Node > (); }
#line 953 "lib/parser.cpp"
    break;

  case 58: // expr: block
#line 191 "lib/parser.yy"
                                      { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 959 "lib/parser.cpp"
    break;

  case 61: // arg: expr
#line 196 "lib/parser.yy"
                             { args_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 965 "lib/parser.cpp"
    break;

  case 62: // arg: arg "," expr
#line 197 "lib/parser.yy"
                                             { args_vec[ctx].push_back(yystack_[0].value.as < Node > ()); }
#line 971 "lib/parser.cpp"
    break;

  case 63: // literal: INTEGER_LIT
#line 199 "lib/parser.yy"
                                    { yylhs.value.as < Node > () = Node::create_int32(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].value.as < std::string > ()); }
#line 977 "lib/parser.cpp"
    break;

  case 64: // literal: STRING_LIT
#line 200 "lib/parser.yy"
                                           { yylhs.value.as < Node > () = Node::create_string(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].value.as < std::string > ()); }
#line 983 "lib/parser.cpp"
    break;

  case 65: // literal: boolean
#line 201 "lib/parser.yy"
                                        { yylhs.value.as < Node > () = Node::create_bool(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].value.as < std::string > ()); }
#line 989 "lib/parser.cpp"
    break;

  case 66: // boolean: "true"
#line 203 "lib/parser.yy"
                               { yylhs.value.as < std::string > () = "true"; }
#line 995 "lib/parser.cpp"
    break;

  case 67: // boolean: "false"
#line 204 "lib/parser.yy"
                                        { yylhs.value.as < std::string > () = "false"; }
#line 1001 "lib/parser.cpp"
    break;


#line 1005 "lib/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
   Parser ::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  //  Parser ::context.
   Parser ::context::context (const  Parser & yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
   Parser ::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
   Parser ::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -53;

  const signed char  Parser ::yytable_ninf_ = -63;

  const short
   Parser ::yypact_[] =
  {
      -6,     1,    20,   -53,    12,   -53,   -53,     8,   -11,   -53,
     -53,   -53,   -11,    -2,   -53,     4,   -53,   -53,   -53,    28,
       6,     7,    14,    15,   -53,   -53,   -53,   -53,   -53,   -53,
     -22,     6,    18,    28,   -53,  1100,   -53,     6,   -53,   -53,
     -53,   -24,   -53,  1100,  1100,    44,    47,  1100,   -53,   -53,
    1100,   -53,  1081,  1100,   -53,   817,   -53,   -53,    27,   -53,
    1100,   827,    62,    24,   -53,   104,   869,  1100,   -53,   882,
     146,  1100,  1100,   -53,  1100,  1100,  1100,  1100,  1100,    50,
    1100,  1100,  1100,   -53,  1100,   188,  1100,     6,  1100,    -5,
     707,   -53,   230,   272,   314,   356,   398,   440,   482,    29,
     966,  1005,  1044,   749,    30,    26,   524,   -13,   566,   -53,
    1100,   -53,   -53,  1100,  1100,  1100,  1100,   764,  1100,   806,
     608,   650,   924,    31,  1100,   -53,   692
  };

  const signed char
   Parser ::yydefact_[] =
  {
       0,     0,     0,     2,     4,     1,     3,     0,     0,     6,
       9,     5,     0,     0,     7,     0,     8,    10,    11,    20,
       0,     0,     0,    21,    22,    15,    17,    16,    18,    19,
       0,     0,     0,     0,    12,     0,    24,     0,    23,    63,
      64,    53,    67,     0,     0,     0,     0,     0,    54,    66,
       0,    25,     0,     0,    58,     0,    55,    65,     0,    48,
       0,     0,     0,     0,    52,     0,     0,     0,    56,     0,
       0,     0,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    59,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,    26,
       0,    50,    49,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,    51,     0
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -53,   -53,    60,   -53,   -53,    52,   -53,   -53,   -53,   -30,
     -53,   -53,    32,     9,   -53,   -53,   -35,   -53,   -53,   -52,
     -53,   -53,   -53
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
       0,     2,     3,     8,    12,    11,    13,    17,    18,    30,
      22,    23,    24,    54,    67,    89,   103,    84,   118,   104,
     105,    56,    57
  };

  const signed char
   Parser ::yytable_[] =
  {
      55,    36,   115,     1,    15,    59,     4,    58,    61,    62,
      34,    25,    65,     9,    26,    66,    10,    69,    70,    60,
       5,    35,    27,   109,     7,    85,    16,   110,    28,     1,
     116,    29,    90,    19,    21,    20,    92,    93,    31,    94,
      95,    96,    97,    98,    32,   100,   101,   102,    33,    37,
      63,   106,    64,   108,    51,    87,    99,   107,   111,   113,
     112,   125,     6,    71,    14,    38,   123,    83,     0,   -37,
       0,     0,   -37,   -37,     0,   117,     0,   -37,   119,   120,
     121,   122,     0,     0,     0,   -37,     0,     0,     0,   126,
     -37,     0,   -37,     0,   -37,   -37,   -37,   -37,   -37,   -37,
      78,    79,   -37,   -37,   -37,    71,     0,     0,     0,     0,
       0,   -35,     0,     0,   -35,   -35,     0,     0,     0,   -35,
       0,     0,     0,     0,     0,     0,     0,   -35,     0,     0,
       0,     0,   -35,     0,   -35,     0,   -35,   -35,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    71,     0,     0,
       0,     0,     0,   -36,     0,     0,   -36,   -36,     0,     0,
       0,   -36,     0,     0,     0,     0,     0,     0,     0,   -36,
       0,     0,     0,     0,   -36,     0,   -36,     0,   -36,   -36,
     -36,   -36,   -36,   -36,    78,    79,   -36,   -36,   -36,    71,
       0,     0,     0,     0,     0,    72,     0,     0,   -34,   -34,
       0,     0,     0,   -34,     0,     0,     0,     0,     0,     0,
       0,   -34,     0,     0,     0,     0,   -34,     0,   -34,     0,
     -34,   -34,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    71,     0,     0,     0,     0,     0,    72,     0,     0,
     -47,   -47,     0,     0,     0,   -47,     0,     0,     0,     0,
       0,     0,     0,   -47,     0,     0,     0,     0,   -47,     0,
     -47,     0,   -47,   -47,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    71,     0,     0,     0,     0,     0,   -38,
       0,     0,   -38,   -38,     0,     0,     0,   -38,     0,     0,
       0,     0,     0,     0,     0,   -38,     0,     0,     0,     0,
     -38,     0,   -38,     0,   -38,   -38,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    71,     0,     0,     0,     0,
       0,   -42,     0,     0,   -42,   -42,     0,     0,     0,   -42,
       0,     0,     0,     0,     0,     0,     0,   -42,     0,     0,
       0,     0,   -42,     0,   -42,     0,   -42,   -42,   -42,   -42,
      76,    77,    78,    79,   -42,   -42,   -42,    71,     0,     0,
       0,     0,     0,   -43,     0,     0,   -43,   -43,     0,     0,
       0,   -43,     0,     0,     0,     0,     0,     0,     0,   -43,
       0,     0,     0,     0,   -43,     0,   -43,     0,   -43,   -43,
     -43,   -43,    76,    77,    78,    79,   -43,   -43,   -43,    71,
       0,     0,     0,     0,     0,   -44,     0,     0,   -44,   -44,
       0,     0,     0,   -44,     0,     0,     0,     0,     0,     0,
       0,   -44,     0,     0,     0,     0,   -44,     0,   -44,     0,
     -44,   -44,   -44,   -44,   -44,   -44,    78,    79,   -44,   -44,
     -44,    71,     0,     0,     0,     0,     0,   -45,     0,     0,
     -45,   -45,     0,     0,     0,   -45,     0,     0,     0,     0,
       0,     0,     0,   -45,     0,     0,     0,     0,   -45,     0,
     -45,     0,   -45,   -45,   -45,   -45,   -45,   -45,    78,    79,
     -45,   -45,   -45,    71,     0,     0,     0,     0,     0,   -46,
       0,     0,   -46,   -46,     0,     0,     0,   -46,     0,     0,
       0,     0,     0,     0,     0,   -46,     0,     0,     0,     0,
     -46,     0,   -46,     0,   -46,   -46,   -46,   -46,   -46,   -46,
      78,    79,   -46,   -46,   -46,    71,     0,     0,     0,     0,
       0,    72,     0,     0,   -29,   114,     0,     0,     0,   -29,
       0,     0,     0,     0,     0,     0,     0,   -29,     0,     0,
       0,     0,   -29,     0,   -29,     0,   -29,   -29,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    71,     0,     0,
       0,     0,     0,    72,     0,     0,   -31,   -31,     0,     0,
       0,   -31,     0,     0,     0,     0,     0,     0,     0,   -31,
       0,     0,     0,     0,   -31,     0,   -31,     0,   -31,   -31,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    71,
       0,     0,     0,     0,     0,    72,     0,     0,   -30,   -30,
       0,     0,     0,   -30,     0,     0,     0,     0,     0,     0,
       0,   -30,     0,     0,     0,     0,   -30,     0,   -30,     0,
     -30,   -30,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    71,     0,     0,     0,     0,     0,    72,     0,     0,
     -32,   -32,     0,     0,     0,   -32,     0,     0,     0,     0,
       0,     0,     0,   -32,     0,     0,     0,     0,   -32,     0,
     -32,     0,   -32,   -32,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    71,     0,     0,     0,     0,     0,    72,
       0,     0,   -33,   -33,     0,     0,     0,   -33,    71,     0,
       0,     0,     0,     0,    72,   -33,     0,     0,     0,     0,
     -33,     0,   -33,     0,   -33,   -33,    74,    75,    76,    77,
      78,    79,    80,    81,    82,   -27,     0,     0,     0,   -27,
       0,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      71,     0,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,   -61,
       0,     0,   -61,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   -28,     0,     0,     0,   -28,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    71,     0,     0,
       0,     0,     0,    72,     0,     0,     0,     0,    71,     0,
       0,     0,     0,     0,    72,     0,     0,     0,    71,     0,
       0,     0,     0,     0,    72,     0,   -62,     0,     0,   -62,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    73,
      86,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       0,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      71,     0,     0,     0,     0,     0,    72,     0,     0,    88,
       0,     0,     0,    71,     0,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    91,     0,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    71,     0,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    71,     0,     0,
       0,     0,     0,   -39,     0,     0,   -39,   -39,     0,     0,
       0,   -39,     0,     0,     0,     0,     0,     0,     0,   -39,
       0,     0,     0,     0,   -39,     0,   -39,     0,   -39,   -39,
      74,    75,    76,    77,    78,    79,    71,     0,     0,     0,
       0,     0,   -40,     0,     0,   -40,   -40,     0,     0,     0,
     -40,     0,     0,     0,     0,     0,     0,     0,   -40,     0,
       0,     0,     0,   -40,     0,   -40,     0,   -40,   -40,    74,
      75,    76,    77,    78,    79,    71,     0,     0,     0,     0,
       0,   -41,     0,     0,   -41,   -41,     0,     0,     0,   -41,
       0,     0,     0,     0,     0,     0,     0,   -41,     0,     0,
       0,     0,   -41,     0,   -41,     0,   -41,   -41,    74,    75,
      76,    77,    78,    79,    39,    40,     0,    41,     0,     0,
       0,     0,     0,     0,    42,    43,     0,     0,    44,    45,
      46,    47,    48,    39,    40,    49,    41,    50,    51,     0,
      52,    68,     0,    42,    43,     0,    53,    44,    45,    46,
      47,    48,     0,     0,    49,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,    53
  };

  const signed char
   Parser ::yycheck_[] =
  {
      35,    31,    15,     9,     6,    29,     5,    37,    43,    44,
      32,     5,    47,     5,     8,    50,    27,    52,    53,    43,
       0,    43,    16,    28,    12,    60,    28,    32,    22,     9,
      43,    25,    67,    29,     6,    31,    71,    72,    31,    74,
      75,    76,    77,    78,    30,    80,    81,    82,    33,    31,
       6,    86,     5,    88,    27,    31,     6,    87,    29,    33,
      30,    30,     2,     1,    12,    33,   118,    58,    -1,     7,
      -1,    -1,    10,    11,    -1,   110,    -1,    15,   113,   114,
     115,   116,    -1,    -1,    -1,    23,    -1,    -1,    -1,   124,
      28,    -1,    30,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     1,
      -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      28,    -1,    30,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     1,
      -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      28,    -1,    30,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     1,
      -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,     1,    -1,
      -1,    -1,    -1,    -1,     7,    23,    -1,    -1,    -1,    -1,
      28,    -1,    30,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    28,    -1,    -1,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    28,    -1,    -1,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    -1,    -1,     1,    -1,
      -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,     1,    -1,
      -1,    -1,    -1,    -1,     7,    -1,    30,    -1,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    32,
      23,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    10,
      -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    30,    -1,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,     1,    -1,    -1,    -1,
      -1,    -1,     7,    -1,    -1,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    -1,    30,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,     3,     4,    -1,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,     3,     4,    24,     6,    26,    27,    -1,
      29,    30,    -1,    13,    14,    -1,    35,    17,    18,    19,
      20,    21,    -1,    -1,    24,    -1,    26,    27,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    35
  };

  const signed char
   Parser ::yystos_[] =
  {
       0,     9,    46,    47,     5,     0,    47,    12,    48,     5,
      27,    50,    49,    51,    50,     6,    28,    52,    53,    29,
      31,     6,    55,    56,    57,     5,     8,    16,    22,    25,
      54,    31,    30,    33,    32,    43,    54,    31,    57,     3,
       4,     6,    13,    14,    17,    18,    19,    20,    21,    24,
      26,    27,    29,    35,    58,    61,    66,    67,    54,    29,
      43,    61,    61,     6,     5,    61,    61,    59,    30,    61,
      61,     1,     7,    32,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    58,    62,    61,    23,    31,    10,    60,
      61,    30,    61,    61,    61,    61,    61,    61,    61,     6,
      61,    61,    61,    61,    64,    65,    61,    54,    61,    28,
      32,    29,    30,    33,    11,    15,    43,    61,    63,    61,
      61,    61,    61,    64,    15,    30,    61
  };

  const signed char
   Parser ::yyr1_[] =
  {
       0,    45,    46,    46,    48,    47,    49,    47,    50,    51,
      51,    51,    52,    52,    53,    54,    54,    54,    54,    54,
      55,    55,    56,    56,    57,    59,    58,    60,    60,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    62,    61,
      63,    61,    61,    61,    61,    61,    61,    61,    61,    64,
      64,    65,    65,    66,    66,    66,    67,    67
  };

  const signed char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     2,     0,     4,     0,     6,     3,     0,
       2,     2,     4,     6,     7,     1,     1,     1,     1,     1,
       0,     1,     1,     3,     3,     0,     4,     1,     3,     4,
       6,     4,     6,     8,     3,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     0,     5,
       0,     7,     2,     1,     1,     1,     2,     3,     1,     0,
       1,     1,     3,     1,     1,     1,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "END", "error", "\"invalid token\"", "INTEGER_LIT", "STRING_LIT",
  "TYPE_ID", "OBJ_ID", "\"and\"", "\"bool\"", "\"class\"", "\"do\"",
  "\"else\"", "\"extends\"", "\"false\"", "\"if\"", "\"in\"", "\"int32\"",
  "\"isnull\"", "\"let\"", "\"new\"", "\"not\"", "\"self\"", "\"string\"",
  "\"then\"", "\"true\"", "\"unit\"", "\"while\"", "\"{\"", "\"}\"",
  "\"(\"", "\")\"", "\":\"", "\";\"", "\",\"", "\"+\"", "\"-\"", "\"*\"",
  "\"/\"", "\"^\"", "\".\"", "\"=\"", "\"<\"", "\"<=\"", "\"<-\"", "UMIN",
  "$accept", "program", "clazz", "$@1", "$@2", "clazzbody", "body",
  "field", "method", "type", "formals", "formal", "form", "block", "$@3",
  "exprs", "expr", "$@4", "$@5", "args", "arg", "literal", "boolean", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,   128,   128,   129,   131,   131,   132,   132,   134,   136,
     137,   138,   140,   141,   143,   145,   146,   147,   148,   149,
     151,   152,   154,   155,   157,   159,   159,   161,   162,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   183,
     184,   184,   185,   186,   187,   188,   189,   190,   191,   193,
     194,   196,   197,   199,   200,   201,   203,   204
  };

  void
   Parser ::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
   Parser ::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1753 "lib/parser.cpp"

#line 206 "lib/parser.yy"
 // End grammar rules


yy::Parser::symbol_type yy::yylex(Scanner &scanner) {

	Token t = scanner.get_next_token();

	yy::position pos;
	pos.column = t.get_column();
	pos.line = t.get_line();
	yy::location loc(pos, pos);
	std::string val = t.get_value();

	switch(t.get_class()) {

		case TokenClass::integer_literal:
			return yy::Parser::make_INTEGER_LIT(val, loc);

		case TokenClass::keyword:
			if(val.compare("and") == 0) {
				return yy::Parser::make_AND_KWORD(loc);
			} else if(val.compare("bool") == 0) {
				return yy::Parser::make_BOOL_KWORD(loc);
			} else if(val.compare("class") == 0) {
				return yy::Parser::make_CLASS_KWORD(loc);
			} else if(val.compare("do") == 0) {
				return yy::Parser::make_DO_KWORD(loc);
			} else if(val.compare("else") == 0) {
				return yy::Parser::make_ELSE_KWORD(loc);
			} else if(val.compare("extends") == 0) {
				return yy::Parser::make_EXTENDS_KWORD(loc);
			} else if(val.compare("false") == 0) {
				return yy::Parser::make_FALSE_KWORD(loc);
			} else if(val.compare("if") == 0) {
				return yy::Parser::make_IF_KWORD(loc);
			} else if(val.compare("in") == 0) {
				return yy::Parser::make_IN_KWORD(loc);
			} else if(val.compare("int32") == 0) {
				return yy::Parser::make_INT32_KWORD(loc);
			} else if(val.compare("isnull") == 0) {
				return yy::Parser::make_ISNULL_KWORD(loc);
			} else if(val.compare("let") == 0) {
				return yy::Parser::make_LET_KWORD(loc);
			} else if(val.compare("new") == 0) {
				return yy::Parser::make_NEW_KWORD(loc);
			} else if(val.compare("not") == 0) {
				return yy::Parser::make_NOT_KWORD(loc);
			} else if(val.compare("self") == 0) {
				return yy::Parser::make_SELF_KWORD(loc);
			} else if(val.compare("string") == 0) {
				return yy::Parser::make_STRING_KWORD(loc);
			}	else if(val.compare("then") == 0) {
				return yy::Parser::make_THEN_KWORD(loc);
			} else if(val.compare("true") == 0) {
				return yy::Parser::make_TRUE_KWORD(loc);
			} else if(val.compare("unit") == 0) {
				return yy::Parser::make_UNIT_KWORD(loc);
			} else {
				return yy::Parser::make_WHILE_KWORD(loc);
			}

		case TokenClass::type_identifier:
			return yy::Parser::make_TYPE_ID(val, loc);

		case TokenClass::object_identifier:
			return yy::Parser::make_OBJ_ID(val, loc);

		case TokenClass::string_literals:
			return yy::Parser::make_STRING_LIT(val, loc);

		case TokenClass::operators:
			if(val.compare("lbrace") == 0) {
				return yy::Parser::make_LBRACE(loc);
			} else if(val.compare("rbrace") == 0) {
				return yy::Parser::make_RBRACE(loc);
			} else if(val.compare("lpar") == 0) {
				return yy::Parser::make_LPAR(loc);
			} else if(val.compare("rpar") == 0) {
				return yy::Parser::make_RPAR(loc);
			} else if(val.compare("colon") == 0) {
				return yy::Parser::make_COLON(loc);
			} else if(val.compare("semicolon") == 0) {
				return yy::Parser::make_SEMICOLON(loc);
			} else if(val.compare("comma") == 0) {
				return yy::Parser::make_COMMA(loc);
			} else if(val.compare("plus") == 0) {
				return yy::Parser::make_PLUS(loc);
			} else if(val.compare("minus") == 0) {
				return yy::Parser::make_MINUS(loc);
			} else if(val.compare("times") == 0) {
				return yy::Parser::make_TIMES(loc);
			} else if(val.compare("div") == 0) {
				return yy::Parser::make_DIV(loc);
			} else if(val.compare("pow") == 0) {
				return yy::Parser::make_POW(loc);
			} else if(val.compare("dot") == 0) {
				return yy::Parser::make_DOT(loc);
			} else if(val.compare("equal") == 0) {
				return yy::Parser::make_EQUAL(loc);
			} else if(val.compare("lower") == 0) {
				return yy::Parser::make_LOWER(loc);
			} else if(val.compare("lower-equal") == 0) {
				return yy::Parser::make_LEQ(loc);
			} else {
				return yy::Parser::make_ASSIGN(loc);
			}
			
		case TokenClass::eof:
		default:
			return yy::Parser::make_END(loc);
	}
}

void yy::Parser::error(const location_type &l, const std::string &m) {
	error_vector.emplace_back(ErrorType::syntaxical, l.begin.line, l.begin.column, m);
}

Node yy::Parser::get_ast() {
	return ast;
}

std::vector<Error> yy::Parser::get_errors() {
	return error_vector;
}

void push_ctx() {
	++ctx;
	methods_vec.push_back(std::vector<Node>());
	fields_vec.push_back(std::vector<Node>());
	formals_vec.push_back(std::vector<Node>());
	args_vec.push_back(std::vector<Node>());
	block_vec.push_back(std::vector<Node>());
}

void pop_ctx() {
	--ctx;
	methods_vec.pop_back();
	fields_vec.pop_back();
	formals_vec.pop_back();
	args_vec.pop_back();
	block_vec.pop_back();
}


