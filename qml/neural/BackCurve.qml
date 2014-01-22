import QtQuick 2.0

BezierCurve {
    property bool _upCurve: startPoint.y > endPoint.y
    controlPoint1: Qt.point(startPoint.x + Math.abs(startPoint.x - endPoint.x) / 2, startPoint.y)
    controlPoint2: Qt.point(endPoint.x, endPoint.y + (1 - 2*_upCurve) * Math.abs(startPoint.x - endPoint.x) / 2)
}
