cmake_minimum_required(VERSION 3.16)

project(RecruitmentTask VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appRecruitmentTask
    main.cpp
    CryptoModel.h
    CryptoModel.cpp
    NetworkWorker.h
    NetworkWorker.cpp
    ViewProxy.h
    ViewProxy.cpp
    Resource.qrc
)

qt_add_qml_module(appRecruitmentTask
    URI RecruitmentTask
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES ViewProxy.h ViewProxy.cpp
    QML_FILES CoinDelegate.qml
    QML_FILES ErrorPopup.qml
    RESOURCES Resource.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRecruitmentTask PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRecruitmentTask
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRecruitmentTask
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appRecruitmentTask
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
