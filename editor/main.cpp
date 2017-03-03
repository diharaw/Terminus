#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QWidget>
#include <QFile>

#define T_SAFE_DELETE(x) if(x) { delete x; x = nullptr; }

struct PlatformState
{
    QApplication* app;
    MainWindow*   window;
};

PlatformState g_platform;

namespace platform
{
    void initialize(int argc, char *argv[])
    {
        g_platform.app = new QApplication(argc, argv);
        g_platform.window = new MainWindow();

        QSettings settings("MyCompany", "MyApp");
        g_platform.window->restoreGeometry(settings.value("geometry").toByteArray());
        g_platform.window->restoreState(settings.value("windowState").toByteArray());

        // Load an application style
        QFile styleFile( ":/qdarkstyle/style.qss" );
        styleFile.open( QFile::ReadOnly );

        // Apply the loaded stylesheet
        QString style( styleFile.readAll() );
        g_platform.app->setStyleSheet( style );

        g_platform.window->show();
    }

    void update()
    {
        g_platform.app->processEvents();
    }

    void request_shutdown()
    {
        g_platform.window->isRunning = false;
    }

    bool is_running()
    {
        return g_platform.window->isRunning;
    }

    void shutdown()
    {
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("geometry", g_platform.window->saveGeometry());
        settings.setValue("windowState", g_platform.window->saveState());

        g_platform.app->exit();

        T_SAFE_DELETE(g_platform.window);
        T_SAFE_DELETE(g_platform.app);
    }
}

class TerminusApp
{
public:
    TerminusApp()
    {

    }

    ~TerminusApp()
    {

    }

    void initialize(int argc, char *argv[])
    {
        platform::initialize(argc, argv);
    }


    void run()
    {
        while(platform::is_running())
        {
            update();
        }
    }

    void shutdown()
    {
        platform::shutdown();
    }

private:

    void update()
    {
        platform::update();
    }
};

int main(int argc, char *argv[])
{
    TerminusApp app;

    app.initialize(argc, argv);
    app.run();
    app.shutdown();

    return 0;
}
