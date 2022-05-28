#include "defines.h"

int main(int, char **) {
    // DEBUG_LEVEL only works with Global.
    //ber_set_option(nullptr, LBER_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);
    Call_LDAP_Func(ldap_set_option, nullptr, LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    LdapConnection ldap = {URI};
    ldap.Search(BASE, SCOPE);
}
