#include "Storage.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include "Path.hpp"

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() { readFromFile(); }

Storage::~Storage() { sync(); }

std::shared_ptr<Storage> Storage::getInstance() {
    if (m_instance == nullptr) {
        m_instance = std::shared_ptr<Storage>(new Storage);
    }
    return m_instance;
}

void Storage::createUser(const User &t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

std::list<User> Storage::queryUser(
    std::function<bool(const User &)> filter) const {
    std::list<User> users;
    if (!m_userList.empty())
        std::copy_if(m_userList.cbegin(), m_userList.cend(),
                     back_inserter(users), filter);
    return std::move(users);
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    int count = 0;
    auto iter = m_userList.begin();
    auto end = m_userList.end();
    for (auto& user : m_userList) {
        if (filter(user)) {
            ++count;
            switcher(user);
        }
    }
    sync();
    return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int old_size = m_userList.size();
    if (!m_userList.empty())
        m_userList.erase(
            std::remove_if(m_userList.begin(), m_userList.end(), filter),
            m_userList.end());
    sync();
    return old_size - m_userList.size();
}

void Storage::createMeeting(const Meeting &t_meeting) {
    m_meetingList.push_back(t_meeting);
    sync();
}

std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> meetings;
    if (!m_meetingList.empty())
        std::copy_if(m_meetingList.cbegin(), m_meetingList.cend(),
                     back_inserter(meetings), filter);
    return std::move(meetings);
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher) {
    int count = 0;
    for (auto& meeting : m_meetingList) {
        if (filter(meeting)) {
            ++count;
            switcher(meeting);
        }
    }
    sync();
    return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int old_size = m_meetingList.size();
    if (!m_meetingList.empty())
        m_meetingList.erase(
            std::remove_if(m_meetingList.begin(), m_meetingList.end(), filter),
            m_meetingList.end());
    sync();
    return old_size - m_meetingList.size();
}

bool Storage::sync() { return writeToFile(); }

std::string csv_value(const std::string &value) {
    std::string formatted;
    for (auto ch : value) {
        formatted += ch;
        if (ch == '"') formatted += ch;
    }
    return std::move(formatted);
}

bool Storage::writeToFile() {
    std::ofstream user_file(Path::userPath);
    std::ofstream meeting_file(Path::meetingPath);
    if (!user_file.good() || !meeting_file.good()) return false;
    for (auto user : m_userList) {
        user_file << '"' << csv_value(user.getName()) << '"' << ",";
        user_file << '"' << csv_value(user.getPassword()) << '"' << ",";
        user_file << '"' << csv_value(user.getEmail()) << '"' << ",";
        user_file << '"' << csv_value(user.getPhone()) << '"' << std::endl;
    }

    for (auto meeting : m_meetingList) {
        meeting_file << '"' << csv_value(meeting.getSponsor()) << '"' << ",";
        auto participators = meeting.getParticipator();
        std::string participators_list = "";
        for (int i = 0; i < participators.size(); ++i) {
            if (i) participators_list += "&";
            participators_list += csv_value(participators[i]);
        }
        meeting_file << '"' << csv_value(participators_list) << '"' << ',';
        meeting_file << '"' << Date::dateToString(meeting.getStartDate()) << '"'
                     << ",";
        meeting_file << '"' << Date::dateToString(meeting.getEndDate()) << '"'
                     << ",";
        meeting_file << '"' << csv_value(meeting.getTitle()) << '"'
                     << std::endl;
    }

    return true;
}

std::vector<std::string> parse_participators(std::string s) {
    std::vector<std::string> participators;
    std::string participator;
    for (auto ch : s) {
        if (ch == '&') {
            participators.push_back(participator);
            participator.clear();
        } else {
            participator += ch;
        }
    }
    participators.push_back(participator);
    participator.clear();
    return participators;
}

std::list<std::list<std::string>> parse_csv(std::istream &file) {
    std::list<std::list<std::string>> lines;
    while (!file.eof()) {
        std::list<std::string> line_items;
        bool same_line = true;
        int ch;
        while (same_line && (ch = file.get()) != EOF) {
            std::string item;
            if (ch == '"') {
                while ((ch = file.get()) != EOF) {
                    if (ch == '"') {
                        ch = file.get();
                        if (ch == ',') break;
                        if (ch == '\n' || ch == EOF) {
                            same_line = false;
                            break;
                        }
                    }
                    item += ch;
                }
                line_items.push_back(item);
            } else {
                item += ch;
                while ((ch = file.get()) != EOF) {
                    if (ch == ',') break;
                    if (ch == '\n') {
                        same_line = false;
                        break;
                    }
                    item += ch;
                }
                line_items.push_back(item);
            }
        }
        if (!line_items.empty()) lines.push_back(line_items);
    }
    return lines;
}

bool Storage::readFromFile() {
    std::ifstream user_file(Path::userPath);
    std::ifstream meeting_file(Path::meetingPath);
    if (!user_file.good() || !meeting_file.good()) return false;
    auto users = parse_csv(user_file);
    auto meetings = parse_csv(meeting_file);

    m_userList.clear();
    m_meetingList.clear();

    for (auto user : users) {
        auto username = user.front();
        user.pop_front();
        auto password = user.front();
        user.pop_front();
        auto email = user.front();
        user.pop_front();
        auto phone = user.front();
        user.pop_front();

        createUser(User(username, password, email, phone));
    }

    for (auto meeting : meetings) {
        auto sponsor = meeting.front();
        meeting.pop_front();
        auto participators = parse_participators(meeting.front());
        meeting.pop_front();
        auto start_date = Date::stringToDate(meeting.front());
        meeting.pop_front();
        auto end_date = Date::stringToDate(meeting.front());
        meeting.pop_front();
        auto title = meeting.front();
        meeting.pop_front();

        createMeeting(
            Meeting(sponsor, participators, start_date, end_date, title));
    }
    return true;
}
