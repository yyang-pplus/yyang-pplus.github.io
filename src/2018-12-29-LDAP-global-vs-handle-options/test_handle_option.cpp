#include "defines.h"
#include "ldap_connection.h"


int main(int, char **) {
    const auto ld = CreateLdapHandle(URI);

    CallLdap(ldap_set_option, ld.get(), LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    SimpleSearch(*ld, BASE, SCOPE);
}
