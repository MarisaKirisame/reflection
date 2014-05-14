TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++1y
SOURCES += main.cpp

OTHER_FILES +=

HEADERS += \
    declare.hpp \
    example.hpp \
    member_function.hpp \
    member_variable.hpp \
    reflection.hpp \
    static_function.hpp \
    static_variable.hpp \
    any.hpp \
    object.hpp \
    has_class.hpp

