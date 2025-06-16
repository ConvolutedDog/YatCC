#include "SYsULexer.h" // 确保这里的头文件名与您生成的词法分析器匹配
#include <fstream>
#include <iostream>
#include <unordered_map>

// 映射定义，将ANTLR的tokenTypeName映射到clang的格式
std::unordered_map<std::string, std::string> tokenTypeMapping = {
  { "Int", "int" },
  { "Identifier", "identifier" },
  { "LeftParen", "l_paren" },
  { "RightParen", "r_paren" },
  { "RightBrace", "r_brace" },
  { "LeftBrace", "l_brace" },
  { "LeftBracket", "l_square" },
  { "RightBracket", "r_square" },
  { "Constant", "numeric_constant" },
  { "Return", "return" },
  { "Semi", "semi" },
  { "EOF", "eof" },
  { "Equal", "equal" },
  { "Plus", "plus" },
  { "Comma", "comma" },

  // 在这里继续添加其他映射
  { "While", "while" },
  { "Less", "less" },
  { "Greater", "greater" },
  { "Minus", "minus" },
  { "If", "if" },
  { "Star", "star" },
  { "EqualEqual", "equalequal" },
  { "Void", "void" },
  { "PipePipe", "pipepipe" },
  { "Else", "else" },
  { "AmpAmp", "ampamp" },
  { "Slash", "slash" },
  { "Percent", "percent" },
  { "LessEqual", "lessequal" },
  { "GreaterEqual", "greaterequal" },
  { "Exclaim", "exclaim" },
  { "ExclaimEqual", "exclaimequal" },
  { "Const", "const" },
  { "Char", "char" },
  { "Break", "break" },
  { "Continue", "continue" },
};

static int lastLine = -1;
static int lastColumn = -1;
static int lastLength = -1;

static int nextLine = 0;
static std::string nextFileName = "";
static int nextflag = 0;

std::string
get_quoted_strings(const std::string& input)
{
  std::string quotedStrings;
  size_t start = 0;

  start = input.find('"', start);
  if (start == std::string::npos)
    return quotedStrings;

  size_t end = input.find('"', start + 1);
  if (end == std::string::npos)
    return quotedStrings;

  quotedStrings = input.substr(start + 1, end - start - 1);

  return quotedStrings;
}

#include <regex>

void
print_token(const antlr4::Token* token,
            const antlr4::CommonTokenStream& tokens,
            std::ofstream& outFile,
            const antlr4::Lexer& lexer)
{
  auto& vocabulary = lexer.getVocabulary();

  auto tokenTypeName =
    std::string(vocabulary.getSymbolicName(token->getType()));

  if (tokenTypeName.empty())
    tokenTypeName = "<UNKNOWN>"; // 处理可能的空字符串情况

  if (tokenTypeMapping.find(tokenTypeName) != tokenTypeMapping.end()) {
    tokenTypeName = tokenTypeMapping[tokenTypeName];
  }

  if (token->getChannel() == antlr4::Token::HIDDEN_CHANNEL &&
      tokenTypeName != "Whitespace" && tokenTypeName != "Newline") {
    std::string text = token->getText();
    std::regex pattern1("# ([0-9]+) \"");
    std::smatch match1;
    if (std::regex_search(text, match1, pattern1)) {
      nextLine = std::stoi(match1[1]);
    }

    nextFileName = get_quoted_strings(text);

    std::regex pattern2("\" ([1-4]+)");
    std::smatch match2;
    nextflag = 0;
    if (std::regex_search(text, match2, pattern2)) {
      nextflag = std::stoi(match2[1]);
    }
  }

  std::string locInfo;
  if (token->getChannel() == antlr4::Token::DEFAULT_CHANNEL) {

    bool startOfLine = false;
    bool leadingSpace = false;

    if (token->getLine() != lastLine) {
      if (tokenTypeName != "eof") {
        startOfLine = true;
        lastLine = token->getLine();
      } else {
        lastLine = -1;
      }
    }

    if (startOfLine)
      leadingSpace = token->getCharPositionInLine() > 0;
    else {
      leadingSpace = token->getCharPositionInLine() > lastColumn + lastLength;
    }

    locInfo = std::string(" Loc=<") + nextFileName + ":" +
              std::to_string(nextLine - 1) + ":" +
              std::to_string(token->getCharPositionInLine() + 1) + ">";

    lastColumn = token->getCharPositionInLine();
    lastLength = token->getText().length();

    if (token->getText() != "<EOF>")
      outFile << tokenTypeName << " '" << token->getText() << "'";
    else
      outFile << tokenTypeName << " '"
              << "'";
    if (startOfLine)
      outFile << "\t [StartOfLine]";
    if (leadingSpace)
      outFile << " [LeadingSpace]";
    outFile << locInfo << std::endl;
  } else {
    if (tokenTypeName == "Newline") {
      nextLine++;
    }
  }
}

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input> <output>\n";
    return -1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile) {
    std::cout << "Error: unable to open input file: " << argv[1] << '\n';
    return -2;
  }

  std::ofstream outFile(argv[2]);
  if (!outFile) {
    std::cout << "Error: unable to open output file: " << argv[2] << '\n';
    return -3;
  }

  std::cout << "程序 '" << argv[0] << std::endl;
  std::cout << "输入 '" << argv[1] << std::endl;
  std::cout << "输出 '" << argv[2] << std::endl;

  antlr4::ANTLRInputStream input(inFile);
  SYsULexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();

  for (auto&& token : tokens.getTokens()) {
    print_token(token, tokens, outFile, lexer);
  }
}
