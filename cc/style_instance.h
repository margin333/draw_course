#ifndef STYLE_INSTANCE_H
#define STYLE_INSTANCE_H

#include <QMutex>

template <class T> class StyleInstance {
public:
    static T &GetIntance()
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        static T instance;
        return instance;
    }

private:
    ~StyleInstance() noexcept {};
    StyleInstance() = default;
    StyleInstance(const StyleInstance &){};
    StyleInstance &operator=(const StyleInstance &) { return *this; };
};

#endif // STYLE_INSTANCE_H
