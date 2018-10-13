#include "User.hpp"

User::User(const User &t_user) {
    m_name = t_user.m_name;
    m_password = t_user.m_password;
    m_email = t_user.m_email;
    m_phone = t_user.m_phone;
}

std::string User::getName() const {
    return m_name;
}

std::string User::getPhone() const {
    return m_phone;
}

std::string User::getPassword() const {
    return m_password;
}

std::string User::getEmail() const {
    return m_email;
}

void User::setName(std::string t_name)  {
    m_name = t_name;
}

void User::setPhone(std::string t_phone)  {
    m_phone = t_phone;
}

void User::setPassword(std::string t_password) {
    m_password = t_password;
}

void User::setEmail(std::string t_email) {
    m_email = t_email;
}