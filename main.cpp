#include "mainwindow.h"

#include <QApplication>
#include <QGoochMaterial>
#include <QNormalDiffuseMapMaterial>
#include <QPerVertexColorMaterial>
#include <QQuaternion>
#include <QTimer>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>

using namespace Qt3DCore;

class PartGeometry : public QGeometry
{
public:
    PartGeometry(QNode *parent = nullptr)
        : QGeometry(parent)
    {
        auto vertexBuffer = new QBuffer(this);
        auto indexBuffer = new QBuffer(this);

        int stride = (3 + 3 + 4 + 2) * sizeof(float);
        int nVerts = 3;
        int nIndices = 3;

        auto positionAttribute = new QAttribute(this);
        positionAttribute->setName(QAttribute::defaultPositionAttributeName());
        positionAttribute->setVertexBaseType(QAttribute::Float);
        positionAttribute->setVertexSize(3);
        positionAttribute->setAttributeType(QAttribute::VertexAttribute);
        positionAttribute->setBuffer(vertexBuffer);
        positionAttribute->setByteStride(stride);
        positionAttribute->setByteOffset(0 * sizeof(float));
        positionAttribute->setCount(nVerts);

        auto normalAttribute = new QAttribute(this);
        normalAttribute->setName(QAttribute::defaultNormalAttributeName());
        normalAttribute->setVertexBaseType(QAttribute::Float);
        normalAttribute->setVertexSize(3);
        normalAttribute->setAttributeType(QAttribute::VertexAttribute);
        normalAttribute->setBuffer(vertexBuffer);
        normalAttribute->setByteStride(stride);
        normalAttribute->setByteOffset(3 * sizeof(float));
        normalAttribute->setCount(nVerts);

        auto tangentAttribute = new QAttribute(this);
        tangentAttribute->setName(QAttribute::defaultTangentAttributeName());
        tangentAttribute->setVertexBaseType(QAttribute::Float);
        tangentAttribute->setVertexSize(4);
        tangentAttribute->setAttributeType(QAttribute::VertexAttribute);
        tangentAttribute->setBuffer(vertexBuffer);
        tangentAttribute->setByteStride(stride);
        tangentAttribute->setByteOffset((3 + 3) * sizeof(float));
        tangentAttribute->setCount(nVerts);

        auto textureAttribute = new QAttribute(this);
        textureAttribute->setName(QAttribute::defaultTextureCoordinateAttributeName());
        textureAttribute->setVertexBaseType(QAttribute::Float);
        textureAttribute->setVertexSize(2);
        textureAttribute->setAttributeType(QAttribute::VertexAttribute);
        textureAttribute->setBuffer(vertexBuffer);
        textureAttribute->setByteStride(stride);
        textureAttribute->setByteOffset((3 + 3 + 4) * sizeof(float));
        textureAttribute->setCount(nVerts);

        auto indexAttribute = new QAttribute(this);
        indexAttribute->setAttributeType(QAttribute::IndexAttribute);
        indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
        indexAttribute->setBuffer(indexBuffer);
        indexAttribute->setCount(nIndices);

        QByteArray vertexBytes;
        vertexBytes.resize(stride * nVerts);
        auto vertices = reinterpret_cast<float *>(vertexBytes.data());

        // position
        *vertices++ = 0;
        *vertices++ = 0;
        *vertices++ = 0;
        // normal
        *vertices++ = 0;
        *vertices++ = 0;
        *vertices++ = 1;
        // tangents
        *vertices++ = 0.0f;
        *vertices++ = 0.0f;
        *vertices++ = -1.0f;
        *vertices++ = 1.0f;
        // u,v
        *vertices++ = 1;
        *vertices++ = 0;

        // position
        *vertices++ = 0;
        *vertices++ = 1;
        *vertices++ = 0;
        // normal
        *vertices++ = 0;
        *vertices++ = 0;
        *vertices++ = 1;
        // tangents
        *vertices++ = 0.0f;
        *vertices++ = 0.0f;
        *vertices++ = -1.0f;
        *vertices++ = 1.0f;
        // u,v
        *vertices++ = 1;
        *vertices++ = 0;

        // position
        *vertices++ = 1;
        *vertices++ = 0;
        *vertices++ = 0;
        // normal
        *vertices++ = 0;
        *vertices++ = 0;
        *vertices++ = 1;
        // tangents
        *vertices++ = 0.0f;
        *vertices++ = 0.0f;
        *vertices++ = -1.0f;
        *vertices++ = 1.0f;
        // u,v
        *vertices++ = 1;
        *vertices++ = 0;

        vertexBuffer->setData(vertexBytes);

        QByteArray indexBytes;
        indexBytes.resize(nIndices * sizeof(quint16));
        auto indices = reinterpret_cast<quint16 *>(indexBytes.data());
        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;
        indexBuffer->setData(indexBytes);

        addAttribute(positionAttribute);
        addAttribute(normalAttribute);
        addAttribute(tangentAttribute);
        addAttribute(textureAttribute);
        addAttribute(indexAttribute);
    }
};

class PartGeometryView : public QGeometryView
{
public:
    PartGeometryView(QNode *parent)
        : QGeometryView(parent)
    {
        auto geometry = new PartGeometry(this);
        setGeometry(geometry);
    }
};

class Part : public Qt3DRender::QGeometryRenderer
{
public:
    Part(QNode *parent = nullptr)
        : Qt3DRender::QGeometryRenderer(parent)
    {
        auto view = new PartGeometryView(this);
        setView(view);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    auto rootEntity = new QEntity();

    auto cameraEntity = w.tdw()->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    auto camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    auto lightEntity = new QEntity(rootEntity);
    auto light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    auto lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    auto torus = new Part();
    //    torus->setRadius(1.0f);
    //    torus->setMinorRadius(0.4f);
    //    torus->setRings(100);
    //    torus->setSlices(20);

    auto torusMaterial = new Qt3DExtras::QPhongMaterial();

    auto torusTransform = new Qt3DCore::QTransform();
    torusTransform->setScale(2.0f);
    torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
    torusTransform->setTranslation(QVector3D(5.0f, 4.0f, 0.0f));

    auto torusEntity = new QEntity(rootEntity);
    torusEntity->addComponent(torus);
    torusEntity->addComponent(torusMaterial);
    torusEntity->addComponent(torusTransform);

    QTimer::singleShot(0, rootEntity, [rootEntity, &w] {
        qDebug() << "setting root entity";
        w.tdw()->setRootEntity(rootEntity);
    });

    w.show();
    return a.exec();
}
