#include <ldap.h>

int ldap_get_option(LDAP *ld, int option, void *outvalue);

int ldap_set_option(LDAP *ld, int option, const void *invalue);
