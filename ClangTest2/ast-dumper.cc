//#include <clang-c/Index.h>
//
//#include <algorithm>
//#include <iostream>
//#include <map>
//#include <string>
//#include <sstream>
//#include <vector>
//
//std::map<std::string, unsigned int> counter;
//
//std::string getCursorKindName( CXCursorKind cursorKind )
//{
//  CXString kindName  = clang_getCursorKindSpelling( cursorKind );
//  std::string result = clang_getCString( kindName );
//
//  clang_disposeString( kindName );
//  return result;
//}
//
//std::string getCursorSpelling( CXCursor cursor )
//{
//  CXString cursorSpelling = clang_getCursorSpelling( cursor );
//  std::string result      = clang_getCString( cursorSpelling );
//
//  clang_disposeString( cursorSpelling );
//  return result;
//}
//
//CXChildVisitResult visitor( CXCursor cursor, CXCursor /* parent */, CXClientData clientData )
//{
//  CXSourceLocation location = clang_getCursorLocation( cursor );
//  if( clang_Location_isFromMainFile( location ) == 0 )
//    return CXChildVisit_Continue;
//
//  CXCursorKind cursorKind = clang_getCursorKind( cursor );
//
//  unsigned int curLevel = *( reinterpret_cast<unsigned int*>( clientData ) );
//
//  bool isFunctionOrMethod = cursorKind == CXCursorKind::CXCursor_CXXMethod
//                         || cursorKind == CXCursorKind::CXCursor_FunctionDecl
//                         || cursorKind == CXCursorKind::CXCursor_FunctionTemplate;
//
//  auto name       = getCursorKindName( cursorKind );
//  counter[ name ] = counter[ name ] + 1;
//
//  std::ostringstream stream;
//  stream << std::string( curLevel, '-' ) << " " << name;
//
//  if( isFunctionOrMethod )
//    stream << " (" << getCursorSpelling( cursor ) << ")\n";
//  else
//    stream << "\n";
//
//  {
//    auto&& translationUnit = clang_Cursor_getTranslationUnit( cursor );
//    auto&& range           = clang_getCursorExtent( cursor );
//
//    CXToken* tokens        = nullptr;
//    unsigned int numTokens = 0;
//
//    clang_tokenize( translationUnit,
//                    range,
//                    &tokens,
//                    &numTokens );
//
//    stream << std::string( curLevel, ' ' ) << "  ";
//
//    for( unsigned int i = 0; i < numTokens; i++ )
//    {
//      CXString tokenSpelling = clang_getTokenSpelling( translationUnit, tokens[i] );
//
//      if( i != 0 )
//        stream << " ";
//
//      stream << clang_getCString( tokenSpelling );
//
//      clang_disposeString( tokenSpelling );
//    }
//
//    stream << "\n";
//
//    clang_disposeTokens( translationUnit, tokens, numTokens );
//  }
//
//  std::cerr << stream.str();
//
//  unsigned int nextLevel = curLevel + 1;
//
//  clang_visitChildren( cursor,
//                       visitor,
//                       &nextLevel ); 
//
//  if( curLevel == 0 )
//    std::cerr << "\n";
//
//  return CXChildVisit_Continue;
//}
//
//int main( int argc, char** argv )
//{
//  if( argc < 2 )
//    return -1;
//
//  char* srcPath = "C:\\Users\\zmy12006\\Downloads\\libclang-experiments-master\\test\\IGData.h";
//
//  CXIndex index        = clang_createIndex( 0, 1 );
//  CXTranslationUnit tu = clang_createTranslationUnit( index, /*argv[1]*/srcPath );
//
//  {
//      const char* args[] = { "c++"};
//      tu = clang_parseTranslationUnit(index,
//          srcPath,
//          NULL,
//          0,
//          NULL,
//          0,
//          CXTranslationUnit_None);
//  }
//
//  CXCursor rootCursor  = clang_getTranslationUnitCursor( tu );
//
//  // FIXME: Need to handle errors in TU creation and index creation. Need to
//  // check the documentation here...
//
//  unsigned int treeLevel = 0;
//
//  clang_visitChildren( rootCursor, visitor, &treeLevel );
//
//  clang_disposeTranslationUnit( tu );
//  clang_disposeIndex( index );
//
//  std::vector<std::string> cursorKinds;
//  cursorKinds.reserve( counter.size() );
//
//  for( auto&& pair : counter )
//    cursorKinds.push_back( pair.first );
//
//  std::sort( cursorKinds.begin(), cursorKinds.end(),
//             [] ( const std::string& s1, const std::string& s2 )
//             {
//               return counter.at( s1 ) > counter.at( s2 );
//             } );
//
//  {
//    std::ostringstream stream;
//
//    for( auto&& cursorKind : cursorKinds )
//      stream << cursorKind << ": " << counter.at( cursorKind ) << "\n";
//
//    std::cerr << stream.str();
//  }
//
//  return 0;
//}


#include <clang-c/Index.h>
#include <iostream>
int cnt = 0;
CXChildVisitResult visitor(CXCursor cursor, CXCursor, CXClientData) {
    CXCursorKind kind = clang_getCursorKind(cursor);

    // Consider functions and methods
    if (kind == CXCursorKind::CXCursor_FunctionDecl ||
        kind == CXCursorKind::CXCursor_CXXMethod ||
        kind == CXCursorKind::CXCursor_ClassDecl ||
        kind == CXCursorKind::CXCursor_FieldDecl ||
        kind == CXCursorKind::CXCursor_ParmDecl) {
        auto cursorName = clang_getCursorDisplayName(cursor);
        
        // Print if function/method starts with doSomething
        auto cursorNameStr = std::string(clang_getCString(cursorName));

        //clang_getTypeSpelling(cursor);

        if (cursorNameStr.find("doSomething") == 0 || true) {

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
            // Get the source locatino
            CXSourceRange range = clang_getCursorExtent(cursor);
            CXSourceLocation location = clang_getRangeStart(range);

            CXFile file;
            unsigned line;
            unsigned column;
            clang_getFileLocation(location, &file, &line, &column, nullptr);

            auto fileName = clang_getFileName(file);
            auto pc = clang_getCursorSemanticParent(cursor);
            std::string pcname = clang_getCString(clang_getCursorDisplayName(pc));
            if (kind == CXCursorKind::CXCursor_ParmDecl)std::cout << "______" << "(" << pcname <<")";
            std::cout << cnt << "Found call to " << clang_getCString(cursorName) << "  type:" <<typeName << " at "
                << line << ":" << column << " in " << clang_getCString(fileName)
                << std::endl;
            cnt++;
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
        nullptr, 0, CXTranslationUnit_SkipFunctionBodies);

    // Visit all the nodes in the AST
    CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
    clang_visitChildren(cursor, visitor, 0);

    // Release memory
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);

    return 0;
}