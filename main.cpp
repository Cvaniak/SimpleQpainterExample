#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <QtDebug>
#include <QKeyEvent>
#include <QBrush>
#include <QList>

struct Object
{
    float x;
    float y;
    float angle;
    float size;
};

class World : public QWidget {
public:
    World(): _timerId(0){
        _transform.scale(10.0f, -10.0f);
        _transform.translate(0.0f, -64.0f);
        qDebug() << _transform.map(QPointF(0.0f,0.0f));
        qDebug() << _transform.map(QPointF(36.0f,64.0f));

        _objects.append(createBall(15.0f, 15.0f, 5.0f));
    }

    Object createBall(float x, float y, float radius) {
        Object o;
        o.x = x;
        o.y = y;
        o.size = radius;
        return o;
    }

    void paintEvent(QPaintEvent *) {
        QPainter p(this);

        p.setRenderHint(QPainter::Antialiasing, true);
        p.setTransform(_transform);

        QRectF r(0, 0, 36, 64);
        p.fillRect(r, QColor(82, 82, 82));

        foreach(const Object& o, _objects) {
            drawEllipse(&p, o);
        }

    }

    void drawEllipse(QPainter *p, const Object& o) {
        float x = o.x;
        float y = o.y;
        float r = o.size;
        p->setPen(QColor(179, 220, 242));
        p->drawEllipse(QPointF(x, y), r, r);
    }

    void start() {
        if(!_timerId) {
            _timerId = startTimer(1000/60); // 60fps
        }
    }

    void timerEvent(QTimerEvent *event) {
        if(event->timerId() == _timerId) {
            update();
            QPoint p = this->mapFromGlobal(QCursor::pos());
            qDebug() << p << endl;
        }
    }

private:
    int _timerId;
    QTransform _transform;
    QVector<Object> _objects;


protected:
     void keyPressEvent(QKeyEvent *event) override
     {
         if(event->key() == Qt::Key_Up)
         {
             _objects[0].y += 2;
         }
         if(event->key() == Qt::Key_Down)
         {
             _objects[0].y -= 2;
         }
     }
};

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    World world;
    world.resize(360, 640);
    world.show();
    world.start();

    return app.exec();
}
