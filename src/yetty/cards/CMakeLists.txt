add_subdirectory(image)
add_subdirectory(texture)
add_subdirectory(plot)
add_subdirectory(hdraw)
add_subdirectory(ydraw)
add_subdirectory(kdraw)
add_subdirectory(jdraw)
add_subdirectory(ydraw-zoo)
add_subdirectory(ymery)
add_subdirectory(markdown)

add_subdirectory(thorvg)

add_subdirectory(python)

option(YETTY_CARD_PDF "Build PDF card (requires PDFium)" ON)
if(YETTY_CARD_PDF)
    add_subdirectory(pdf)
endif()
