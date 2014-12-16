TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++1z
SOURCES += main.cpp
LIBS += -lboost_unit_test_framework
OTHER_FILES +=

HEADERS += \
    declare.hpp \
    member_function.hpp \
    member_variable.hpp \
    reflection.hpp \
    static_function.hpp \
    static_variable.hpp \
    any.hpp \
    object.hpp \
    has_class.hpp \
    string_to_tag.hpp \
    reflection_base.hpp \
    test.hpp

