import QtQuick        2.8
import QtQuick.Shapes 1.12

//==============================================================
// Tooth shape to left
//
//                             *
//                            **
//                           ***
//                          ****
//                         *****
//                        ******
//                       *******
//                      ********
//                     *********
//                    **********
//                  ************
//               ***************
//          ********************
//   ***************************
//
//==============================================================

Item
{
    id: toothShapeItem

    // default values for some tooth-shape properties
    property color itemColor:   "lightgrey"
    property real  maxHeight:   20
    property real  rightOffset: 16

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

            capStyle:  ShapePath.FlatCap
            joinStyle: ShapePath.RoundJoin

            startX: toothShapeItem.rightOffset + toothShapeItem.width
            startY: 0
            PathLine { x: toothShapeItem.rightOffset + toothShapeItem.width; y: -toothShapeItem.maxHeight  }
            PathLine { x: 2 + toothShapeItem.width; y: -toothShapeItem.height     }
            PathLine { x: toothShapeItem.width;     y: -toothShapeItem.height/2   }
            PathLine { x: toothShapeItem.width - 1; y: -toothShapeItem.height/3   }
            PathLine { x: toothShapeItem.width - 2; y: -toothShapeItem.height/3.5 }
            PathLine { x: toothShapeItem.width - 3; y: -toothShapeItem.height/4   }
            PathLine { x: 0; y: 0 }
        }
    }
}
