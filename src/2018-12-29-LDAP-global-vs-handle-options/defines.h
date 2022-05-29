#pragma once

#include <string>

#include <ldap.h>


const std::string URI = "ldap://";
const std::string BASE = "dc=yyang-pplus,dc=github,dc=io";
constexpr auto SCOPE = LDAP_SCOPE_BASE;
constexpr auto DEBUG_LEVEL = -1;
