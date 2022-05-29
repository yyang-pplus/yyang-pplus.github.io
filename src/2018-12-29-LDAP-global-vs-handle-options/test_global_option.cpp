#include "defines.h"
#include "ldap_connection.h"


int main(int, char **) {
    // DEBUG_LEVEL only works with Global.
    CallLdap(ldap_set_option, nullptr, LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    const auto ld = CreateLdapHandle(URI);

    SimpleSearch(*ld, BASE, SCOPE);
}
