#include "defines.hpp"
#include "ldap-utils.hpp"


int main() {
    // DEBUG_LEVEL only works with Global.
    CallLdap(ldap_set_option, nullptr, LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    const auto ld = CreateLdapHandle(URI);

    SimpleSearch(*ld, BASE, SCOPE);
}
