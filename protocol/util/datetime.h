#pragma once

#include <cstdint>
#include <tuple>
#include <ctime>

namespace util {

struct Time
{
  explicit Time(int h = 0, int m = 0, int s = 0)
    : hours(h), minutes(m), seconds(s)
  { }

  int hours;
  int minutes;
  int seconds;
};

inline bool operator< (const Time& lhs, const Time& rhs)
{
  return std::tie(lhs.hours, lhs.minutes, lhs.seconds)
       < std::tie(rhs.hours, rhs.minutes, rhs.seconds);
}

inline bool operator== (const Time& lhs, const Time& rhs)
{
  return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes && lhs.seconds == rhs.seconds;
}



struct Date
{
  explicit Date(int y = 0, int m = 0, int d = 0)
    : year(y), month(m), day(d)
  { }

  int year;
  int month;
  int day;
};

inline bool operator< (const Date& lhs, const Date& rhs)
{
  return std::tie(lhs.year, lhs.month, lhs.day)
       < std::tie(rhs.year, rhs.month, rhs.day);
}

inline bool operator== (const Date& lhs, const Date& rhs)
{
  return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
}



struct DateTime
{
  explicit DateTime(const Date& d = Date{}, const Time& t = Time{})
    : date(d), time(t)
  { }

  Date date;
  Time time;
};

inline bool operator< (const DateTime& lhs, const DateTime& rhs)
{
  return std::tie(lhs.date, lhs.time)
       < std::tie(rhs.date, rhs.time);
}

inline bool operator== (const DateTime& lhs, const DateTime& rhs)
{
  return lhs.date == rhs.date && lhs.time == rhs.time;
}



static DateTime current_datetime()
{
  std::time_t t = std::time(0);
  std::tm* now = std::localtime(&t);

  return DateTime{
    Date{now->tm_year + 1900, now->tm_mon + 1, now->tm_mday},
    Time{now->tm_hour, now->tm_min, now->tm_sec}
  };
}

} // namespace util
