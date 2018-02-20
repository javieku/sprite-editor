#ifndef SHAREDDATAPOINTER_H_
#define SHAREDDATAPOINTER_H_

#include <QExplicitlySharedDataPointer>
#include <QList>
#include <QSharedData>

// Currently SharedDataPointer implementation is Qt ExplicitlySharedDataPointer
#define SharedDataPointerImpl QExplicitlySharedDataPointer

// shared data pointer cast helper
namespace SharedDataPointer
{
template < class X, class Y >
SharedDataPointerImpl< X >
staticCast( SharedDataPointerImpl< Y > p )
{
    return SharedDataPointerImpl< X >( static_cast< X* >( p.data( ) ) );
}
template < class X, class Y >
SharedDataPointerImpl< X >
dynamicCast( SharedDataPointerImpl< Y > p )
{
    return SharedDataPointerImpl< X >( dynamic_cast< X* >( p.data( ) ) );
}
template < class X, class Y >
SharedDataPointerImpl< X >
constCast( SharedDataPointerImpl< Y > p )
{
    return SharedDataPointerImpl< X >( const_cast< X* >( p.data( ) ) );
}
}

/*
 * Given class Xxx declare the following types
 *  - XxxSharedPointer
 *  - XxxCollection
 */

#define SHARED_DATA_POINTER_DECLARE_TYPES( x )           \
    class x;                                             \
    typedef SharedDataPointerImpl< x > x##SharedPointer; \
    typedef QList< x##SharedPointer > x##Collection;

#endif /* SHAREDDATAPOINTER_H_ */
