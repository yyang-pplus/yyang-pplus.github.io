#define LDAP_DEBUG_ANY (-1)

constexpr int DEBUG_LEVEL = LDAP_DEBUG_ANY;

ldap_set_option(nullptr, LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);
