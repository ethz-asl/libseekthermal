remake_qt4()
remake_qt4_include()

remake_add_executables(LINK seekthermal ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY})
