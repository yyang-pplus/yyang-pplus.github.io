#pragma once

#include <exception>
#include <iostream>
#include <memory>

#include <ldap.h>


class LdapException : public std::exception {
public:
    explicit LdapException(const int s) noexcept : m_status(s) {}

    [[nodiscard]] const char *what() const noexcept override {
        return ldap_err2string(m_status);
    }

private:
    int m_status = 0;
};

template<typename Function, typename... Args>
inline constexpr void CallLdap(const Function func, Args &&...args) {
    if (const auto status = func(std::forward<Args>(args)...); status != LDAP_SUCCESS) {
        throw LdapException {status};
    }
}

inline void LdapUnbind(LDAP *ld) noexcept {
    CallLdap(ldap_unbind_ext_s, ld, nullptr, nullptr);
}

[[nodiscard]] inline auto CreateLdapHandle(const std::string &uri) noexcept {
    LDAP *ld = nullptr;
    CallLdap(ldap_initialize, &ld, uri.c_str());
    return std::unique_ptr<LDAP, decltype(&LdapUnbind)> {ld, &LdapUnbind};
}

void SimpleSearch(LDAP &ld, const std::string &base, const ber_int_t &scope) noexcept {
    const char *filter = nullptr;
    char **attributes = nullptr;
    LDAPMessage *pMessage = nullptr;

    try {
        CallLdap(
            ldap_search_ext_s,
            &ld,
            base.c_str(),
            scope,
            filter, //NULL may be specified to indicate the library should send the filter (objectClass=*)
            attributes,
            /*attrsonly=*/0,
            nullptr,
            nullptr,
            nullptr,
            0,
            &pMessage);
    } catch (const LdapException &e) {
        std::cerr << e.what() << std::endl;
    }

    ldap_msgfree(pMessage);
}
