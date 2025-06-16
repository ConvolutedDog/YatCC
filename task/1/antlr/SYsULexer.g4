lexer grammar SYsULexer;

Int : 'int';
Return : 'return';

LeftParen : '(';
RightParen : ')';
LeftBracket : '[';
RightBracket : ']';
LeftBrace : '{';
RightBrace : '}';

Plus : '+';
Minus : '-';

Semi : ';';
Comma : ',';

Equal : '=';
Less : '<';
Greater : '>';
LessEqual : '<=';
GreaterEqual : '>=';
ExclaimEqual : '!=';
Exclaim : '!';
Slash : '/';
Percent : '%';

EqualEqual : '==';
PipePipe : '||';
AmpAmp : '&&';

If : 'if';
Else : 'else';
While : 'while';
Break : 'break';
Continue : 'continue';

Void : 'void';
Const : 'const';
Char : 'char';

Star : '*';


Identifier
    :   IdentifierNondigit
        (   IdentifierNondigit
        |   Digit
        )*
    ;

fragment
IdentifierNondigit
    :   Nondigit
    ;

fragment
Nondigit
    :   [a-zA-Z_]
    ;

fragment
Digit
    :   [0-9]
    ;

Constant
    :   IntegerConstant
    ;

fragment
IntegerConstant
    :   DecimalConstant
    |   OctalConstant
    |   HexadecimalConstant
    ;

fragment
DecimalConstant
    :   NonzeroDigit Digit*
    ;

fragment
OctalConstant
    :   '0' OctalDigit*
    ;

fragment
HexadecimalConstant
    :   '0' [xX] HexadecimalDigit+
    ;

fragment
NonzeroDigit
    :   [1-9]
    ;

fragment
OctalDigit
    :   [0-7]
    ;

fragment
HexadecimalDigit
    :   [0-9a-fA-F]
    ;

// 预处理信息处理，可以从预处理信息中获得文件名以及行号
// 预处理信息中的第一个数字即为行号
LineAfterPreprocessing
    :   '#' Whitespace* ~[\r\n]*
        -> channel(HIDDEN)
    ;

Whitespace
    :   [ \t]+
        -> channel(HIDDEN)
    ;

// 换行符号，可以利用这个信息来更新行号
Newline
    :   (   '\r' '\n'?
        |   '\n'
        )
        -> channel(HIDDEN)
    ;

