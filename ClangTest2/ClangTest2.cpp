// ClangTest2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
//
//int main()
//{
//    std::cout << "Hello World!\n";
//}

#include <clang-c/Index.h>
#include <iostream>
#include <vector>
using namespace std;
std::string g_fileName = "";

struct fileLocation
{
    std::string fileName;
    unsigned line;
    unsigned column;
};

ostream& operator<<(ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

std::string getTypeName(CXCursor cursor)
{
    CXType type = clang_getCursorType(cursor);
    {
        // 判断类型是否是一个模板实例化，普通的类的模板参数是-1
        int num = clang_Type_getNumTemplateArguments(type);

        // 遍历全部的实例化的模板参数
        for (auto loop = 0; loop < num; ++loop) {
            CXType ttype = clang_Type_getTemplateArgumentAsType(type, loop);
            std::string ttypeName = clang_getCString(clang_getTypeSpelling(ttype));
            std::cout << "......" << ttypeName << std::endl;
        }
    }
    std::string typeName = clang_getCString(clang_getTypeSpelling(type));
    return typeName;
}

fileLocation getFileName(CXCursor cursor)
{
    fileLocation flocation;
    //CXCursorKind kind = clang_getCursorKind(cursor);
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXSourceLocation location = clang_getRangeStart(range);

    CXFile file;
    unsigned line;
    unsigned column;
    clang_getFileLocation(location, &file, &line, &column, nullptr);

    auto fileName = clang_getFileName(file);

    std::string nameStr = "";
    if (clang_getCString(fileName)) nameStr = clang_getCString(fileName);
    flocation.line = line;
    flocation.column = column;
    flocation.fileName = nameStr;
    return flocation;
}

std::vector<CXCursor> stackList;

CXChildVisitResult visitor2(CXCursor cursor, CXCursor parent, CXClientData data) {
    fileLocation loc = getFileName(cursor);
    if ( loc.fileName != g_fileName) return CXChildVisit_Continue;

    if (clang_getCString(clang_Cursor_getRawCommentText(cursor)))
        std::cout <<"////////////////"<< clang_getCString(clang_Cursor_getRawCommentText(cursor)) << std::endl;

    while (stackList.size() > 0)
    {
        if (stackList[stackList.size() - 1].data[0] == parent.data[0])
            break;
        else
            stackList.erase(stackList.end()-1);
    }
    for (int i = 0; i < stackList.size(); i++)
    {
        std::cout << "......";
    }
    stackList.push_back(cursor);

    std::cout << "Cursor '" << clang_getCursorSpelling(cursor) << "' of kind '"
        << clang_getCursorKindSpelling(clang_getCursorKind(cursor)) << "  type:" << getTypeName(cursor) << "' parent:" << clang_getCursorSpelling(parent) << "  line:" << loc.line << "\n";

    return CXChildVisit_Recurse;
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData data) {

    fileLocation loc = getFileName(cursor);
    if (loc.fileName != g_fileName) return CXChildVisit_Continue;

    CXCursorKind kind = clang_getCursorKind(cursor);

    // Consider functions and methods
    if (kind == CXCursorKind::CXCursor_FunctionDecl ||
        kind == CXCursorKind::CXCursor_CXXMethod) {
        auto cursorName = clang_getCursorDisplayName(cursor);

        // Print if function/method starts with doSomething
        auto cursorNameStr = std::string(clang_getCString(cursorName));
        if (cursorNameStr.find("doSomething") == 0) {
            // Get the source locatino
            CXSourceRange range = clang_getCursorExtent(cursor);
            CXSourceLocation location = clang_getRangeStart(range);

            CXFile file;
            unsigned line;
            unsigned column;
            clang_getFileLocation(location, &file, &line, &column, nullptr);

            auto fileName = clang_getFileName(file);

            std::cout << "Found call to " << clang_getCString(cursorName) << " at "
                << line << ":" << column << " in " << clang_getCString(fileName)
                << std::endl;

            clang_disposeString(fileName);
        }

        clang_disposeString(cursorName);
    }

    return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }

    // Command line arguments required for parsing the TU
    //constexpr const char* ARGUMENTS[] = {};
    const char* ARGUMENTS[] = { "-c", "-x", "c++" };

    // Create an index with excludeDeclsFromPCH = 1, displayDiagnostics = 0
    CXIndex index = clang_createIndex(1, 0);

    // Speed up parsing by skipping function bodies
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(
        index, argv[1], ARGUMENTS, std::extent<decltype(ARGUMENTS)>::value,
        nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_DetailedPreprocessingRecord);

    g_fileName = argv[1];

    

    // Visit all the nodes in the AST
    CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
    clang_visitChildren(cursor, visitor2, 0);

    // Release memory
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);

    return 0;
}