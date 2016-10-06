macro(add_gf_snippet NAME)
  add_executable("${NAME}" "${NAME}.cc")
  target_link_libraries("${NAME}" gf0)
endmacro()

add_gf_snippet(doc_index)
