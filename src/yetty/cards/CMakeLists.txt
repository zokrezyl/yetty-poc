# Card subdirectories - each creates its own library
add_subdirectory(texture)
add_subdirectory(image)
add_subdirectory(plot)
add_subdirectory(ydraw)
add_subdirectory(diagram)
add_subdirectory(ydraw-zoo)
add_subdirectory(ydraw-maze)
add_subdirectory(ytext)
add_subdirectory(ypdf)
add_subdirectory(yhtml)
add_subdirectory(ygui)
add_subdirectory(markdown)
add_subdirectory(qrcode)

# Aggregate library linking all cards
add_library(yetty_cards INTERFACE)
target_link_libraries(yetty_cards INTERFACE
    yetty_card_texture
    yetty_card_image
    yetty_card_plot
    yetty_card_ydraw
    yetty_card_diagram
    yetty_card_ydraw_zoo
    yetty_card_ydraw_maze
    yetty_card_ytext
    yetty_card_ypdf
    yetty_card_yhtml
    yetty_card_ygui
    yetty_card_markdown
    yetty_card_qrcode
)
