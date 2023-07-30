// Includes relevant modules used by the QML
import QtQuick 2.14
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.14
//KDE
import org.kde.kirigami 2.13 as Kirigami
//Custom Instances
import kSTL 1.0
import kSTLErrorMessage 1.0

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

    //File Selector
    FileDialog{
        id: fileSelector
        title: "Select a STL Mesh"
        nameFilters: [ "Stereolitography Meshes (*.stl *.ast)", "All files (*)" ]
        folder: shortcuts.home
        selectMultiple: false
        selectFolder: false
        onAccepted:{
            console.log(fileSelector.fileUrl);
            renderarea.file = fileSelector.fileUrl
            filenamedisp.text = fileSelector.fileUrl
        }

        onRejected: {
            console.log("Rejected");
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
                        id: filenamedisp
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
                    Kirigami.Heading {
                      text: i18n("Dimesions")
                      level: 3
                    }
                    Controls.Label{
                        //text: "0 x 0 x 0 mm"
                        text: initialFileName;
                        color: Kirigami.Theme.disabledTextColor
                    }
                }
            }
        }

    // Initial page to be loaded on app load
    pageStack.initialPage: Kirigami.Page {
        title: "kSTL"
        id: mainPage
        //Page must be transparent to see underlying OpenGL window
        background: null
        // Padding must be set to 0 to show error messages correctly
        padding: Kirigami.Units.largeSpacing

        actions {
                main: Kirigami.Action {
                    text: i18n("Open")
                    iconName: "folder-open"
                    shortcut: StandardKey.Open
                    onTriggered: fileSelector.open()
                }
                left: Kirigami.Action {
                    text: i18n("Material Color")
                    iconName: "preferences-desktop-theme"
                    onTriggered: colorDialog.open()
                }
                right: Kirigami.Action {
                    text: i18n("Statistics")
                    icon.name: "dialog-information"
                    onTriggered: metaSheet.open()
                }
                contextualActions: [
                    Kirigami.Action {
                        text: i18n("Restore View")
                        icon.name: "view-refresh"
                        onTriggered:{
                            showPassiveNotification(i18n("Reset model view"))
                            //Spherical coordinates; Except the x value is supposed to be the factor for the radius
                            renderarea.view = Qt.vector3d(1,45,45)
                        }
                    }
                ]
            }
        MouseArea {
            id: mousearea
            //Layout.fillWidth: true
            //height: 480
            width: root.width;
            height: root.height;

            acceptedButtons: Qt.LeftButton | Qt.MiddleButton

            property var chacheX: 0
            property var chacheY: 0
            onPressed: {
                //console.log("Pressed!")
                hoverEnabled: true
                chacheX = mouseX
                chacheY = mouseY
            }

            onReleased: {
                hoverEnabled: false
                //console.log("Released")
            }

            onPositionChanged: {
                if(mousearea.pressedButtons & Qt.LeftButton && mousearea.pressedButtons & Qt.MiddleButton){
                    chacheX = mouseX
                    chacheY = mouseY
                    return;
                }else if(mousearea.pressedButtons & Qt.LeftButton){
                    renderarea.deltaRotation = Qt.vector2d(mouseX - chacheX, mouseY - chacheY)
                    chacheX = mouseX
                    chacheY = mouseY
                }else{
                    renderarea.deltaTransform = Qt.vector2d(mouseX - chacheX, mouseY - chacheY)
                    chacheX = mouseX
                    chacheY = mouseY
                }
            }

            onWheel: {
                //console.log("Wheel: " + wheel.angleDelta.y)
                renderarea.deltaZoom = wheel.angleDelta.y
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

                Kirigami.InlineMessage {
                    Layout.fillWidth: true
                    Layout.leftMargin: Kirigami.Units.largeSpacing
                    Layout.rightMargin: 2 * Kirigami.Units.largeSpacing
                    Layout.bottomMargin: Kirigami.Units.smallSpacing

                    text: ErrorHandle.message
                    type: Kirigami.MessageType.Warning
                    showCloseButton: true
                    visible: true
                }
                ColumnLayout {
                    GlRenderArea {
                        id: renderarea;
                        filament: Kirigami.Theme.focusColor;
                        file: initialFileName;
                        SequentialAnimation on t {
                            NumberAnimation { to: 1; duration: 2500;}
                            loops: Animation.Infinite
                            running: true
                        }
                    }
                }

            }
        }
    }
}
