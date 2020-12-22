// ClangTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


#include <iostream>
#include <clang-c/Index.h>
using namespace std;

ostream& operator<<(ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

CXChildVisitResult printVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXCursor cursor1 = clang_getCursorReferenced(cursor);

    CXType type = clang_getCursorType(cursor1);
    CXCursorKind kind = clang_getCursorKind(cursor1);
    CXString str = clang_getTypeSpelling(type);
    CXString str1 = clang_getCursorSpelling(cursor1);
    std::string cstr = clang_getCString(str);
    std::string cstr1 = clang_getCString(str1);

    if (type.kind != 0 && kind == CXCursorKind::CXCursor_FunctionDecl)
    {
        std::cout << "Declaration!!!!!!!!!!!!!!!\n" << "type is: " << cstr << std::endl;
        std::cout << "name is: " << cstr1 << std::endl;
    }
    //clang_getCursorResultType
    else if (type.kind != 0 && kind == CXCursorKind::CXCursor_CXXMethod)
    {
        std::cout << "return type:" << clang_getTypeSpelling(clang_getCursorResultType(cursor)) <<std::endl;
    }
    else if (type.kind != 0 && kind == CXCursorKind::CXCursor_EnumConstantDecl )
    {
        std::cout << "value is: " << clang_getEnumConstantDeclUnsignedValue(cursor)<<std::endl;
    }
    else
    {
        std::cout << "unknow......\n";
    }

    return CXChildVisit_Recurse;
}

int main()
{
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        "header.hpp", nullptr, 0,
        nullptr, 0,
        CXTranslationUnit_None);
    if (unit == nullptr)
    {
        cerr << "Unable to parse translation unit. Quitting." << endl;
        exit(-1);
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
                << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "' parent:" << clang_getCursorSpelling(parent) << "\n";
            printVisitor(c, parent, client_data);
            return CXChildVisit_Recurse;
        },
        nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}