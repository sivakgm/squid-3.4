#ifndef SQUID_SRC_HTTP_METHODTYPE_H
#define SQUID_SRC_HTTP_METHODTYPE_H

namespace Http
{

/*
 * The IANA registry for HTTP status codes can be found at:
 * http://www.iana.org/assignments/http-methods/http-methods.xhtml
 */
typedef enum _method_t {
    METHOD_NONE = 0,

#if NO_SPECIAL_HANDLING
    // RFC 2068
    METHOD_LINK,
    METHOD_UNLINK,
#endif

    // RFC 2616 (HTTP)
    METHOD_GET,
    METHOD_POST,
    METHOD_PUT,
    METHOD_HEAD,
    METHOD_CONNECT,
    METHOD_TRACE,
    METHOD_OPTIONS,
    METHOD_DELETE,

    // RFC 3253
    METHOD_CHECKOUT,
    METHOD_CHECKIN,
    METHOD_UNCHECKOUT,
    METHOD_MKWORKSPACE,
    METHOD_VERSION_CONTROL,
    METHOD_REPORT,
    METHOD_UPDATE,
    METHOD_LABEL,
    METHOD_MERGE,
    METHOD_BASELINE_CONTROL,
    METHOD_MKACTIVITY,

#if NO_SPECIAL_HANDLING
    // RFC 3648
    METHOD_ORDERPATCH,

    // RFC 3744
    METHOD_ACL,

    // RFC 4437
    METHOD_MKREDIRECTREF,
    METHOD_UPDATEREDIRECTREF,

    // RFC 4791
    METHOD_MKCALENDAR,
#endif

    // RFC 4918 (WebDAV)
    METHOD_PROPFIND,
    METHOD_PROPPATCH,
    METHOD_MKCOL,
    METHOD_COPY,
    METHOD_MOVE,
    METHOD_LOCK,
    METHOD_UNLOCK,

    // RFC 5323
    METHOD_SEARCH,

#if NO_SPECIAL_HANDLING
    // RFC 5789
    METHOD_PATCH,

    // RFC 5842
    METHOD_BIND,
    METHOD_REBIND,
    METHOD_UNBIND,
#endif

    // Squid extension methods
    METHOD_PURGE,
    METHOD_OTHER,
    METHOD_ENUM_END  // MUST be last, (yuck) this is used as an array-initialization index constant!
} MethodType;

extern const char *MethodType_str[];

inline const char*
MethodStr(const MethodType m)
{
    return MethodType_str[m];
}

}; // namespace Http

#endif /* SQUID_SRC_HTTP_METHODTYPE_H */
