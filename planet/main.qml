import QtQuick 2.0
import QtQuick.Scene3D 2.0

Item {
    Image {
        anchors.fill: parent
        source: "starfield.jpg"

        Scene3D {
            anchors.fill: parent
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Planet {}
        }
    }
}
