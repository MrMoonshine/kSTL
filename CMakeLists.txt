cmake_minimum_required(VERSION 3.1.0)

project(kSTL)

add_definitions(
    -DAPP_TITLE="kSTL"
    -DDEFAULT_APP_HEIGHT=600
    -DDEFAULT_APP_WIDTH=800
    -DDEFAULT_RENDER_HEIGHT=1600
    -DDEFAULT_RENDER_WIDTH=900
    -DTEST_IMAGE="/home/david/Bilder/carBackground/67e77813c76ba4e924d2abcd952820fc.jpg"
)

file(GLOB SRC_DIR
    "src/*.cpp"
    "inc/*.hpp"
)

file(GLOB INC_DIR
    ./inc
)

file(GLOB RSRC_FILES
    src/resources.qrc
)

include(FeatureSummary)

set(QT5_MIN_VERSION 5.12)
set(KF5_MIN_VERSION 5.65)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 ${QT5_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 CoreAddons I18n KIO)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMPoQmTools)
include(ECMQMLModules)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

include_directories(${INC_DIR})

#Include files must be sadly added as executable to be recognised by the MOC
add_executable(${PROJECT_NAME} ${SRC_DIR} ${RSRC_FILES})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n
    KF5::Kirigami2
    KF5::CoreAddons
    KF5::KIOCore
    KF5::KIOWidgets

)
install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

#install(PROGRAMS org.kde.kstl.desktop DESTINATION ${KDE_INSTALL_APPDIR})
#install(FILES org.kde.kstl.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
