#pragma once

#include <QObject>

namespace sp {
class KeyboardSp: public QObject
{
    Q_OBJECT
    Q_PROPERTY (bool visible READ visible NOTIFY visibleChanged)
    Q_PROPERTY (int  height  READ height  NOTIFY heightChanged)

    public:
        static KeyboardSp& instance();
        static void sendVisibleChanged(bool visible, int height);

        Q_INVOKABLE void show();
        Q_INVOKABLE void hide();

        bool visible() const { return _visible; }
        int  height() const { return _height; }

    protected:
        KeyboardSp();

    signals:
        void visibleChanged(bool visible);
        void heightChanged(int height);

    protected:
        int    _height;
        bool   _visible = false;
};
} // namespace k12 {
