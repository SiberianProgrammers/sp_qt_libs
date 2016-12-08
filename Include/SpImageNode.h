#pragma once

#include <QSGGeometryNode>
#include <QImage>

namespace sp {
    class SpImageNode: public QSGGeometryNode
    {
        public:
            SpImageNode(int vertexAtCorner = 20);
            ~SpImageNode();

            void setImage(const QImage &image);

        protected:
            int _vertexAtCorner;
            int _segmentCount;
    };
} // namespace sp {
