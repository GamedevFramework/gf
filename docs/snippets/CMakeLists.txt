macro(add_gf_snippet NAME)
  add_executable("${NAME}" "${NAME}.cc")
  target_link_libraries("${NAME}" gf0)
endmacro()

add_gf_snippet(doc_full_template)
add_gf_snippet(doc_gamedev_101)
add_gf_snippet(doc_index)
add_gf_snippet(doc_it_moves)
add_gf_snippet(doc_simple_template)

add_executable(gfdev_core_doc
  doc_class_clock.cc
  doc_class_flags.cc
  doc_class_image.cc
  doc_class_rect.cc
  doc_class_time.cc
  doc_struct_circ.cc
  doc_tutorial_id.cc
  doc_tutorial_messages.cc
  main.cc
)

target_link_libraries(gfdev_core_doc
  PRIVATE
    gfcore0
)

add_executable(gfdev_graphics_doc
  doc_class_console.cc
  doc_class_cursor.cc
  doc_class_drawable.cc
  doc_class_entity_container.cc
  doc_class_font.cc
  doc_class_sprite.cc
  doc_class_text.cc
  doc_class_texture.cc
  doc_class_view_container.cc
  doc_class_window.cc
  doc_struct_blend_mode.cc
  doc_struct_event.cc
  main.cc
)

target_link_libraries(gfdev_graphics_doc
  PRIVATE
    gf0
)

add_executable(gfdev_net_doc
  doc_class_socket_selector.cc
  doc_class_tcp_listener.cc
  doc_class_tcp_socket.cc
  doc_class_udp_socket.cc
  main.cc
)

target_link_libraries(gfdev_net_doc
  PRIVATE
    gfnet0
)
