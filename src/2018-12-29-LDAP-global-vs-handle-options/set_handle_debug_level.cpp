#include "defines.hpp"
#include "ldap_utils.hpp"


int main(int, char **) {
    const auto ld = CreateLdapHandle(URI);

    CallLdap(ldap_set_option, ld.get(), LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    SimpleSearch(*ld, BASE, SCOPE);
}
