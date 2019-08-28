import QtQuick        2.8
import QtQuick.Shapes 1.12

//==============================================================
// Tooth shape to left
//
//                 *
//                 *
//                 *
//                 *
//                ****
//              *******
//            ***********
//          ****************
//        ***********************
//   ***********************************
//
//==============================================================

Item
{
    id: toothShapeItem

    // default values for some tooth-shape properties
    property color itemColor:   "lightgrey"
    property real  maxHeight:   20
    property real  leftOffset:  16

    Shape
    {
        anchors.bottom: toothShapeItem.bottom

        width:  toothShapeItem.implicitWidth
        height: toothShapeItem.implicitHeight

        ShapePath
        {
            strokeWidth: 0
            strokeColor: toothShapeItem.itemColor
            fillColor:   toothShapeItem.itemColor

            startX: -toothShapeItem.leftOffset;
            startY: 0
            PathLine { x: -2; y: -toothShapeItem.height     }
            PathLine { x: -1; y: -toothShapeItem.maxHeight  }
            PathLine { x: 0;  y: -toothShapeItem.height/2   }
            PathLine { x: 1;  y: -toothShapeItem.height/3   }
            PathLine { x: 2;  y: -toothShapeItem.height/3.5 }
            PathLine { x: 3;  y: -toothShapeItem.height/4   }
            PathLine { x: toothShapeItem.width; y: 0 }
        }
    }
}
