import QtQuick 2.2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Entity {
    OrbitCameraController {
        camera: Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 45
            aspectRatio: 16/9
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d(-3.0, 0.0, 0.0)
            upVector: Qt.vector3d(0.0, 1.0, 0.0)
            viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
        }
        linearSpeed: 0
        lookSpeed: -180
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0, 0, 0, 0)
                camera: camera
            }
        },
        InputSettings {}
    ]

    Entity {
        id: sphereEntity
        components: [
            SphereMesh {
                radius: 1.0
                generateTangents: true
                rings: 64
                slices: 64
            },
            NormalDiffuseMapMaterial {
                ambient: Qt.rgba(0.8, 0.8, 0.8, 1.0)
                diffuse:  TextureLoader { source: "planetmap.jpg" }
                textureScale: 1.0
                shininess: 0.0
            }
        ]
    }
}
