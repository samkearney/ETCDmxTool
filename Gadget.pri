# Gadget II

contains(QT_ARCH, i386) {
    LIBGADGET_BASE = gadget/libGadget/Win32
} else {
    LIBGADGET_BASE = gadget/libGadget/x64
}

INCLUDEPATH += $$LIBGADGET_BASE/include

HEADERS += $$LIBGADGET_BASE/include/GadgetDLL.h \
           $$LIBGADGET_BASE/include/RDM_CmdC.h \
           $$LIBGADGET_BASE/include/RdmDeviceInfo.h \
           $$LIBGADGET_BASE/include/rdmEtcConsts.h \
           $$LIBGADGET_BASE/include/uid.h

win32 {
    GADGET_DLL_SRC = $$shell_quote($$system_path($${_PRO_FILE_PWD_}/$$LIBGADGET_BASE/bin/GadgetDll.dll))
    GADGET_DLL_DST = $$shell_quote($$system_path($${_PRO_FILE_PWD_}/install/deploy/GadgetDll.dll))
    LIBS += -L$$PWD/$$LIBGADGET_BASE/lib -lGadgetDll
}
unix {
    SOURCES += gadget/GadgetDLL.cpp
}

win32 {
    # Copy the Gadget DLL to the debug directory for debugging
    QMAKE_POST_LINK += $$QMAKE_COPY $${GADGET_DLL_SRC} $$shell_quote($$system_path($${DESTDIR})) $$escape_expand(\\n\\t)
}
