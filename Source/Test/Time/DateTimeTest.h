/**
 * @file    DateTimeTest.h
 * @author  ggomdyu
 * @since   08/02/2019
 */

#pragma once
#include <cassert>

#include "Time/DateTime.h"
#include "Time/TimeZoneInfo.h"
#include "Diagnostics/Debug.h"

#include "../Test.h"

namespace tgon
{

class DateTimeTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
        auto dt = DateTime(2017, 10, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt, 2017, 10, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddDays(2), 2017, 10, 22, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddDays(-5), 2017, 10, 15, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddYears(-5), 2012, 10, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddYears(5), 2022, 10, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddYears(-80).AddMonths(-27), 1935, 7, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddYears(123), 2140, 10, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddMonths(5), 2018, 3, 20, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddMonths(-5).AddDays(277).AddDays(-135), 2017, 10, 9, 10, 0, 5, DateTimeKind::Local);
        EvaluateDateTime(dt.AddSeconds(-234324234.123), 2010, 5, 18, 7, 56, 10, DateTimeKind::Local);

        // The below tests are based on UTC+9, so some countries except S.K and Japan would occur wrong test result.
        EvaluateDateTime(dt.ToUniversalTime(), 2017, 10, 20, 1, 0, 5, DateTimeKind::Utc);
        EvaluateDateTime(dt.ToLocalTime(), 2017, 10, 20, 10, 0, 5, DateTimeKind::Local);
        assert(dt.ToFileTime() == 131529348050000000);
        assert(dt.ToFileTimeUtc() == 131529348050000000);
    }

private:
    DateTime EvaluateDateTime(DateTime dt, int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind dateTimeKind)
    {
        DateTimeToString(dt);
        assert(dt.GetYear() == year && dt.GetMonth() == month && dt.GetDay() == day && dt.GetHour() == hour && dt.GetMinute() == minute && dt.GetSecond() == second && dt.GetKind() == dateTimeKind);

        return dt;
    }

    void DateTimeToString(DateTime dt)
    {
        //Debug::Log(LogLevel::Debug, "%d-%d-%d %d:%d:%d\n", dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
    }
};

} /* namespace tgon */
