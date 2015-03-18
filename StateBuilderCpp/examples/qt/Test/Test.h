#include <QtCore/QObject>
#include <QtTest/QtTest>

class UniTest : public QObject {
  
  Q_OBJECT 
  
  public:
    UniTest();

  public slots:

  private slots:
    void testParallelRoot();
    
  private:
    void logInit();
};

