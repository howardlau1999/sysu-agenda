#include "Date.hpp"

const char *Date::default_date = "0000-00-00/00:00";
const char *Date::date_format = "%04d-%02d-%02d/%02d:%02d";

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
    : m_year(t_year),
      m_month(t_month),
      m_day(t_day),
      m_hour(t_hour),
      m_minute(t_minute){};

int Date::getYear() const { return m_year; }

void Date::setYear(int t_year) { m_year = t_year; }

int Date::getMonth() const { return m_month; }

void Date::setMonth(int t_month) { m_month = t_month; }

int Date::getDay() const { return m_day; }

void Date::setDay(int t_day) { m_day = t_day; }

int Date::getHour() const { return m_hour; }

void Date::setHour(int t_hour) { m_hour = t_hour; }

int Date::getMinute() const { return m_minute; }

void Date::setMinute(int t_minute) { m_minute = t_minute; }

Date &Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}

bool Date::operator==(const Date &t_date) const {
    return m_year == t_date.m_year && m_month == t_date.m_month &&
           m_day == t_date.m_day && m_hour == t_date.m_hour &&
           m_minute == t_date.m_minute;
}

bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.m_year) return true;
    if (m_year < t_date.m_year) return false;

    if (m_month > t_date.m_month) return true;
    if (m_month < t_date.m_month) return false;

    if (m_day > t_date.m_day) return true;
    if (m_day < t_date.m_day) return false;

    if (m_hour > t_date.m_hour) return true;
    if (m_hour < t_date.m_hour) return false;

    if (m_minute > t_date.m_minute) return true;
    if (m_minute < t_date.m_minute) return false;

    return false;
}

bool Date::operator<(const Date &t_date) const {
    return !(*this > t_date) && !(*this == t_date);
}

bool Date::operator>=(const Date &t_date) const { return !(*this < t_date); }

bool Date::operator<=(const Date &t_date) const { return !(*this > t_date); }

bool Date::isValid(const Date t_date) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (t_date.m_year < 0 || t_date.m_year > 9999) return false;
    if (t_date.m_month < 1 || t_date.m_month > 12) return false;
    if (t_date.m_day < 1) return false;
    if (t_date.m_month == 2 &&
        (t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0 ||
         t_date.m_year % 400 == 0)) {
        if (t_date.m_month > 29) return false;
    }
    if (t_date.m_month > days[t_date.m_month - 1]) return false;
    if (t_date.m_hour < 0 || t_date.m_hour > 23) return false;
    if (t_date.m_minute < 0 || t_date.m_hour > 59) return false;

    return true;
}

std::string Date::dateToString(Date t_date) {
    if (!Date::isValid(t_date)) return default_date;
    char date_string[17];
    sprintf(date_string, date_format, t_date.m_year, t_date.m_month,
            t_date.m_day, t_date.m_hour, t_date.m_minute);
    return date_string;
}

Date Date::stringToDate(std::string t_dateString) {
    Date t_date;
    if (5 == sscanf(t_dateString.c_str(), date_format, &t_date.m_year,
                    &t_date.m_month, &t_date.m_day, &t_date.m_hour,
                    &t_date.m_minute)) {
        if (isValid(t_date))
            return t_date;
        else
            return Date();
    } else {
        return Date();
    }
}