#pragma once

#include <QSGGeometryNode>
#include <QImage>

namespace sp {
    class ImageSpNode: public QSGGeometryNode
    {
        public:
            ImageSpNode(int vertexAtCorner = 20);
            ~ImageSpNode();

            void setImage(const QImage &image);
            void setMipmap(bool mipmap);

        protected:
            int _vertexAtCorner;
            int _segmentCount;
            bool _mipmap = true;
    };
} // namespace sp {
