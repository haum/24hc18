import QtQuick 2.0

Item {
    property alias anim: ani
    Text {
        id: acronym
        visible: false
        height: parent.height
        text: "R\nT\nF\nM"
        color: Qt.rgba(1, 1, 1, 1)
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 200
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        id: expand
        visible: false
        anchors.left: acronym.right
        height: parent.height
        text: "egarde\nes\nourmis\nourir"
        color: Qt.rgba(1, 1, 1, 1)
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 200
    }
    SequentialAnimation {
        id: ani
        PropertyAction { target: acronym; property: "y"; value: -target.height }
        PropertyAction { target: acronym; property: "opacity"; value: 1 }
        PropertyAction { target: acronym; property: "visible"; value: true }
        PropertyAction { target: expand; property: "visible"; value: false }
        NumberAnimation {
            target: acronym
            property: "y"
            to: 0
            duration: 1500
            easing.type: Easing.InOutQuad
        }
        PropertyAction { target: expand; property: "opacity"; value: 0 }
        PropertyAction { target: expand; property: "visible"; value: true }
        NumberAnimation {
            target: expand
            property: "opacity"
            duration: 800
            to: 1
            easing.type: Easing.InOutQuad
        }
        PauseAnimation { duration: 1000 }
        NumberAnimation {
            targets: [acronym, expand]
            property: "opacity"
            duration: 800
            to: 0
            easing.type: Easing.InOutQuad
        }
        PropertyAction { targets: [acronym, expand]; property: "visible"; value: false }
    }
}
