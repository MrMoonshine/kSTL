// Includes relevant modules used by the QML
import QtQml 2.15
import QtQuick 2.14
import QtQuick.Controls 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.14
import org.kde.kirigami 2.13 as Kirigami


// Base element, provides basic features needed for all kirigami applications
Kirigami.ApplicationWindow {
    // ID provides unique identifier to reference this element
    id: root

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
                onTriggered: showPassiveNotification(i18n("About information"))
            }
        ]
    }

    // Initial page to be loaded on app load
    pageStack.initialPage: Kirigami.Page {
        title: "kSTL"
        actions {
                main: Kirigami.Action {
                    text: i18n("Open")
                    iconName: "folder-open"
                    shortcut: StandardKey.Open
                    onTriggered: showPassiveNotification(i18n("Open File Action"))
                }
                left: Kirigami.Action {
                    text: i18n("Material Colour")
                    iconName: "preferences-desktop-theme"
                    onTriggered: showPassiveNotification(i18n("Colour Select"))
                }
                right: Kirigami.Action {
                    icon.name: "network-offline"
                    onTriggered: showPassiveNotification(i18n("Right action triggered"))
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
        Controls.Label {
            // Center label horizontally and vertically within parent element
            anchors.centerIn: parent
            text: i18n("I will be Vulkan :3!")
        }
    }
}
