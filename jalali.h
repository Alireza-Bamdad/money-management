#ifndef JALALI_H
#define JALALI_H

#include <QDate>

class Jalali {
public:
    static QDate gregorianToJalali(QDate gDate) {
        int gy = gDate.year();
        int gm = gDate.month();
        int gd = gDate.day();

        int g_d_m[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
        int jy, jm, jd;
        int gy2 = (gm > 2) ? (gy + 1) : gy;

        int days = 355666 + (365 * gy) + ((gy2 + 3) / 4)
                   - ((gy2 + 99) / 100) + ((gy2 + 399) / 400) + gd + g_d_m[gm - 1];

        jy = -1595 + (33 * (days / 12053));
        days %= 12053;
        jy += 4 * (days / 1461);
        days %= 1461;

        if (days > 365) {
            jy += (days - 1) / 365;
            days = (days - 1) % 365;
        }

        int jm_table[] = {31, (jy % 4 == 3) ? 30 : 29};  // آخرین ماه‌ها

        for (jm = 1; jm <= 12; jm++) {
            int v = (jm <= 6) ? 31 : (jm <= 11 ? 30 : jm_table[1]);
            if (days < v) break;
            days -= v;
        }
        jd = days + 1;

        return QDate(jy, jm, jd);
    }
};

#endif // JALALI_H
