#pragma once

#include "JsonConversion.hpp"

#include <QJsonValue>
#include <QObject>
#include <QVariant>

class INotifiable : public QObject
{
    Q_OBJECT
  public:
    explicit INotifiable() : QObject(){};
    explicit INotifiable(INotifiable &) : QObject(){};
  signals:
    void notify();
};

template<typename T>
struct Bindable : public INotifiable
{
  public:
    typedef T value_type;
    const bool isRequired = false;
    Bindable(const Bindable<T> &&another) : value(another.defaultValue), defaultValue(another.defaultValue), isRequired(another.isRequired){};
    Bindable(const Bindable<T> &another) : value(another.defaultValue), defaultValue(defaultValue), isRequired(isRequired){};
    Bindable(T def = T{}, bool required = false) : value(def), defaultValue(def), isRequired(required){};

    // clang-format off
    const T* operator->() const { return &value; }
          T* operator->()       { return &value; }

    const T& operator*() const { return value; }
          T& operator*()       { return value; }

    operator const T()   const { return value; }
    operator       T()         { return value; }

    T & operator=(const T& f)           { return set(f); }
    T & operator=(const T&&f)           { return set(std::move(f)); }
    T & operator=(const Bindable<T> &f) { return set(f.value); }

    Bindable<T> &operator++() { value++; return *this; }
    Bindable<T> &operator--() { value--; return *this; } 

    template<typename Y> void operator<<(const Y &another) { value << another; }
    template<typename Y> void operator>>(const Y &another) { value >> another; }
    template<typename V> T operator+=(const V &v) { value += v; return value; }
    template<typename V> T operator-=(const V &v) { value -= v; return value; }
    template<typename V> T operator*=(const V &v) { value *= v; return value; }
    template<typename V> T operator/=(const V &v) { value /= v; return value; }
    template<typename V> T operator&=(const V &v) { value &= v; return value; }
    template<typename V> T operator%=(const V &v) { value %= v; return value; }
    template<typename V> T operator|=(const V &v) { value |= v; return value; }

    QJsonValue toJson() const { return JsonStructHelper::Serialize(value); }
    void loadJson(const QJsonValue &val) { JsonStructHelper::Deserialize(value, val); }
    // clang-format on

    // clang-format off
    bool operator==(const T& val) const { return val == value ; }
    bool operator!=(const T& val) const { return val != value ; }
    bool operator==(const Bindable<T>& left) const { return   left.value == value ; }
    bool operator!=(const Bindable<T>& left) const { return !(left.value == value); }
    // clang-format on

    bool isDefault() const
    {
        return defaultValue == value;
    }

  public:
    void EmitNotify()
    {
        emit notify();
    }

    template<typename TCallback>
    inline void Observe(TCallback callback) const
    {
        static_assert(std::is_invocable<TCallback, const T &>::value, "Callback function must be callable with a const reference parameter T");
        QObject::connect(this, &INotifiable::notify, [this, callback] { callback(value); });
    }

    inline void WriteBind(Bindable<T> *propTarget)
    {
        propTarget->set(value);
        QObject::connect(this, &INotifiable::notify, [this, propTarget]() { propTarget->set(value); });
    }

    inline void ReadBind(const Bindable<T> *target)
    {
        QObject::connect(target, &INotifiable::notify, [this, target]() { set(target->value); });
    }

    inline void ReadWriteBind(Bindable<T> *target)
    {
        WriteBind(target);
        ReadBind(target);
    }

    ///
    /// \brief Change the value of target property when this value has changed.
    ///
    template<typename TTarget>
    inline void WriteBind(TTarget *target, const char *target_prop)
    {
        static_assert(std::is_base_of_v<QObject, TTarget>, "Wrong Usage: Target must be a QObject");
        Q_ASSERT_X(qMetaTypeId<T>() == ((QObject *) target)->property(target_prop).metaType().id(), "WriteBind", "ID doesn't match.");

        // Firstly, sync target properties.
        ((QObject *) target)->setProperty(target_prop, value);

        QObject::connect(this, &INotifiable::notify,
                         [this, target, target_prop]()
                         {
                             if (auto obj = dynamic_cast<QObject *>(target); obj)
                                 obj->setProperty(target_prop, value);
                         });
    }

    ///
    /// \brief Change the value of current property of something happened in target, triggered by target signal
    ///
    template<typename TTarget, typename Func>
    inline void ReadBind(const TTarget *target, const char *target_prop, Func trigger_signal)
    {
        static_assert(std::is_base_of_v<QObject, TTarget>, "Wrong Usage: Target must be a QObject");
        Q_ASSERT_X(qMetaTypeId<T>() == ((QObject *) target)->property(target_prop).metaType().id(), "ReadBind", "ID doesn't match.");

        QObject::connect(target, trigger_signal,
                         [this, target, target_prop]()
                         {
                             if (auto obj = dynamic_cast<const QObject *>(target); obj)
                                 set(obj->property(target_prop).value<T>());
                         });
    }

    template<typename TTarget, typename Func>
    inline void ReadWriteBind(TTarget *target, const char *target_prop, Func trigger_signal)
    {
        WriteBind(target, target_prop);
        ReadBind(target, target_prop, trigger_signal);
    }

  private:
    T &set(const T &v)
    {
        if (value == v)
            return value;
        value = v;
        emit notify();
        return value;
    }
    T value;
    const T defaultValue;
};
