
#include "AcsAppBase.h"

using namespace std;

class AcsRebootApp : public AcsAppBase {
  
  Q_OBJECT 
  
  public:
    
    AcsRebootApp(const QString &host);
    
    void addRequest();
};

