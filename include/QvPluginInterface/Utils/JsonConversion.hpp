#pragma once
#include "ForEachMacros.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QVariant>
#include <type_traits>

template<typename T>
struct Bindable;

#define _QJS_FUNC_COMPAREImpl(x) (this->x == another.x)
#define QJS_FUNC_COMPARE(CLASS, ...)                                                                                                                                     \
    bool operator==(const CLASS &another) const                                                                                                                          \
    {                                                                                                                                                                    \
        return FOR_EACH_DELIM(_QJS_FUNC_COMPAREImpl, &&, __VA_ARGS__);                                                                                                   \
    }

#define __FROMJSON_B(name) name::loadJson(json);
#define __FROMJSON_F(name) ::JsonStructHelper::Deserialize(this->name, json.toObject()[#name]);
#define __FROMJSON_P(name) ::JsonStructHelper::Deserialize(this->name, json.toObject()[#name]);

#define __TOJSON_P(name) json.insert(#name, ::JsonStructHelper::Serialize(this->name));
#define __TOJSON_F(name) json.insert(#name, ::JsonStructHelper::Serialize(this->name));
#define __TOJSON_B(base) ::JsonStructHelper::MergeJson(json, base::toJson());

// ============================================================================================
// Load JSON Wrapper
#define _QJS_FROM_JSON_F(...) FOR_EACH_2(__FROMJSON_F, __VA_ARGS__)
#define _QJS_FROM_JSON_B(...) FOR_EACH_2(__FROMJSON_B, __VA_ARGS__)
#define _QJS_FROM_JSON_P(...) FOR_EACH_2(__FROMJSON_P, __VA_ARGS__)
#define _QJS_FROM_JSON_BF(option) _QJS_FROM_JSON_##option

// ============================================================================================
// To JSON Wrapper
#define _QJS_TO_JSON_F(...) FOR_EACH_2(__TOJSON_F, __VA_ARGS__)
#define _QJS_TO_JSON_B(...) FOR_EACH_2(__TOJSON_B, __VA_ARGS__)
#define _QJS_TO_JSON_P(...) FOR_EACH_2(__TOJSON_P, __VA_ARGS__)
#define _QJS_TO_JSON_BF(option) _QJS_TO_JSON_##option

// ============================================================================================
// QJsonStruct main macro
#define QJS_FUNC_JSON(...)                                                                                                                                               \
    QJsonObject toJson() const                                                                                                                                           \
    {                                                                                                                                                                    \
        QJsonObject json;                                                                                                                                                \
        FOR_EACH(_QJS_TO_JSON_BF, __VA_ARGS__);                                                                                                                          \
        return json;                                                                                                                                                     \
    }                                                                                                                                                                    \
    void loadJson(const QJsonValue &json)                                                                                                                                \
    {                                                                                                                                                                    \
        FOR_EACH(_QJS_FROM_JSON_BF, __VA_ARGS__);                                                                                                                        \
    }

template<typename T>
struct QJsonStructSerializer
{
    static void Deserialize(T &t, const QJsonValue &d);
    static QJsonValue Serialize(const T &t);
};

struct JsonStructHelper
{

    // clang-format off
    template<typename, typename = void> struct has_toJson : public std::false_type {};
    template<typename C> struct has_toJson<C, typename std::enable_if_t<std::is_convertible_v<decltype(std::declval<C>().toJson()), QJsonValue>>> : public std::true_type {};
    
    template<typename, typename = void> struct has_loadJson : public std::false_type {};
    template<typename C> struct has_loadJson<C, typename std::enable_if_t<std::is_void_v<decltype(std::declval<C>().loadJson(std::declval<const QJsonValue&>()))>>> : public std::true_type {};
    
    template <class T, std::size_t = sizeof(T)>
    static std::true_type is_complete_impl(T *);
    static std::false_type is_complete_impl(...);
    template <class T> using is_complete = decltype(is_complete_impl(std::declval<T*>()));
    
    template <class, template <class> class>
    struct is_instance : public std::false_type {};
    
    template <class T, template <class> class U>
    struct is_instance<U<T>, U> : public std::true_type {};
    
    template <class T>
    using is_bindable_template = is_instance<T, Bindable>;

    // clang-format on

    static void MergeJson(QJsonObject &mergeTo, const QJsonObject &mergeIn)
    {
        for (auto it = mergeIn.constBegin(); it != mergeIn.constEnd(); it++)
            mergeTo[it.key()] = it.value();
    }

    // =========================== Deserialize ===========================

    // clang-format off
#define LOAD_SIMPLE_FUNC(type, convert_func) static void Deserialize(type &t, const QJsonValue &d) { t = d.convert_func; }
    LOAD_SIMPLE_FUNC(QString, toString());
    LOAD_SIMPLE_FUNC(QChar, toVariant().toChar());
    LOAD_SIMPLE_FUNC(std::string, toString().toStdString());
    LOAD_SIMPLE_FUNC(std::wstring, toString().toStdWString());
    LOAD_SIMPLE_FUNC(bool, toBool());
    LOAD_SIMPLE_FUNC(double, toDouble());
    LOAD_SIMPLE_FUNC(float, toVariant().toFloat());
    LOAD_SIMPLE_FUNC(int, toInt());
    LOAD_SIMPLE_FUNC(long, toVariant().toLongLong());
    LOAD_SIMPLE_FUNC(long long, toVariant().toLongLong());
    LOAD_SIMPLE_FUNC(unsigned int, toVariant().toUInt());
    LOAD_SIMPLE_FUNC(unsigned long, toVariant().toULongLong());
    LOAD_SIMPLE_FUNC(unsigned long long, toVariant().toULongLong());
#undef LOAD_SIMPLE_FUNC
    // clang-format on

    template<typename T>
    static void Deserialize(QList<T> &t, const QJsonValue &d)
    {
        t.clear();
        for (const auto &val : d.toArray())
        {
            T data;
            Deserialize(data, val);
            t.push_back(data);
        }
    }

    template<typename TKey, typename TValue>
    static void Deserialize(QMap<TKey, TValue> &t, const QJsonValue &d)
    {
        t.clear();
        const auto &jsonObject = d.toObject();
        TKey keyVal;
        TValue valueVal;
        for (const auto &key : jsonObject.keys())
        {
            Deserialize(keyVal, key);
            Deserialize(valueVal, jsonObject.value(key));
            t.insert(keyVal, valueVal);
        }
    }

    template<typename T>
    static void Deserialize(T &t, const QJsonValue &d)
    {
        using _T = std::remove_cv_t<std::remove_reference_t<T>>;
        if constexpr (std::is_enum_v<_T>)
            t = (T) d.toInt();
        else if constexpr (std::is_same_v<_T, QJsonObject>)
            t = d.toObject();
        else if constexpr (std::is_same_v<_T, QJsonArray>)
            t = d.toArray();
        else if constexpr (is_bindable_template<_T>::value)
            Deserialize(*t, d);
        else if constexpr (has_loadJson<_T>::value)
            t.loadJson(d);
        else if constexpr (is_complete<QJsonStructSerializer<_T>>::value)
            QJsonStructSerializer<_T>::Deserialize(t, d);
        else
            assert(false);
    }

    // =========================== Serialize ===========================

    // clang-format off
#define STORE_SIMPLE_FUNC(type) static QJsonValue Serialize(const type &t) { return QJsonValue{ t }; }
    STORE_SIMPLE_FUNC(int);
    STORE_SIMPLE_FUNC(bool);
    STORE_SIMPLE_FUNC(QJsonArray);
    STORE_SIMPLE_FUNC(QJsonObject);
    STORE_SIMPLE_FUNC(QString);
    STORE_SIMPLE_FUNC(long long);
    STORE_SIMPLE_FUNC(float);
    STORE_SIMPLE_FUNC(double);
#undef STORE_SIMPLE_FUNC
    // clang-format on

    // clang-format off
#define STORE_VARIANT_FUNC(type, func) static QJsonValue Serialize(const type &t) { return QJsonValue::fromVariant(func); }
    STORE_VARIANT_FUNC(std::string, QString::fromStdString(t))
    STORE_VARIANT_FUNC(std::wstring, QString::fromStdWString(t))
    STORE_VARIANT_FUNC(long, QVariant::fromValue<long>(t))
    STORE_VARIANT_FUNC(unsigned int, QVariant::fromValue<unsigned int>(t))
    STORE_VARIANT_FUNC(unsigned long, QVariant::fromValue<unsigned long>(t))
    STORE_VARIANT_FUNC(unsigned long long, QVariant::fromValue<unsigned long long>(t))
#undef STORE_VARIANT_FUNC
    // clang-format on

    template<typename TValue>
    static QJsonValue Serialize(const QMap<QString, TValue> &t)
    {
        QJsonObject mapObject;
        for (const auto &key : t.keys())
        {
            auto valueVal = Serialize(t.value(key));
            mapObject.insert(key, valueVal);
        }
        return mapObject;
    }

    template<typename T>
    static QJsonValue Serialize(const QList<T> &t)
    {
        QJsonArray listObject;
        for (const auto &item : t)
        {
            listObject.push_back(Serialize(item));
        }
        return listObject;
    }

    template<typename T>
    static QJsonValue Serialize(const T &t)
    {
        using _T = std::remove_cv_t<std::remove_reference_t<T>>;
        if constexpr (std::is_enum_v<T>)
            return (int) t;
        else if constexpr (std::is_same_v<_T, QJsonObject>)
            return t;
        else if constexpr (std::is_same_v<_T, QJsonArray>)
            return t;
        else if constexpr (std::is_convertible_v<_T, QJsonObject>)
            return (QJsonObject) t;
        else if constexpr (std::is_convertible_v<_T, QJsonValue>)
            return (QJsonValue) t;
        else if constexpr (is_bindable_template<_T>::value)
            return Serialize(*t);
        else if constexpr (has_toJson<_T>::value)
            return t.toJson();
        else if constexpr (is_complete<QJsonStructSerializer<_T>>::value)
            return ::QJsonStructSerializer<_T>::Serialize(t);
        assert(false);
        return {};
    }
};
