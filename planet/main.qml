import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Scene3D 2.0

Window {
    id: win
    visible: true
    width: 1024
    height: 800
    title: qsTr("Marabunta - Planet")
    color: "#000"

    function toggleFullscreen() {
        win.visibility = (win.visibility == Window.FullScreen) ? Window.Windowed : Window.FullScreen;
    }

    Image {
        anchors.fill: parent
        source: "starfield.jpg"

        Scene3D {
            anchors.fill: parent
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Keys.onPressed: {
                if ([Qt.Key_F, Qt.Key_F11].indexOf(event.key) !== -1) {
                    toggleFullscreen();
                    event.accepted = true;

                } else if (event.key == Qt.Key_Space) {
                    controls.opened = !controls.opened;

                } else if (event.key == Qt.Key_Escape) {
                    if (win.visibility == Window.Windowed)
                        Qt.quit();
                    else
                        win.visibility = Window.Windowed;
                    event.accepted = true;
                }
            }

            Planet {}
        }
    }
    MouseArea {
        id: globalMouseArea
        anchors.fill: parent
        onDoubleClicked: toggleFullscreen()
        onClicked: controls.opened = !controls.opened
    }
    Item {
        id: controls
        property bool opened: false
        width: 200
        height: parent.height
        x: -width * !opened
        Behavior on x {
            NumberAnimation {}
        }

        Rectangle {
            anchors.fill: parent
            color: "#43144a"
            opacity: 0.8
        }
    }
}
