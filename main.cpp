#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>
#include <queue>
#include <unordered_map>
#include <stack>
using namespace std;

struct SearchResult {
    QString result;
    QString Grades;
    QString Attendance;
};
class SearchBox : public QWidget {
    Q_OBJECT
public:
    SearchBox(QWidget *parent = nullptr) : QWidget(parent) {
        search = new QLineEdit;
        search->setPlaceholderText("What's on Your Mind?");
        searchbutton = new QPushButton("Search");
        backButton = new QPushButton("Back");
        forwardButton = new QPushButton("Forward");

        connect(searchbutton, &QPushButton::clicked, this, &SearchBox::performSearch);
        connect(backButton, &QPushButton::clicked, this, &SearchBox::navigateBack);
        connect(forwardButton, &QPushButton::clicked, this, &SearchBox::navigateForward);

        // Create layout for search box and navigation buttons
        QHBoxLayout *topLayout = new QHBoxLayout;
        topLayout->addWidget(search);
        topLayout->addWidget(searchbutton);
        topLayout->addWidget(backButton);
        topLayout->addWidget(forwardButton);

        // Create layout for search results
        resultLayout = new QVBoxLayout;
        layout = new QVBoxLayout;
        layout->addLayout(topLayout);
        layout->addLayout(resultLayout);

        initialLabel = new QLabel("IGIS");
        initialLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(initialLabel);

        setLayout(layout);
        setWindowTitle("Search Engine");
        initializeGrade();
        initializeAttendance();
        updateNavigation();
    }

private slots:
    void performSearch() {
        QString query = search->text();
        if (query.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please provide a valid search.");
            return;
        }
        QStringList searchResults = bfs(query);
        if (!searchResults.isEmpty()) {
            std::priority_queue<std::pair<int, QString>> pq;
            for (const QString& result : searchResults) {
                int rank = 0;
                QString lowerResult = result.toLower();
                int index = lowerResult.indexOf(query);
                while (index != -1) {
                    ++rank;
                    index = lowerResult.indexOf(query, index + 1);
                }
                pq.push({rank, result});
            }

            QVector<SearchResult> detailedResults;
            while (!pq.empty()) {
                auto [rank, result] = pq.top();
                pq.pop();
                SearchResult searchResult;
                searchResult.result = result;
                auto it = Grade.find(result);
                if (it != Grade.end()) {
                    searchResult.Grades = it->second;
                } else {
                    searchResult.Grades = "No details available for this result";
                }
                auto it2 = Attendance.find(result);
                if (it2 != Attendance.end()) {
                    searchResult.Attendance = it2->second;
                } else {
                    searchResult.Attendance = "No details available for this result";
                }
                detailedResults.append(searchResult);
            }
            if (!currentResults.isEmpty()) {
                navigationStack.push(currentResults);
            }
            currentResults = detailedResults;
            while (!forwardQueue.empty()) {
                forwardQueue.pop();
            }
            updateResults(detailedResults);
        } else {
            QMessageBox::information(this, "Error", "No results found for the query.");
        }
        updateNavigation();
    }

    void navigateBack() {
        if (!navigationStack.empty()) {
            forwardQueue.push(currentResults);
            currentResults = navigationStack.top();
            navigationStack.pop();
            updateResults(currentResults);
        }
        updateNavigation();
    }

    void navigateForward() {
        if (!forwardQueue.empty()) {
            navigationStack.push(currentResults);
            currentResults = forwardQueue.front();
            forwardQueue.pop();
            updateResults(currentResults);
        }
        updateNavigation();
    }

private:
    void updateNavigation() {
        backButton->setEnabled(!navigationStack.empty());
        forwardButton->setEnabled(!forwardQueue.empty());
    }

    void updateResults(const QVector<SearchResult>& results) {
        // Clear the result layout before updating
        clearLayout(resultLayout);

        for (const SearchResult& searchResult : results) {
            QLabel *resultLabel = new QLabel(searchResult.result);
            QPushButton *GradesButton = new QPushButton("Grades");
            QPushButton *AttendanceButton = new QPushButton("Attendance");
            resultLayout->addWidget(resultLabel);
            resultLayout->addWidget(GradesButton);
            resultLayout->addWidget(AttendanceButton);
            resultLabel->setAlignment(Qt::AlignCenter);
            connect(GradesButton, &QPushButton::clicked, [this, searchResult]() {
                displayGrades(searchResult.Grades);
            });
            connect(AttendanceButton, &QPushButton::clicked, [this, searchResult]() {
                displayAttendance(searchResult.Attendance);
            });
        }
    }

    void displayGrades(const QString& grades) {
        QMessageBox::information(this, "Grades", grades);
    }

    void displayAttendance(const QString& attendance) {
        QMessageBox::information(this, "Attendance", attendance);
    }

    void clearLayout(QLayout* layout) {
        while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
    }

    QLineEdit *search;
    QPushButton *searchbutton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QLabel *initialLabel;
    QVBoxLayout *layout;
    QVBoxLayout *resultLayout; // Layout for search results
    QVector<SearchResult> currentResults;
    stack<QVector<SearchResult>> navigationStack;
    queue<QVector<SearchResult>> forwardQueue;
    unordered_map<QString, QString> Grade;
    unordered_map<QString, QString> Attendance;
    QString Graph[4][1] = {
        {"BE20"},
        {"BE21"},
        {"BE22"},
        {"BE23"}
    };

    QStringList bfs(const QString& query) {
        QStringList searchResults;
        unordered_map<QString, bool> visited;
        queue<QString> q;
        for (int i = 0; i < 4; i++) {
            visited[Graph[i][0]] = false;
        }
        for (int i = 0; i < 4; i++) {
            if (Graph[i][0].contains(query, Qt::CaseInsensitive)) {
                q.push(Graph[i][0]);
                visited[Graph[i][0]] = true;
                break;
            }
        }
        while (!q.empty()) {
            QString current = q.front();
            q.pop();
            searchResults.append(current);
            for (int i = 0; i < 4; i++) {
                QString adjacent = Graph[i][0];
                if (!visited[adjacent] && adjacent.contains(query, Qt::CaseInsensitive)) {
                    q.push(adjacent);
                    visited[adjacent] = true;
                }
            }
        }
        return searchResults;
    }

    void initializeGrade() {
        Grade["BE20"] = "Sharjeel: A";
        Grade["BE21"] = "Shaf: A";
        Grade["BE22"] = "Iliyan: A";
        Grade["BE23"] = "Mehdi: A";
    }

    void initializeAttendance() {
        Attendance["BE20"] = "Sharjeel: 100%";
        Attendance["BE21"] = "Shaf: 100%";
        Attendance["BE22"] = "Iliyan: 100%";
        Attendance["BE23"] = "Mehdi: 100%";
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SearchBox SEO;
    SEO.show();
    return app.exec();
}
#include "main.moc"
