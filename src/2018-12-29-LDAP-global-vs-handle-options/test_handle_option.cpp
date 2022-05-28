#include "defines.h"

int main(int, char **) {
    LdapConnection ldap = {URI};

    //ldap.BerSetOption(LBER_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);
    ldap.LdapSetOption(LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    ldap.Search(BASE, SCOPE);
}
