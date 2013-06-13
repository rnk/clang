add_clang_library(clangDriver
  Action.cpp
  CC1AsOptions.cpp
  Compilation.cpp
  Driver.cpp
  DriverOptions.cpp
  Job.cpp
  MSVCOptions.cpp
  MSVCRewriter.cpp
  Phases.cpp
  Tool.cpp
  ToolChain.cpp
  ToolChains.cpp
  WindowsToolChain.cpp
  Tools.cpp
  Types.cpp
  )

set(LLVM_TARGET_DEFINITIONS MSVCOptions.td)
tablegen(CLANG MSVCOptions.inc -gen-opt-parser-defs)
add_public_tablegen_target(ClangMSVCOptions)

add_dependencies(clangDriver
  ClangAttrList
  ClangCC1AsOptions
  ClangDiagnosticCommon
  ClangDiagnosticDriver
  ClangDriverOptions
  ClangMSVCOptions
  )

target_link_libraries(clangDriver
  clangBasic
  LLVMOption
  )
