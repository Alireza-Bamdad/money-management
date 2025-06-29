# Qt  Finance Manager (Persian UI)

A  desktop application built with **C++ and Qt** for managing personal income and expenses, complete with **Persian calendar support**, **RTL interface**, and **monthly cost charts**.

## ✨ Features

- 🧾 **Income Management**: Add, view, and delete income entries with source and amount.
- 💸 **Expense Tracking**: Categorize expenses (e.g., food, transport) and monitor total spending.
- 📆 **Persian Date Support**: Automatically shows current date using Jalali (Shamsi) calendar.
- 📊 **Monthly Bar Chart**: Visualize monthly expenses by category using interactive bar charts.
- 💾 **Data Persistence**: Automatically saves and loads data to/from `data.txt`.
- 🔢 **Formatted Numbers**: Persian digits and thousands separators for better readability.
- 🧭 **RTL Layout**: Full right-to-left support for Persian users.

## 📸 Screenshots

![Screenshot (18)](https://github.com/user-attachments/assets/5938d416-1ee3-4b3a-ac1b-3b000eeba067)

## 🛠️ Technologies Used

- **C++**
- **Qt 5/6 (Widgets + QtCharts)**
- **Custom Jalali (Shamsi) date conversion**
- **QFile/QTextStream for file handling**
- **QLocale for number formatting**

## 📂 Project Structure

```bash
.
├── mainwindow.h/cpp       # Main logic for income/cost/chart handling
├── main.cpp               # Application entry point
├── ui_mainwindow.h        # Auto-generated UI file
├── jalali.h/cpp           # Gregorian to Jalali date conversion logic
├── data.txt               # Saved user data (generated on runtime)
