// Includes relevant modules used by the QML
import QtQml 2.15
import QtQuick 2.14
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.14
//KDE
import org.kde.kirigami 2.13 as Kirigami
//Custom Instances
import kSTL 1.0

// Base element, provides basic features needed for all kirigami applications
Kirigami.ApplicationWindow {
    // ID provides unique identifier to reference this element
    id: root
    objectName: "root"
    // Window title
    title: "kSTL"
    // i18nc is useful for adding context for translators, also lets strings be changed for different languages
    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: false
        actions: [
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "gtk-quit"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            },
            Kirigami.Action {
                text: i18n("About")
                icon.name: "gtk-about"
                onTriggered: pageStack.layers.push(aboutPage)
            }
        ]
    }
    //About page.
    Component {
            id: aboutPage

            Kirigami.AboutPage {
                aboutData: Controller.aboutData
            }
        }

    //Color Picker
    ColorDialog {
        id: colorDialog
        title: "Select your material color"
        color: Kirigami.Theme.highlightColor
        onAccepted: {
            console.log("Material Color is: " + colorDialog.color)
            renderarea.filament = colorDialog.color;
        }
        onRejected: {
            console.log("No color selected")
        }
    }


    Kirigami.OverlaySheet {
            id: metaSheet
            header: Kirigami.Heading{
                text: i18n("Meta Informations")
                level: 1
            }

            GridLayout{
                id: metaInformationsLayout
                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                }
                rowSpacing: Kirigami.Units.largeSpacing
                columnSpacing: Kirigami.Units.largeSpacing
                columns: 1
                ColumnLayout {
                    Kirigami.Heading {
                      text: i18n("Filename")
                      level: 3
                    }
                    Controls.Label{
                        text: "none"
                        color: Kirigami.Theme.disabledTextColor
                    }
                    Kirigami.Heading {
                      text: i18n("File Size")
                      level: 3
                    }
                    Controls.Label{
                        text: "none"
                        color: Kirigami.Theme.disabledTextColor
                    }
                    Kirigami.Heading {
                      text: i18n("Vertices")
                      level: 3
                    }
                    Controls.Label{
                        text: "0"
                        color: Kirigami.Theme.disabledTextColor
                    }
                }
            }
        }

    // Initial page to be loaded on app load
    pageStack.initialPage: Kirigami.Page {
        title: "kSTL"
        id: mainPage
        //Page must be transparent to see underlying vulkan window
        background: null

        actions {
                main: Kirigami.Action {
                    text: i18n("Open")
                    iconName: "folder-open"
                    shortcut: StandardKey.Open
                    onTriggered: showPassiveNotification(i18n("Open File Action"))
                }
                left: Kirigami.Action {
                    text: i18n("Material color")
                    iconName: "preferences-desktop-theme"
                    onTriggered: colorDialog.open()
                }
                right: Kirigami.Action {
                    icon.name: "dialog-information"
                    onTriggered: metaSheet.open()
                }
                contextualActions: [
                    Kirigami.Action {
                        text: i18n("W.I.P Action 1")
                        icon.name: "network-offline"
                        onTriggered: showPassiveNotification(i18n("Contextual action 1 clicked"))
                    },
                    Kirigami.Action {
                        text: i18n("W.I.P Action 2")
                        icon.name: "network-offline"
                        enabled: false
                    }
                ]
            }
        GridLayout{
            id: mainContentLayout
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            rowSpacing: Kirigami.Units.largeSpacing
            columnSpacing: Kirigami.Units.largeSpacing
            columns: 1

            ColumnLayout {
                Kirigami.InlineMessage {
                    Layout.fillWidth: true
                    text: "Invalid STL mesh"
                    type: Kirigami.MessageType.Warning
                    visible: false
                }

                Kirigami.InlineMessage {
                    Layout.fillWidth: true
                    text: "Fatal error while loading STL file"
                    type: Kirigami.MessageType.Error
                    visible: false
                }

                Kirigami.InlineMessage {
                    Layout.fillWidth: true
                    text: "A Pink Cube, Rendered with OpenGL"
                    type: Kirigami.MessageType.Information
                    visible: true
                }

                MouseArea {
                    Layout.fillWidth: true;
                    height: 480;

                    GlRenderArea {
                        id: renderarea;
                        filament: Kirigami.Theme.highlightColor;
                        SequentialAnimation on t {
                            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
                            loops: Animation.Infinite
                            running: true
                        }
                    }
                }
            }
        }
    }
}
