import QtQuick 2.0
import QtQuick.Controls 2.0
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
            id: scene3d
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

            Planet {
                id: planet
            }
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

            Column {
                anchors.fill: parent
                anchors.margins: 10
                Text {
                    text: qsTr("Connected: ") + (gcomm && gcomm.connected)
                    color: Qt.rgba(1, 1, 1, 1)
                }
                Text {
                    text: qsTr("Rotation:")
                    color: Qt.rgba(1, 1, 1, 1)
                }
                Slider {
                    id: slRotation
                    width: parent.width
                    visible: !cbRotation.checked
                    onActiveFocusChanged: if (activeFocus) scene3d.forceActiveFocus();
                    onPositionChanged: planet.rotationAngle = position * 360
                }
                Row {
                    CheckBox {
                        id: cbRotation
                        checked: true
                        onActiveFocusChanged: if (activeFocus) scene3d.forceActiveFocus();

                        NumberAnimation {
                            target: planet
                            property: "rotationAngle"
                            from: 0
                            to: 360
                            duration: 5*60*1000
                            loops: Animation.Infinite
                            running: cbRotation.checked
                        }
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Automatic")
                        color: Qt.rgba(1, 1, 1, 1)
                    }
                }
            }
        }
    }
}
